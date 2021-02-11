// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: wso2/discovery/config/enforcer/keystore.proto

package org.wso2.gateway.discovery.config.enforcer;

/**
 * <pre>
 * Listener and client certificate store model
 * </pre>
 *
 * Protobuf type {@code wso2.discovery.config.enforcer.Keystore}
 */
public final class Keystore extends
    com.google.protobuf.GeneratedMessageV3 implements
    // @@protoc_insertion_point(message_implements:wso2.discovery.config.enforcer.Keystore)
    KeystoreOrBuilder {
private static final long serialVersionUID = 0L;
  // Use Keystore.newBuilder() to construct.
  private Keystore(com.google.protobuf.GeneratedMessageV3.Builder<?> builder) {
    super(builder);
  }
  private Keystore() {
    privateKeyLocation_ = "";
    publicKeyLocation_ = "";
  }

  @java.lang.Override
  @SuppressWarnings({"unused"})
  protected java.lang.Object newInstance(
      UnusedPrivateParameter unused) {
    return new Keystore();
  }

  @java.lang.Override
  public final com.google.protobuf.UnknownFieldSet
  getUnknownFields() {
    return this.unknownFields;
  }
  private Keystore(
      com.google.protobuf.CodedInputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    this();
    if (extensionRegistry == null) {
      throw new java.lang.NullPointerException();
    }
    com.google.protobuf.UnknownFieldSet.Builder unknownFields =
        com.google.protobuf.UnknownFieldSet.newBuilder();
    try {
      boolean done = false;
      while (!done) {
        int tag = input.readTag();
        switch (tag) {
          case 0:
            done = true;
            break;
          case 10: {
            java.lang.String s = input.readStringRequireUtf8();

            privateKeyLocation_ = s;
            break;
          }
          case 18: {
            java.lang.String s = input.readStringRequireUtf8();

            publicKeyLocation_ = s;
            break;
          }
          default: {
            if (!parseUnknownField(
                input, unknownFields, extensionRegistry, tag)) {
              done = true;
            }
            break;
          }
        }
      }
    } catch (com.google.protobuf.InvalidProtocolBufferException e) {
      throw e.setUnfinishedMessage(this);
    } catch (java.io.IOException e) {
      throw new com.google.protobuf.InvalidProtocolBufferException(
          e).setUnfinishedMessage(this);
    } finally {
      this.unknownFields = unknownFields.build();
      makeExtensionsImmutable();
    }
  }
  public static final com.google.protobuf.Descriptors.Descriptor
      getDescriptor() {
    return org.wso2.gateway.discovery.config.enforcer.KeystoreProto.internal_static_wso2_discovery_config_enforcer_Keystore_descriptor;
  }

  @java.lang.Override
  protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
      internalGetFieldAccessorTable() {
    return org.wso2.gateway.discovery.config.enforcer.KeystoreProto.internal_static_wso2_discovery_config_enforcer_Keystore_fieldAccessorTable
        .ensureFieldAccessorsInitialized(
            org.wso2.gateway.discovery.config.enforcer.Keystore.class, org.wso2.gateway.discovery.config.enforcer.Keystore.Builder.class);
  }

  public static final int PRIVATEKEYLOCATION_FIELD_NUMBER = 1;
  private volatile java.lang.Object privateKeyLocation_;
  /**
   * <pre>
   * Path to the private-key of the keypair
   * </pre>
   *
   * <code>string privateKeyLocation = 1;</code>
   * @return The privateKeyLocation.
   */
  @java.lang.Override
  public java.lang.String getPrivateKeyLocation() {
    java.lang.Object ref = privateKeyLocation_;
    if (ref instanceof java.lang.String) {
      return (java.lang.String) ref;
    } else {
      com.google.protobuf.ByteString bs = 
          (com.google.protobuf.ByteString) ref;
      java.lang.String s = bs.toStringUtf8();
      privateKeyLocation_ = s;
      return s;
    }
  }
  /**
   * <pre>
   * Path to the private-key of the keypair
   * </pre>
   *
   * <code>string privateKeyLocation = 1;</code>
   * @return The bytes for privateKeyLocation.
   */
  @java.lang.Override
  public com.google.protobuf.ByteString
      getPrivateKeyLocationBytes() {
    java.lang.Object ref = privateKeyLocation_;
    if (ref instanceof java.lang.String) {
      com.google.protobuf.ByteString b = 
          com.google.protobuf.ByteString.copyFromUtf8(
              (java.lang.String) ref);
      privateKeyLocation_ = b;
      return b;
    } else {
      return (com.google.protobuf.ByteString) ref;
    }
  }

  public static final int PUBLICKEYLOCATION_FIELD_NUMBER = 2;
  private volatile java.lang.Object publicKeyLocation_;
  /**
   * <pre>
   * Path to the public-key of the keypair
   * </pre>
   *
   * <code>string publicKeyLocation = 2;</code>
   * @return The publicKeyLocation.
   */
  @java.lang.Override
  public java.lang.String getPublicKeyLocation() {
    java.lang.Object ref = publicKeyLocation_;
    if (ref instanceof java.lang.String) {
      return (java.lang.String) ref;
    } else {
      com.google.protobuf.ByteString bs = 
          (com.google.protobuf.ByteString) ref;
      java.lang.String s = bs.toStringUtf8();
      publicKeyLocation_ = s;
      return s;
    }
  }
  /**
   * <pre>
   * Path to the public-key of the keypair
   * </pre>
   *
   * <code>string publicKeyLocation = 2;</code>
   * @return The bytes for publicKeyLocation.
   */
  @java.lang.Override
  public com.google.protobuf.ByteString
      getPublicKeyLocationBytes() {
    java.lang.Object ref = publicKeyLocation_;
    if (ref instanceof java.lang.String) {
      com.google.protobuf.ByteString b = 
          com.google.protobuf.ByteString.copyFromUtf8(
              (java.lang.String) ref);
      publicKeyLocation_ = b;
      return b;
    } else {
      return (com.google.protobuf.ByteString) ref;
    }
  }

  private byte memoizedIsInitialized = -1;
  @java.lang.Override
  public final boolean isInitialized() {
    byte isInitialized = memoizedIsInitialized;
    if (isInitialized == 1) return true;
    if (isInitialized == 0) return false;

    memoizedIsInitialized = 1;
    return true;
  }

  @java.lang.Override
  public void writeTo(com.google.protobuf.CodedOutputStream output)
                      throws java.io.IOException {
    if (!getPrivateKeyLocationBytes().isEmpty()) {
      com.google.protobuf.GeneratedMessageV3.writeString(output, 1, privateKeyLocation_);
    }
    if (!getPublicKeyLocationBytes().isEmpty()) {
      com.google.protobuf.GeneratedMessageV3.writeString(output, 2, publicKeyLocation_);
    }
    unknownFields.writeTo(output);
  }

  @java.lang.Override
  public int getSerializedSize() {
    int size = memoizedSize;
    if (size != -1) return size;

    size = 0;
    if (!getPrivateKeyLocationBytes().isEmpty()) {
      size += com.google.protobuf.GeneratedMessageV3.computeStringSize(1, privateKeyLocation_);
    }
    if (!getPublicKeyLocationBytes().isEmpty()) {
      size += com.google.protobuf.GeneratedMessageV3.computeStringSize(2, publicKeyLocation_);
    }
    size += unknownFields.getSerializedSize();
    memoizedSize = size;
    return size;
  }

  @java.lang.Override
  public boolean equals(final java.lang.Object obj) {
    if (obj == this) {
     return true;
    }
    if (!(obj instanceof org.wso2.gateway.discovery.config.enforcer.Keystore)) {
      return super.equals(obj);
    }
    org.wso2.gateway.discovery.config.enforcer.Keystore other = (org.wso2.gateway.discovery.config.enforcer.Keystore) obj;

    if (!getPrivateKeyLocation()
        .equals(other.getPrivateKeyLocation())) return false;
    if (!getPublicKeyLocation()
        .equals(other.getPublicKeyLocation())) return false;
    if (!unknownFields.equals(other.unknownFields)) return false;
    return true;
  }

  @java.lang.Override
  public int hashCode() {
    if (memoizedHashCode != 0) {
      return memoizedHashCode;
    }
    int hash = 41;
    hash = (19 * hash) + getDescriptor().hashCode();
    hash = (37 * hash) + PRIVATEKEYLOCATION_FIELD_NUMBER;
    hash = (53 * hash) + getPrivateKeyLocation().hashCode();
    hash = (37 * hash) + PUBLICKEYLOCATION_FIELD_NUMBER;
    hash = (53 * hash) + getPublicKeyLocation().hashCode();
    hash = (29 * hash) + unknownFields.hashCode();
    memoizedHashCode = hash;
    return hash;
  }

  public static org.wso2.gateway.discovery.config.enforcer.Keystore parseFrom(
      java.nio.ByteBuffer data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static org.wso2.gateway.discovery.config.enforcer.Keystore parseFrom(
      java.nio.ByteBuffer data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static org.wso2.gateway.discovery.config.enforcer.Keystore parseFrom(
      com.google.protobuf.ByteString data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static org.wso2.gateway.discovery.config.enforcer.Keystore parseFrom(
      com.google.protobuf.ByteString data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static org.wso2.gateway.discovery.config.enforcer.Keystore parseFrom(byte[] data)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data);
  }
  public static org.wso2.gateway.discovery.config.enforcer.Keystore parseFrom(
      byte[] data,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws com.google.protobuf.InvalidProtocolBufferException {
    return PARSER.parseFrom(data, extensionRegistry);
  }
  public static org.wso2.gateway.discovery.config.enforcer.Keystore parseFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static org.wso2.gateway.discovery.config.enforcer.Keystore parseFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input, extensionRegistry);
  }
  public static org.wso2.gateway.discovery.config.enforcer.Keystore parseDelimitedFrom(java.io.InputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input);
  }
  public static org.wso2.gateway.discovery.config.enforcer.Keystore parseDelimitedFrom(
      java.io.InputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseDelimitedWithIOException(PARSER, input, extensionRegistry);
  }
  public static org.wso2.gateway.discovery.config.enforcer.Keystore parseFrom(
      com.google.protobuf.CodedInputStream input)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input);
  }
  public static org.wso2.gateway.discovery.config.enforcer.Keystore parseFrom(
      com.google.protobuf.CodedInputStream input,
      com.google.protobuf.ExtensionRegistryLite extensionRegistry)
      throws java.io.IOException {
    return com.google.protobuf.GeneratedMessageV3
        .parseWithIOException(PARSER, input, extensionRegistry);
  }

  @java.lang.Override
  public Builder newBuilderForType() { return newBuilder(); }
  public static Builder newBuilder() {
    return DEFAULT_INSTANCE.toBuilder();
  }
  public static Builder newBuilder(org.wso2.gateway.discovery.config.enforcer.Keystore prototype) {
    return DEFAULT_INSTANCE.toBuilder().mergeFrom(prototype);
  }
  @java.lang.Override
  public Builder toBuilder() {
    return this == DEFAULT_INSTANCE
        ? new Builder() : new Builder().mergeFrom(this);
  }

  @java.lang.Override
  protected Builder newBuilderForType(
      com.google.protobuf.GeneratedMessageV3.BuilderParent parent) {
    Builder builder = new Builder(parent);
    return builder;
  }
  /**
   * <pre>
   * Listener and client certificate store model
   * </pre>
   *
   * Protobuf type {@code wso2.discovery.config.enforcer.Keystore}
   */
  public static final class Builder extends
      com.google.protobuf.GeneratedMessageV3.Builder<Builder> implements
      // @@protoc_insertion_point(builder_implements:wso2.discovery.config.enforcer.Keystore)
      org.wso2.gateway.discovery.config.enforcer.KeystoreOrBuilder {
    public static final com.google.protobuf.Descriptors.Descriptor
        getDescriptor() {
      return org.wso2.gateway.discovery.config.enforcer.KeystoreProto.internal_static_wso2_discovery_config_enforcer_Keystore_descriptor;
    }

    @java.lang.Override
    protected com.google.protobuf.GeneratedMessageV3.FieldAccessorTable
        internalGetFieldAccessorTable() {
      return org.wso2.gateway.discovery.config.enforcer.KeystoreProto.internal_static_wso2_discovery_config_enforcer_Keystore_fieldAccessorTable
          .ensureFieldAccessorsInitialized(
              org.wso2.gateway.discovery.config.enforcer.Keystore.class, org.wso2.gateway.discovery.config.enforcer.Keystore.Builder.class);
    }

    // Construct using org.wso2.gateway.discovery.config.enforcer.Keystore.newBuilder()
    private Builder() {
      maybeForceBuilderInitialization();
    }

    private Builder(
        com.google.protobuf.GeneratedMessageV3.BuilderParent parent) {
      super(parent);
      maybeForceBuilderInitialization();
    }
    private void maybeForceBuilderInitialization() {
      if (com.google.protobuf.GeneratedMessageV3
              .alwaysUseFieldBuilders) {
      }
    }
    @java.lang.Override
    public Builder clear() {
      super.clear();
      privateKeyLocation_ = "";

      publicKeyLocation_ = "";

      return this;
    }

    @java.lang.Override
    public com.google.protobuf.Descriptors.Descriptor
        getDescriptorForType() {
      return org.wso2.gateway.discovery.config.enforcer.KeystoreProto.internal_static_wso2_discovery_config_enforcer_Keystore_descriptor;
    }

    @java.lang.Override
    public org.wso2.gateway.discovery.config.enforcer.Keystore getDefaultInstanceForType() {
      return org.wso2.gateway.discovery.config.enforcer.Keystore.getDefaultInstance();
    }

    @java.lang.Override
    public org.wso2.gateway.discovery.config.enforcer.Keystore build() {
      org.wso2.gateway.discovery.config.enforcer.Keystore result = buildPartial();
      if (!result.isInitialized()) {
        throw newUninitializedMessageException(result);
      }
      return result;
    }

    @java.lang.Override
    public org.wso2.gateway.discovery.config.enforcer.Keystore buildPartial() {
      org.wso2.gateway.discovery.config.enforcer.Keystore result = new org.wso2.gateway.discovery.config.enforcer.Keystore(this);
      result.privateKeyLocation_ = privateKeyLocation_;
      result.publicKeyLocation_ = publicKeyLocation_;
      onBuilt();
      return result;
    }

    @java.lang.Override
    public Builder clone() {
      return super.clone();
    }
    @java.lang.Override
    public Builder setField(
        com.google.protobuf.Descriptors.FieldDescriptor field,
        java.lang.Object value) {
      return super.setField(field, value);
    }
    @java.lang.Override
    public Builder clearField(
        com.google.protobuf.Descriptors.FieldDescriptor field) {
      return super.clearField(field);
    }
    @java.lang.Override
    public Builder clearOneof(
        com.google.protobuf.Descriptors.OneofDescriptor oneof) {
      return super.clearOneof(oneof);
    }
    @java.lang.Override
    public Builder setRepeatedField(
        com.google.protobuf.Descriptors.FieldDescriptor field,
        int index, java.lang.Object value) {
      return super.setRepeatedField(field, index, value);
    }
    @java.lang.Override
    public Builder addRepeatedField(
        com.google.protobuf.Descriptors.FieldDescriptor field,
        java.lang.Object value) {
      return super.addRepeatedField(field, value);
    }
    @java.lang.Override
    public Builder mergeFrom(com.google.protobuf.Message other) {
      if (other instanceof org.wso2.gateway.discovery.config.enforcer.Keystore) {
        return mergeFrom((org.wso2.gateway.discovery.config.enforcer.Keystore)other);
      } else {
        super.mergeFrom(other);
        return this;
      }
    }

    public Builder mergeFrom(org.wso2.gateway.discovery.config.enforcer.Keystore other) {
      if (other == org.wso2.gateway.discovery.config.enforcer.Keystore.getDefaultInstance()) return this;
      if (!other.getPrivateKeyLocation().isEmpty()) {
        privateKeyLocation_ = other.privateKeyLocation_;
        onChanged();
      }
      if (!other.getPublicKeyLocation().isEmpty()) {
        publicKeyLocation_ = other.publicKeyLocation_;
        onChanged();
      }
      this.mergeUnknownFields(other.unknownFields);
      onChanged();
      return this;
    }

    @java.lang.Override
    public final boolean isInitialized() {
      return true;
    }

    @java.lang.Override
    public Builder mergeFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws java.io.IOException {
      org.wso2.gateway.discovery.config.enforcer.Keystore parsedMessage = null;
      try {
        parsedMessage = PARSER.parsePartialFrom(input, extensionRegistry);
      } catch (com.google.protobuf.InvalidProtocolBufferException e) {
        parsedMessage = (org.wso2.gateway.discovery.config.enforcer.Keystore) e.getUnfinishedMessage();
        throw e.unwrapIOException();
      } finally {
        if (parsedMessage != null) {
          mergeFrom(parsedMessage);
        }
      }
      return this;
    }

    private java.lang.Object privateKeyLocation_ = "";
    /**
     * <pre>
     * Path to the private-key of the keypair
     * </pre>
     *
     * <code>string privateKeyLocation = 1;</code>
     * @return The privateKeyLocation.
     */
    public java.lang.String getPrivateKeyLocation() {
      java.lang.Object ref = privateKeyLocation_;
      if (!(ref instanceof java.lang.String)) {
        com.google.protobuf.ByteString bs =
            (com.google.protobuf.ByteString) ref;
        java.lang.String s = bs.toStringUtf8();
        privateKeyLocation_ = s;
        return s;
      } else {
        return (java.lang.String) ref;
      }
    }
    /**
     * <pre>
     * Path to the private-key of the keypair
     * </pre>
     *
     * <code>string privateKeyLocation = 1;</code>
     * @return The bytes for privateKeyLocation.
     */
    public com.google.protobuf.ByteString
        getPrivateKeyLocationBytes() {
      java.lang.Object ref = privateKeyLocation_;
      if (ref instanceof String) {
        com.google.protobuf.ByteString b = 
            com.google.protobuf.ByteString.copyFromUtf8(
                (java.lang.String) ref);
        privateKeyLocation_ = b;
        return b;
      } else {
        return (com.google.protobuf.ByteString) ref;
      }
    }
    /**
     * <pre>
     * Path to the private-key of the keypair
     * </pre>
     *
     * <code>string privateKeyLocation = 1;</code>
     * @param value The privateKeyLocation to set.
     * @return This builder for chaining.
     */
    public Builder setPrivateKeyLocation(
        java.lang.String value) {
      if (value == null) {
    throw new NullPointerException();
  }
  
      privateKeyLocation_ = value;
      onChanged();
      return this;
    }
    /**
     * <pre>
     * Path to the private-key of the keypair
     * </pre>
     *
     * <code>string privateKeyLocation = 1;</code>
     * @return This builder for chaining.
     */
    public Builder clearPrivateKeyLocation() {
      
      privateKeyLocation_ = getDefaultInstance().getPrivateKeyLocation();
      onChanged();
      return this;
    }
    /**
     * <pre>
     * Path to the private-key of the keypair
     * </pre>
     *
     * <code>string privateKeyLocation = 1;</code>
     * @param value The bytes for privateKeyLocation to set.
     * @return This builder for chaining.
     */
    public Builder setPrivateKeyLocationBytes(
        com.google.protobuf.ByteString value) {
      if (value == null) {
    throw new NullPointerException();
  }
  checkByteStringIsUtf8(value);
      
      privateKeyLocation_ = value;
      onChanged();
      return this;
    }

    private java.lang.Object publicKeyLocation_ = "";
    /**
     * <pre>
     * Path to the public-key of the keypair
     * </pre>
     *
     * <code>string publicKeyLocation = 2;</code>
     * @return The publicKeyLocation.
     */
    public java.lang.String getPublicKeyLocation() {
      java.lang.Object ref = publicKeyLocation_;
      if (!(ref instanceof java.lang.String)) {
        com.google.protobuf.ByteString bs =
            (com.google.protobuf.ByteString) ref;
        java.lang.String s = bs.toStringUtf8();
        publicKeyLocation_ = s;
        return s;
      } else {
        return (java.lang.String) ref;
      }
    }
    /**
     * <pre>
     * Path to the public-key of the keypair
     * </pre>
     *
     * <code>string publicKeyLocation = 2;</code>
     * @return The bytes for publicKeyLocation.
     */
    public com.google.protobuf.ByteString
        getPublicKeyLocationBytes() {
      java.lang.Object ref = publicKeyLocation_;
      if (ref instanceof String) {
        com.google.protobuf.ByteString b = 
            com.google.protobuf.ByteString.copyFromUtf8(
                (java.lang.String) ref);
        publicKeyLocation_ = b;
        return b;
      } else {
        return (com.google.protobuf.ByteString) ref;
      }
    }
    /**
     * <pre>
     * Path to the public-key of the keypair
     * </pre>
     *
     * <code>string publicKeyLocation = 2;</code>
     * @param value The publicKeyLocation to set.
     * @return This builder for chaining.
     */
    public Builder setPublicKeyLocation(
        java.lang.String value) {
      if (value == null) {
    throw new NullPointerException();
  }
  
      publicKeyLocation_ = value;
      onChanged();
      return this;
    }
    /**
     * <pre>
     * Path to the public-key of the keypair
     * </pre>
     *
     * <code>string publicKeyLocation = 2;</code>
     * @return This builder for chaining.
     */
    public Builder clearPublicKeyLocation() {
      
      publicKeyLocation_ = getDefaultInstance().getPublicKeyLocation();
      onChanged();
      return this;
    }
    /**
     * <pre>
     * Path to the public-key of the keypair
     * </pre>
     *
     * <code>string publicKeyLocation = 2;</code>
     * @param value The bytes for publicKeyLocation to set.
     * @return This builder for chaining.
     */
    public Builder setPublicKeyLocationBytes(
        com.google.protobuf.ByteString value) {
      if (value == null) {
    throw new NullPointerException();
  }
  checkByteStringIsUtf8(value);
      
      publicKeyLocation_ = value;
      onChanged();
      return this;
    }
    @java.lang.Override
    public final Builder setUnknownFields(
        final com.google.protobuf.UnknownFieldSet unknownFields) {
      return super.setUnknownFields(unknownFields);
    }

    @java.lang.Override
    public final Builder mergeUnknownFields(
        final com.google.protobuf.UnknownFieldSet unknownFields) {
      return super.mergeUnknownFields(unknownFields);
    }


    // @@protoc_insertion_point(builder_scope:wso2.discovery.config.enforcer.Keystore)
  }

  // @@protoc_insertion_point(class_scope:wso2.discovery.config.enforcer.Keystore)
  private static final org.wso2.gateway.discovery.config.enforcer.Keystore DEFAULT_INSTANCE;
  static {
    DEFAULT_INSTANCE = new org.wso2.gateway.discovery.config.enforcer.Keystore();
  }

  public static org.wso2.gateway.discovery.config.enforcer.Keystore getDefaultInstance() {
    return DEFAULT_INSTANCE;
  }

  private static final com.google.protobuf.Parser<Keystore>
      PARSER = new com.google.protobuf.AbstractParser<Keystore>() {
    @java.lang.Override
    public Keystore parsePartialFrom(
        com.google.protobuf.CodedInputStream input,
        com.google.protobuf.ExtensionRegistryLite extensionRegistry)
        throws com.google.protobuf.InvalidProtocolBufferException {
      return new Keystore(input, extensionRegistry);
    }
  };

  public static com.google.protobuf.Parser<Keystore> parser() {
    return PARSER;
  }

  @java.lang.Override
  public com.google.protobuf.Parser<Keystore> getParserForType() {
    return PARSER;
  }

  @java.lang.Override
  public org.wso2.gateway.discovery.config.enforcer.Keystore getDefaultInstanceForType() {
    return DEFAULT_INSTANCE;
  }

}
