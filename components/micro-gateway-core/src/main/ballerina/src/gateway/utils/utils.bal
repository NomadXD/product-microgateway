// Copyright (c)  WSO2 Inc. (http://www.wso2.org) All Rights Reserved.
//
// WSO2 Inc. licenses this file to you under the Apache License,
// Version 2.0 (the "License"); you may not use this file except
// in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.

import ballerina/http;
import ballerina/log;
import ballerina/auth;
import ballerina/config;
import ballerina/runtime;
import ballerina/time;
import ballerina/io;
import ballerina/reflect;
import ballerina/internal;
import ballerina/system;
import ballerina/encoding;
import ballerina/lang.'string;
import ballerina/lang.'int;

map<http:HttpResourceConfig?> resourceAnnotationMap = {};
map<http:HttpServiceConfig?> serviceAnnotationMap = {};
map<TierConfiguration?> resourceTierAnnotationMap = {};
map<APIConfiguration?> apiConfigAnnotationMap = {};

public function populateAnnotationMaps(string serviceName, service s, string[] resourceArray) {
    foreach string resourceFunction in resourceArray {
        resourceAnnotationMap[resourceFunction] = <http:HttpResourceConfig?>reflect:getResourceAnnotations(s, resourceFunction, RESOURCE_ANN_NAME,ANN_PACKAGE);
        resourceTierAnnotationMap[resourceFunction] = <TierConfiguration?>reflect:getResourceAnnotations(s, resourceFunction, RESOURCE_ANN_NAME, ANN_PACKAGE);
    }
    serviceAnnotationMap[serviceName] = <http:HttpServiceConfig?>reflect:getServiceAnnotations(s, RESOURCE_ANN_NAME, ANN_PACKAGE);
    apiConfigAnnotationMap[serviceName] = <APIConfiguration?>reflect:getServiceAnnotations(s, API_ANN_NAME, GATEWAY_ANN_PACKAGE);
}

# Retrieve the key validation request dto from filter context
#
# + return - api key validation request dto
public function getKeyValidationRequestObject(runtime:InvocationContext context) returns APIRequestMetaDataDto {
    APIRequestMetaDataDto apiKeyValidationRequest = {};
    string serviceName = runtime:getInvocationContext().attributes["ServiceName"].toString();
    string resourceName = runtime:getInvocationContext().attributes["ResourceName"].toString();
    http:HttpServiceConfig httpServiceConfig =  <http:HttpServiceConfig>serviceAnnotationMap[getServiceName(serviceName)];
    http:HttpResourceConfig? httpResourceConfig = resourceAnnotationMap[resourceName];
    if (httpResourceConfig is http:HttpResourceConfig) {
       apiKeyValidationRequest.matchingResource = <string>httpResourceConfig.path;
       apiKeyValidationRequest.httpVerb = <string>httpResourceConfig.methods[0];
    }
    string apiContext = <string>httpServiceConfig.basePath;
    APIConfiguration? apiConfig = apiConfigAnnotationMap[getServiceName(serviceName)];
    string apiVersion;
    if (apiConfig is APIConfiguration) {
     apiVersion = <string>apiConfig.apiVersion;
     }
    apiKeyValidationRequest.apiVersion = apiVersion;
    if (!contains(apiContext,apiVersion)) {
        if (hasSuffix(apiContext,PATH_SEPERATOR)) {
            apiContext = apiContext + apiVersion;
        } else {
            apiContext = apiContext + PATH_SEPERATOR + apiVersion;
        }
    }
    apiKeyValidationRequest.context = apiContext;
    apiKeyValidationRequest.requiredAuthenticationLevel = ANY_AUTHENTICATION_LEVEL;
    apiKeyValidationRequest.clientDomain = "*";
    
    apiKeyValidationRequest.accessToken = <string>runtime:getInvocationContext().attributes[ACCESS_TOKEN_ATTR];
    printDebug(KEY_UTILS, "Created request meta-data object with context: " + apiContext
            + ", resource: " + apiKeyValidationRequest.matchingResource
            + ", verb: " + apiKeyValidationRequest.httpVerb);
    return apiKeyValidationRequest;

}

# Retrieve the correct service name from service name that contains object reference(for ex; MyService$$service$0).
# This method is a work around due to ballerina filter context returns wrong service name
#
# + return - service name
public function getServiceName(string serviceObjectName) returns string {
    return split(serviceObjectName, "\\$")[0];
}

public function getTenantFromBasePath(string basePath) returns string {
    string[] splittedArray = split(basePath, "/");
    return splittedArray[splittedArray.length() - 1];
}

public function isAccessTokenExpired(APIKeyValidationDto apiKeyValidationDto) returns boolean {
    int|error validityPeriod;
    int|error issuedTime;
    string? validPeriod = apiKeyValidationDto?.validityPeriod;
    string? issueTime = apiKeyValidationDto?.issuedTime;
    if (validityPeriod is string) {
        validityPeriod =  'int:fromString(validityPeriod);
    }
    if (issueTime is string ) {
        issuedTime = 'int:fromString(issueTime);
    }
    int timestampSkew = getConfigIntValue(KM_CONF_INSTANCE_ID, TIMESTAMP_SKEW, 5000);
    int currentTime = time:currentTime().time;
    int intMaxValue = 9223372036854775807;
    if (!(validityPeriod is int) || !(issuedTime is int)) {
        error e = error("Error while converting time stamps to integer when retrieved from cache");
        panic e;
    }
    if(validityPeriod is int && issuedTime is int) {
        if( validityPeriod != intMaxValue &&
                // For cases where validityPeriod is closer to int.MAX_VALUE (then issuedTime + validityPeriod would spill
                // over and would produce a negative value)
                (currentTime - timestampSkew) > validityPeriod) {
            if ((currentTime - timestampSkew) > (issuedTime + validityPeriod)) {
                apiKeyValidationDto.validationStatus = API_AUTH_INVALID_CREDENTIALS_STRING;
                return true;
            }
        }
    }
    return false;
}
public function getContext(http:FilterContext context) returns (string) {
    http:HttpServiceConfig httpServiceConfig = <http:HttpServiceConfig>serviceAnnotationMap[getServiceName(context.getServiceName())];
    return <string>httpServiceConfig.basePath;

}

public function getClientIp(http:Request request, http:Caller caller) returns (string) {
    string clientIp;
    if (request.hasHeader(X_FORWARD_FOR_HEADER)) {
        clientIp = request.getHeader(X_FORWARD_FOR_HEADER);
        int? idx = clientIp.indexOf(",");
        if (idx is int) {
            clientIp = clientIp.substring(0, idx);
        }
    } else {
        clientIp = caller.remoteAddress.host;
    }
    return clientIp;
}

public function extractAccessToken(http:Request req, string authHeaderName) returns (string|error) {
    string authHeader = req.getHeader(authHeaderName);
    string[] authHeaderComponents = split(authHeader, " ");
    if (authHeaderComponents.length() != 2){
        return handleError("Incorrect bearer authentication header format");
    }
    return authHeaderComponents[1];
}

public function handleError(string message) returns (error) {
    return error(message);
}

public function getTenantDomain(http:FilterContext context) returns (string) {
    // todo: need to implement to get tenantDomain
    string apiContext = getContext(context);
    string[] splittedContext = split(apiContext, "/");
    if (splittedContext.length() > 3){
        // this check if basepath have /t/domain in
        return splittedContext[2];
    } else {
        return SUPER_TENANT_DOMAIN_NAME;
    }
}

public function getApiName(http:FilterContext context) returns (string) {
    string serviceName = context.getServiceName();
    string apiName = split(serviceName, "__")[0];

    if (contains(apiName, "_")) {
        apiName = replaceAll(apiName, "_", "-");
    }

    return apiName;
}

public function getConfigValue(string instanceId, string property, string defaultValue) returns string {
    return config:getAsString(instanceId + "." + property,  defaultValue);
}

public function getConfigIntValue(string instanceId, string property, int defaultValue) returns int {
    return config:getAsInt(instanceId + "." + property, defaultValue);
}

public function getConfigBooleanValue(string instanceId, string property, boolean defaultValue) returns boolean {
    return config:getAsBoolean(instanceId + "." + property, defaultValue);
}

public function getConfigFloatValue(string instanceId, string property, float defaultValue) returns float {
    return config:getAsFloat(instanceId + "." + property, defaultValue);
}

public function getConfigMapValue(string property) returns map<any> {
    return config:getAsMap(property);
}

public function setErrorMessageToFilterContext(http:FilterContext context, int errorCode) {
    int status;
    if (errorCode == API_AUTH_GENERAL_ERROR) {
        status = INTERNAL_SERVER_ERROR;
    } else if (errorCode == API_AUTH_INCORRECT_API_RESOURCE ||
        errorCode == API_AUTH_FORBIDDEN ||
        errorCode == INVALID_SCOPE) {
        status = FORBIDDEN;
    } else if(errorCode == INVALID_ENTITY) {
        status = UNPROCESSABLE_ENTITY;
    } else if(errorCode == INVALID_RESPONSE) {
        status = INTERNAL_SERVER_ERROR;
    } else {
        status = UNAUTHORIZED;

    }
    context.attributes[HTTP_STATUS_CODE] = status;
    context.attributes[FILTER_FAILED] = true;
    context.attributes[ERROR_CODE] = errorCode;
    string errorMessage = getAuthenticationFailureMessage(errorCode);
    context.attributes[ERROR_MESSAGE] = errorMessage;
    context.attributes[ERROR_DESCRIPTION] = getFailureMessageDetailDescription(context, errorCode, errorMessage);
}

# Default error response sender with json error response
public function sendErrorResponse(http:Caller caller, http:Request request, http:FilterContext context) {
    string errorDescription = <string>context.attributes[ERROR_DESCRIPTION];
    string errorMesssage = <string>context.attributes[ERROR_MESSAGE];
    int errorCode = <int>context.attributes[ERROR_CODE];
    http:Response response = new;
    response.statusCode = <int>context.attributes[HTTP_STATUS_CODE];
    response.setContentType(APPLICATION_JSON);
    json payload = { fault: {
        code: errorCode,
        message: errorMesssage,
        description: errorDescription
    } };
    response.setJsonPayload(payload);
    var value = caller->respond(response);
    if(value is error) {
    log:printError("Error occurred while sending the error response", err = value);
    }
}

public function getAuthorizationHeader(runtime:InvocationContext context) returns string {
    string serviceName = runtime:getInvocationContext().attributes["ServiceName"].toString();
    APIConfiguration? apiConfig = apiConfigAnnotationMap[getServiceName(serviceName)];
    string authHeader = "";
    string? annotatedHeadeName = apiConfig["authorizationHeader"];
    if(annotatedHeadeName is string) {
        authHeader = annotatedHeadeName;
    }
    if (authHeader == "") {
        authHeader = getConfigValue(AUTH_CONF_INSTANCE_ID, AUTH_HEADER_NAME, AUTHORIZATION_HEADER);
    }
    return authHeader;

}

public function getCurrentTime() returns int {
    time:Time currentTime = time:currentTime();
    int time = currentTime.time;
    return time;

}

public function rotateFile(string fileName) returns string|error {
    string uuid = system:uuid();
    string fileLocation = retrieveConfig(API_USAGE_PATH, API_USAGE_DIR) + PATH_SEPERATOR;
    int rotatingTimeStamp = getCurrentTime();
    string zipName = fileName + "." + rotatingTimeStamp.toString() + "." + uuid + ZIP_EXTENSION;
    var compressResult = internal:compress(fileName, zipName);
    if(compressResult is error) {
        printFullError(KEY_UTILS, compressResult);
        return compressResult;
    } else {
        printInfo(KEY_UTILS, "File compressed successfully");
        var deleteResult = system:remove(fileName);
            if(deleteResult is ()) {
                printInfo(KEY_UTILS, "Existing file deleted successfully");
            }
            else {
                printFullError(KEY_UTILS, deleteResult);
            }
        return zipName;
    }
}

# Retrieve external configurations defined against a key
#
# + return - Returns the confif value as a string
public function retrieveConfig(string key, string defaultConfig) returns string {
    return config:getAsString(key, defaultConfig);
}

# mask all letters with given text except last 4 charactors.
#
# + return - Returns the masked string value
public function mask(string text) returns string {
    if (text.length() > 4) {
        string last = text.substring(text.length() - 4, text.length());
        string first = replaceAll(text.substring(0, text.length() - 4), ".", "x");
        return first + last;
    } else {
        return "xxxx";
    }
}

# Returns the current message ID (uuid).
#
# + return - The UUID of current context
public function getMessageId() returns string {
    any messageId = runtime:getInvocationContext().attributes[MESSAGE_ID];
    if(messageId is string) {
        if (messageId == "") {
            return "-";
        } else {
            return messageId;
        }
    } else {
        return "-";
    }
}

# Add a error log with provided key (class) and message ID
public function printError(string key, string message) {
    log:printError(io:sprintf("[%s] [%s] %s", key, getMessageId(), message));
}

# Add a debug log with provided key (class) and message ID
public function printDebug(string key, string message) {
    log:printDebug(function() returns string {
            return io:sprintf("[%s] [%s] %s",  key, getMessageId(), message); });
}

# Add a trace log with provided key (class) and message ID
public function printTrace(string key, string message) {
    log:printTrace(function() returns string {
            return io:sprintf("[%s] [%s] %s",  key, getMessageId(), message); });
}

# Add a info log with provided key (class) and message ID
public function printInfo(string key, string message) {
    log:printInfo(io:sprintf("[%s] [%s] %s", key, getMessageId(), message));
}

# Add a full error log with provided key (class) and message ID
public function printFullError(string key, error message) {
    log:printError(io:sprintf("[%s] [%s] %s", key, getMessageId(), message.reason()), err = message);
}

public function setLatency(int starting, http:FilterContext context, string latencyType) {
    int ending = getCurrentTime();
    context.attributes[latencyType] = ending - starting;
    int latency = ending - starting;
    printDebug(KEY_THROTTLE_FILTER, "Throttling latency: " + latency.toString() + "ms");
}

# Check MESSAGE_ID in context and set if it is not
public function checkOrSetMessageID(http:FilterContext context) {
    if (!context.attributes.hasKey(MESSAGE_ID)) {
        context.attributes[MESSAGE_ID] = system:uuid();
    }
}

public function checkExpectHeaderPresent(http:Request request) {
    if (request.expects100Continue()) {
        request.removeHeader(EXPECT_HEADER);
        printDebug(KEY_UTILS, "Expect header is removed from the request");

    }
}

# Encode a given value to base64 format
#
# + return - Returns a string in base64 format
public function encodeValueToBase64(string value) returns string {
    return encoding:encodeBase64(value.toBytes());
}

# Decode a given base64value to base10 format
#
# + return - Returns a string in base10 format
public function decodeValueToBase10(string value) returns string {
    string decodedValue = "";
    var result = encoding:decodeBase64(value);
    if(result is byte[]) {
        decodedValue = encoding:byteArrayToString(result);
    }
    else {
        printError(KEY_UTILS, result.reason());
    }
    return decodedValue;
}

# Extracts host header from request and set it to the filter context
public function setHostHeaderToFilterContext(http:Request request, http:FilterContext context) {
    if(context.attributes[HOSTNAME_PROPERTY] == ()) {
        printDebug(KEY_AUTHN_FILTER, "Setting hostname to filter context");
        if (request.hasHeader(HOST_HEADER_NAME)) {
            context.attributes[HOSTNAME_PROPERTY] = request.getHeader(HOST_HEADER_NAME);

        } else {
            context.attributes[HOSTNAME_PROPERTY] = "localhost";
        }
        printDebug(KEY_UTILS, "Hostname attribute of the filter context is set to : " +
                    <string>context.attributes[HOSTNAME_PROPERTY]);
    } else {
        printDebug(KEY_UTILS, "Hostname attribute of the filter context is already set to : " +
                            <string>context.attributes[HOSTNAME_PROPERTY]);
    }
}

# Logs, prepares, and returns the `AuthenticationError`.
#
# + message -The error message.
# + err - The `error` instance.
# + return - Returns the prepared `AuthenticationError` instance.
function prepareAuthenticationError(string message, error? err = ()) returns http:AuthenticationError {
    log:printDebug(function () returns string { return message; });
    if (err is error) {
        http:AuthenticationError preparedError = error(http:AUTHN_FAILED, message = message, cause = err);
        return preparedError;
    }
    http:AuthenticationError preparedError = error(http:AUTHN_FAILED, message = message);
    return preparedError;
}

