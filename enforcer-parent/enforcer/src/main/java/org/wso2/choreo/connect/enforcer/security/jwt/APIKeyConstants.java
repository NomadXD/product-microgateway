/*
 * Copyright (c) 2024, WSO2 LLC. (https://www.wso2.com)
 *
 * WSO2 LLC. licenses this file to you under the Apache License,
 * Version 2.0 (the "License"); you may not use this file except
 * in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

package org.wso2.choreo.connect.enforcer.security.jwt;

/**
 * Holds PAT related constants.
 */
public class APIKeyConstants {

    public static final String PAT_PREFIX = "chp_";
    public static final String API_KEY_PREFIX = "chk_";

    public static final String API_KEY_JSON_KEY = "key";

    public static final String API_KEY_ID_SEPARATOR = "#";

    public static final String PAT_EXCHANGE_ENDPOINT = "/internal/pat";
    public static final String API_KEY_EXCHANGE_ENDPOINT = "/internal/apiKey/token";
    public static final String INTERNAL_API_KEY_HEADER = "choreo-api-key";
}
