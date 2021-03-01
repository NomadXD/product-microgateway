// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: wso2/discovery/config/enforcer/jms_connection_params.proto

package org.wso2.gateway.discovery.config.enforcer;

public interface JmsConnectionParametersOrBuilder extends
    // @@protoc_insertion_point(interface_extends:wso2.discovery.config.enforcer.JmsConnectionParameters)
    com.google.protobuf.MessageOrBuilder {

  /**
   * <pre>
   * Event listening endpoints of JMS connection
   * </pre>
   *
   * <code>repeated string eventListeningEndpoints = 1;</code>
   * @return A list containing the eventListeningEndpoints.
   */
  java.util.List<java.lang.String>
      getEventListeningEndpointsList();
  /**
   * <pre>
   * Event listening endpoints of JMS connection
   * </pre>
   *
   * <code>repeated string eventListeningEndpoints = 1;</code>
   * @return The count of eventListeningEndpoints.
   */
  int getEventListeningEndpointsCount();
  /**
   * <pre>
   * Event listening endpoints of JMS connection
   * </pre>
   *
   * <code>repeated string eventListeningEndpoints = 1;</code>
   * @param index The index of the element to return.
   * @return The eventListeningEndpoints at the given index.
   */
  java.lang.String getEventListeningEndpoints(int index);
  /**
   * <pre>
   * Event listening endpoints of JMS connection
   * </pre>
   *
   * <code>repeated string eventListeningEndpoints = 1;</code>
   * @param index The index of the value to return.
   * @return The bytes of the eventListeningEndpoints at the given index.
   */
  com.google.protobuf.ByteString
      getEventListeningEndpointsBytes(int index);
}
