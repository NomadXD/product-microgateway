// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: wso2/discovery/config/enforcer/api_key_config.proto

package org.wso2.choreo.connect.discovery.config.enforcer;

public interface APIKeyConfigOrBuilder extends
    // @@protoc_insertion_point(interface_extends:wso2.discovery.config.enforcer.APIKeyConfig)
    com.google.protobuf.MessageOrBuilder {

  /**
   * <code>string oauthAgentURL = 1;</code>
   * @return The oauthAgentURL.
   */
  java.lang.String getOauthAgentURL();
  /**
   * <code>string oauthAgentURL = 1;</code>
   * @return The bytes for oauthAgentURL.
   */
  com.google.protobuf.ByteString
      getOauthAgentURLBytes();

  /**
   * <code>repeated string internalAPIKeyHeaders = 2;</code>
   * @return A list containing the internalAPIKeyHeaders.
   */
  java.util.List<java.lang.String>
      getInternalAPIKeyHeadersList();
  /**
   * <code>repeated string internalAPIKeyHeaders = 2;</code>
   * @return The count of internalAPIKeyHeaders.
   */
  int getInternalAPIKeyHeadersCount();
  /**
   * <code>repeated string internalAPIKeyHeaders = 2;</code>
   * @param index The index of the element to return.
   * @return The internalAPIKeyHeaders at the given index.
   */
  java.lang.String getInternalAPIKeyHeaders(int index);
  /**
   * <code>repeated string internalAPIKeyHeaders = 2;</code>
   * @param index The index of the value to return.
   * @return The bytes of the internalAPIKeyHeaders at the given index.
   */
  com.google.protobuf.ByteString
      getInternalAPIKeyHeadersBytes(int index);
}
