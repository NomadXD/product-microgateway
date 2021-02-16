package org.wso2.micro.gateway.enforcer.server;

import io.envoyproxy.envoy.service.auth.v3.CheckRequest;
import io.envoyproxy.envoy.service.auth.v3.CheckResponse;
import io.envoyproxy.envoy.type.v3.StatusCode;
import io.grpc.stub.StreamObserver;
import org.apache.logging.log4j.LogManager;
import org.apache.logging.log4j.Logger;
import org.wso2.micro.gateway.enforcer.api.API;
import org.wso2.micro.gateway.enforcer.api.APIFactory;
import org.wso2.micro.gateway.enforcer.api.RequestContext;
import org.wso2.micro.gateway.enforcer.api.ResponseObject;
import org.wso2.micro.gateway.enforcer.api.config.APIConfig;
import org.wso2.micro.gateway.enforcer.api.config.ResourceConfig;
import org.wso2.micro.gateway.enforcer.constants.APIConstants;
import org.wso2.micro.gateway.enforcer.constants.AdapterConstants;

/**
 * RequestHandler generic interface
 * @param <T> Request type
 * @param <S> Response type
 */
public interface RequestHandler <T, S>{
    S process(T request);
}
