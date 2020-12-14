/*
 *  Copyright (c) 2020, WSO2 Inc. (http://www.wso2.org) All Rights Reserved.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 */

// Package api contains the REST API implementation for the adapter
package api

import (
	"archive/zip"
	"bytes"
	"encoding/json"
	"errors"
	"io/ioutil"
	"log"
	"strings"

	"github.com/wso2/micro-gw/loggers"
	"github.com/wso2/micro-gw/pkg/oasparser/model"
	"github.com/wso2/micro-gw/pkg/oasparser/utills"
	xds "github.com/wso2/micro-gw/pkg/xds"
)

// ApplyAPIProject accepts an apictl project (as a byte array) and updates the xds servers based upon the
// content.
// The apictl project must be in zipped format. And all the extensions should be defined with in the openAPI
// definition as only swagger.yaml is taken into consideration here. (http)
// For web socket APIs api.yaml is taken into consideration.
func ApplyAPIProject(payload []byte) error {
	zipReader, err := zip.NewReader(bytes.NewReader(payload), int64(len(payload)))

	if err != nil {
		loggers.LoggerAPI.Errorf("Error occured while unzipping the apictl project. Error: %v", err.Error())
		return err
	}

	// TODO: (VirajSalaka) this won't support for distributed openAPI definition
	var apiJSON []byte     // For WS APIs
	var swaggerJSON []byte // For HTTP APIs
	var apiType string     // HTTP or WS
	var conversionError error

	for _, file := range zipReader.File {
		if strings.HasSuffix(file.Name, "Meta-information/swagger.yaml") {
			loggers.LoggerAPI.Debugf("openAPI file : %v", file.Name)
			swaggerYAML, err := readZipFile(file)
			if err != nil {
				loggers.LoggerAPI.Errorf("Error occured while reading the openapi file. %v", err.Error())
				continue
			}
			swaggerJSON, conversionError = utills.ToJSON(swaggerYAML)
			if conversionError != nil {
				loggers.LoggerAPI.Errorf("Error converting swagger.yaml to json: %v", err.Error())
				return conversionError
			}
		} else if strings.HasSuffix(file.Name, "Meta-information/api.yaml") {
			loggers.LoggerAPI.Debugf("Opening api.yaml file")
			apiYAML, err := readZipFile(file)
			if err != nil {
				loggers.LoggerAPI.Errorf("Error occured while reading the api.yaml file %v", err.Error())
				return err
			}
			apiJSON, conversionError = utills.ToJSON(apiYAML)
			if conversionError != nil {
				loggers.LoggerAPI.Errorf("Error converting api.yaml to json: %v")
				return conversionError
			}
			var apiDef map[string]interface{}
			unmarshalErr := json.Unmarshal(apiJSON, &apiDef)
			if unmarshalErr != nil {
				loggers.LoggerAPI.Errorf("Error occured while parsing apiJSON %v", err.Error())
				return unmarshalErr
			}
			apiType = apiDef["type"].(string)
			loggers.LoggerAPI.Debugf("API type detected for the apictl project received: %v", apiType)

		}
	}
	switch apiType {
	case model.HTTP:
		xds.UpdateEnvoy(swaggerJSON, model.HTTP)
	case model.WS:
		xds.UpdateEnvoy(apiJSON, model.WS)
	default:
		loggers.LoggerAPI.Error("API type is not currently supported")
		return errors.New("API type is not currently supported")
	}
	return nil
}

// ApplyOpenAPIFile accepts an openapi definition as a bytearray and apply the changes to XDS servers.
// TODO: (VirajSalaka) Remove the code segment as it is not in use for the main flow.
func ApplyOpenAPIFile(payload []byte) {
	apiJsn, err := utills.ToJSON(payload)
	if err != nil {
		log.Fatal("Error converting api file to json:", err)
		return
	}
	xds.UpdateEnvoy(apiJsn, model.HTTP)
}

func readZipFile(zf *zip.File) ([]byte, error) {
	f, err := zf.Open()
	if err != nil {
		return nil, err
	}
	defer f.Close()
	return ioutil.ReadAll(f)
}
