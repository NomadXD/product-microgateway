// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: wso2/discovery/service/websocket/frame_service.proto

package org.wso2.choreo.connect.discovery.service.websocket;

public interface WebSocketFrameRequestOrBuilder extends
    // @@protoc_insertion_point(interface_extends:envoy.extensions.filters.http.mgw_wasm_websocket.v3.WebSocketFrameRequest)
    com.google.protobuf.MessageOrBuilder {

  /**
   * <pre>
   * Unique Id / label of the router
   * </pre>
   *
   * <code>string node_id = 1;</code>
   * @return The nodeId.
   */
  java.lang.String getNodeId();
  /**
   * <pre>
   * Unique Id / label of the router
   * </pre>
   *
   * <code>string node_id = 1;</code>
   * @return The bytes for nodeId.
   */
  com.google.protobuf.ByteString
      getNodeIdBytes();

  /**
   * <pre>
   * Ext_authz metadata
   * </pre>
   *
   * <code>.envoy.extensions.filters.http.mgw_wasm_websocket.v3.Metadata metadata = 2;</code>
   * @return Whether the metadata field is set.
   */
  boolean hasMetadata();
  /**
   * <pre>
   * Ext_authz metadata
   * </pre>
   *
   * <code>.envoy.extensions.filters.http.mgw_wasm_websocket.v3.Metadata metadata = 2;</code>
   * @return The metadata.
   */
  org.wso2.choreo.connect.discovery.service.websocket.Metadata getMetadata();
  /**
   * <pre>
   * Ext_authz metadata
   * </pre>
   *
   * <code>.envoy.extensions.filters.http.mgw_wasm_websocket.v3.Metadata metadata = 2;</code>
   */
  org.wso2.choreo.connect.discovery.service.websocket.MetadataOrBuilder getMetadataOrBuilder();

  /**
   * <pre>
   * Web socket frame length
   * </pre>
   *
   * <code>int32 frame_length = 3;</code>
   * @return The frameLength.
   */
  int getFrameLength();

  /**
   * <pre>
   * IP address of upstream service
   * </pre>
   *
   * <code>string remote_ip = 4;</code>
   * @return The remoteIp.
   */
  java.lang.String getRemoteIp();
  /**
   * <pre>
   * IP address of upstream service
   * </pre>
   *
   * <code>string remote_ip = 4;</code>
   * @return The bytes for remoteIp.
   */
  com.google.protobuf.ByteString
      getRemoteIpBytes();
}