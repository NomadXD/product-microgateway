// Code generated by protoc-gen-go. DO NOT EDIT.
// versions:
// 	protoc-gen-go v1.25.0-devel
// 	protoc        v3.13.0
// source: wso2/discovery/service/throttle/tdds.proto

package throttle

import (
	context "context"
	v3 "github.com/envoyproxy/go-control-plane/envoy/service/discovery/v3"
	grpc "google.golang.org/grpc"
	codes "google.golang.org/grpc/codes"
	status "google.golang.org/grpc/status"
	protoreflect "google.golang.org/protobuf/reflect/protoreflect"
	protoimpl "google.golang.org/protobuf/runtime/protoimpl"
	reflect "reflect"
)

const (
	// Verify that this generated code is sufficiently up-to-date.
	_ = protoimpl.EnforceVersion(20 - protoimpl.MinVersion)
	// Verify that runtime/protoimpl is sufficiently up-to-date.
	_ = protoimpl.EnforceVersion(protoimpl.MaxVersion - 20)
)

var File_wso2_discovery_service_throttle_tdds_proto protoreflect.FileDescriptor

var file_wso2_discovery_service_throttle_tdds_proto_rawDesc = []byte{
	0x0a, 0x2a, 0x77, 0x73, 0x6f, 0x32, 0x2f, 0x64, 0x69, 0x73, 0x63, 0x6f, 0x76, 0x65, 0x72, 0x79,
	0x2f, 0x73, 0x65, 0x72, 0x76, 0x69, 0x63, 0x65, 0x2f, 0x74, 0x68, 0x72, 0x6f, 0x74, 0x74, 0x6c,
	0x65, 0x2f, 0x74, 0x64, 0x64, 0x73, 0x2e, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x12, 0x1a, 0x64, 0x69,
	0x73, 0x63, 0x6f, 0x76, 0x65, 0x72, 0x79, 0x2e, 0x73, 0x65, 0x72, 0x76, 0x69, 0x63, 0x65, 0x2e,
	0x74, 0x68, 0x72, 0x6f, 0x74, 0x74, 0x6c, 0x65, 0x1a, 0x2a, 0x65, 0x6e, 0x76, 0x6f, 0x79, 0x2f,
	0x73, 0x65, 0x72, 0x76, 0x69, 0x63, 0x65, 0x2f, 0x64, 0x69, 0x73, 0x63, 0x6f, 0x76, 0x65, 0x72,
	0x79, 0x2f, 0x76, 0x33, 0x2f, 0x64, 0x69, 0x73, 0x63, 0x6f, 0x76, 0x65, 0x72, 0x79, 0x2e, 0x70,
	0x72, 0x6f, 0x74, 0x6f, 0x32, 0x8b, 0x02, 0x0a, 0x1c, 0x54, 0x68, 0x72, 0x6f, 0x74, 0x74, 0x6c,
	0x65, 0x44, 0x61, 0x74, 0x61, 0x44, 0x69, 0x73, 0x63, 0x6f, 0x76, 0x65, 0x72, 0x79, 0x53, 0x65,
	0x72, 0x76, 0x69, 0x63, 0x65, 0x12, 0x77, 0x0a, 0x12, 0x53, 0x74, 0x72, 0x65, 0x61, 0x6d, 0x54,
	0x68, 0x72, 0x6f, 0x74, 0x74, 0x6c, 0x65, 0x44, 0x61, 0x74, 0x61, 0x12, 0x2c, 0x2e, 0x65, 0x6e,
	0x76, 0x6f, 0x79, 0x2e, 0x73, 0x65, 0x72, 0x76, 0x69, 0x63, 0x65, 0x2e, 0x64, 0x69, 0x73, 0x63,
	0x6f, 0x76, 0x65, 0x72, 0x79, 0x2e, 0x76, 0x33, 0x2e, 0x44, 0x69, 0x73, 0x63, 0x6f, 0x76, 0x65,
	0x72, 0x79, 0x52, 0x65, 0x71, 0x75, 0x65, 0x73, 0x74, 0x1a, 0x2d, 0x2e, 0x65, 0x6e, 0x76, 0x6f,
	0x79, 0x2e, 0x73, 0x65, 0x72, 0x76, 0x69, 0x63, 0x65, 0x2e, 0x64, 0x69, 0x73, 0x63, 0x6f, 0x76,
	0x65, 0x72, 0x79, 0x2e, 0x76, 0x33, 0x2e, 0x44, 0x69, 0x73, 0x63, 0x6f, 0x76, 0x65, 0x72, 0x79,
	0x52, 0x65, 0x73, 0x70, 0x6f, 0x6e, 0x73, 0x65, 0x22, 0x00, 0x28, 0x01, 0x30, 0x01, 0x12, 0x72,
	0x0a, 0x11, 0x46, 0x65, 0x74, 0x63, 0x68, 0x54, 0x68, 0x72, 0x6f, 0x74, 0x74, 0x6c, 0x65, 0x44,
	0x61, 0x74, 0x61, 0x12, 0x2c, 0x2e, 0x65, 0x6e, 0x76, 0x6f, 0x79, 0x2e, 0x73, 0x65, 0x72, 0x76,
	0x69, 0x63, 0x65, 0x2e, 0x64, 0x69, 0x73, 0x63, 0x6f, 0x76, 0x65, 0x72, 0x79, 0x2e, 0x76, 0x33,
	0x2e, 0x44, 0x69, 0x73, 0x63, 0x6f, 0x76, 0x65, 0x72, 0x79, 0x52, 0x65, 0x71, 0x75, 0x65, 0x73,
	0x74, 0x1a, 0x2d, 0x2e, 0x65, 0x6e, 0x76, 0x6f, 0x79, 0x2e, 0x73, 0x65, 0x72, 0x76, 0x69, 0x63,
	0x65, 0x2e, 0x64, 0x69, 0x73, 0x63, 0x6f, 0x76, 0x65, 0x72, 0x79, 0x2e, 0x76, 0x33, 0x2e, 0x44,
	0x69, 0x73, 0x63, 0x6f, 0x76, 0x65, 0x72, 0x79, 0x52, 0x65, 0x73, 0x70, 0x6f, 0x6e, 0x73, 0x65,
	0x22, 0x00, 0x42, 0x8c, 0x01, 0x0a, 0x32, 0x6f, 0x72, 0x67, 0x2e, 0x77, 0x73, 0x6f, 0x32, 0x2e,
	0x63, 0x68, 0x6f, 0x72, 0x65, 0x6f, 0x2e, 0x63, 0x6f, 0x6e, 0x6e, 0x65, 0x63, 0x74, 0x2e, 0x64,
	0x69, 0x73, 0x63, 0x6f, 0x76, 0x65, 0x72, 0x79, 0x2e, 0x73, 0x65, 0x72, 0x76, 0x69, 0x63, 0x65,
	0x2e, 0x74, 0x68, 0x72, 0x6f, 0x74, 0x74, 0x6c, 0x65, 0x42, 0x09, 0x54, 0x44, 0x64, 0x73, 0x50,
	0x72, 0x6f, 0x74, 0x6f, 0x50, 0x01, 0x5a, 0x46, 0x67, 0x69, 0x74, 0x68, 0x75, 0x62, 0x2e, 0x63,
	0x6f, 0x6d, 0x2f, 0x65, 0x6e, 0x76, 0x6f, 0x79, 0x70, 0x72, 0x6f, 0x78, 0x79, 0x2f, 0x67, 0x6f,
	0x2d, 0x63, 0x6f, 0x6e, 0x74, 0x72, 0x6f, 0x6c, 0x2d, 0x70, 0x6c, 0x61, 0x6e, 0x65, 0x2f, 0x77,
	0x73, 0x6f, 0x32, 0x2f, 0x64, 0x69, 0x73, 0x63, 0x6f, 0x76, 0x65, 0x72, 0x79, 0x2f, 0x73, 0x65,
	0x72, 0x76, 0x69, 0x63, 0x65, 0x2f, 0x74, 0x68, 0x72, 0x6f, 0x74, 0x74, 0x6c, 0x65, 0x88, 0x01,
	0x01, 0x62, 0x06, 0x70, 0x72, 0x6f, 0x74, 0x6f, 0x33,
}

var file_wso2_discovery_service_throttle_tdds_proto_goTypes = []interface{}{
	(*v3.DiscoveryRequest)(nil),  // 0: envoy.service.discovery.v3.DiscoveryRequest
	(*v3.DiscoveryResponse)(nil), // 1: envoy.service.discovery.v3.DiscoveryResponse
}
var file_wso2_discovery_service_throttle_tdds_proto_depIdxs = []int32{
	0, // 0: discovery.service.throttle.ThrottleDataDiscoveryService.StreamThrottleData:input_type -> envoy.service.discovery.v3.DiscoveryRequest
	0, // 1: discovery.service.throttle.ThrottleDataDiscoveryService.FetchThrottleData:input_type -> envoy.service.discovery.v3.DiscoveryRequest
	1, // 2: discovery.service.throttle.ThrottleDataDiscoveryService.StreamThrottleData:output_type -> envoy.service.discovery.v3.DiscoveryResponse
	1, // 3: discovery.service.throttle.ThrottleDataDiscoveryService.FetchThrottleData:output_type -> envoy.service.discovery.v3.DiscoveryResponse
	2, // [2:4] is the sub-list for method output_type
	0, // [0:2] is the sub-list for method input_type
	0, // [0:0] is the sub-list for extension type_name
	0, // [0:0] is the sub-list for extension extendee
	0, // [0:0] is the sub-list for field type_name
}

func init() { file_wso2_discovery_service_throttle_tdds_proto_init() }
func file_wso2_discovery_service_throttle_tdds_proto_init() {
	if File_wso2_discovery_service_throttle_tdds_proto != nil {
		return
	}
	type x struct{}
	out := protoimpl.TypeBuilder{
		File: protoimpl.DescBuilder{
			GoPackagePath: reflect.TypeOf(x{}).PkgPath(),
			RawDescriptor: file_wso2_discovery_service_throttle_tdds_proto_rawDesc,
			NumEnums:      0,
			NumMessages:   0,
			NumExtensions: 0,
			NumServices:   1,
		},
		GoTypes:           file_wso2_discovery_service_throttle_tdds_proto_goTypes,
		DependencyIndexes: file_wso2_discovery_service_throttle_tdds_proto_depIdxs,
	}.Build()
	File_wso2_discovery_service_throttle_tdds_proto = out.File
	file_wso2_discovery_service_throttle_tdds_proto_rawDesc = nil
	file_wso2_discovery_service_throttle_tdds_proto_goTypes = nil
	file_wso2_discovery_service_throttle_tdds_proto_depIdxs = nil
}

// Reference imports to suppress errors if they are not otherwise used.
var _ context.Context
var _ grpc.ClientConnInterface

// This is a compile-time assertion to ensure that this generated file
// is compatible with the grpc package it is being compiled against.
const _ = grpc.SupportPackageIsVersion6

// ThrottleDataDiscoveryServiceClient is the client API for ThrottleDataDiscoveryService service.
//
// For semantics around ctx use and closing/ending streaming RPCs, please refer to https://godoc.org/google.golang.org/grpc#ClientConn.NewStream.
type ThrottleDataDiscoveryServiceClient interface {
	StreamThrottleData(ctx context.Context, opts ...grpc.CallOption) (ThrottleDataDiscoveryService_StreamThrottleDataClient, error)
	FetchThrottleData(ctx context.Context, in *v3.DiscoveryRequest, opts ...grpc.CallOption) (*v3.DiscoveryResponse, error)
}

type throttleDataDiscoveryServiceClient struct {
	cc grpc.ClientConnInterface
}

func NewThrottleDataDiscoveryServiceClient(cc grpc.ClientConnInterface) ThrottleDataDiscoveryServiceClient {
	return &throttleDataDiscoveryServiceClient{cc}
}

func (c *throttleDataDiscoveryServiceClient) StreamThrottleData(ctx context.Context, opts ...grpc.CallOption) (ThrottleDataDiscoveryService_StreamThrottleDataClient, error) {
	stream, err := c.cc.NewStream(ctx, &_ThrottleDataDiscoveryService_serviceDesc.Streams[0], "/discovery.service.throttle.ThrottleDataDiscoveryService/StreamThrottleData", opts...)
	if err != nil {
		return nil, err
	}
	x := &throttleDataDiscoveryServiceStreamThrottleDataClient{stream}
	return x, nil
}

type ThrottleDataDiscoveryService_StreamThrottleDataClient interface {
	Send(*v3.DiscoveryRequest) error
	Recv() (*v3.DiscoveryResponse, error)
	grpc.ClientStream
}

type throttleDataDiscoveryServiceStreamThrottleDataClient struct {
	grpc.ClientStream
}

func (x *throttleDataDiscoveryServiceStreamThrottleDataClient) Send(m *v3.DiscoveryRequest) error {
	return x.ClientStream.SendMsg(m)
}

func (x *throttleDataDiscoveryServiceStreamThrottleDataClient) Recv() (*v3.DiscoveryResponse, error) {
	m := new(v3.DiscoveryResponse)
	if err := x.ClientStream.RecvMsg(m); err != nil {
		return nil, err
	}
	return m, nil
}

func (c *throttleDataDiscoveryServiceClient) FetchThrottleData(ctx context.Context, in *v3.DiscoveryRequest, opts ...grpc.CallOption) (*v3.DiscoveryResponse, error) {
	out := new(v3.DiscoveryResponse)
	err := c.cc.Invoke(ctx, "/discovery.service.throttle.ThrottleDataDiscoveryService/FetchThrottleData", in, out, opts...)
	if err != nil {
		return nil, err
	}
	return out, nil
}

// ThrottleDataDiscoveryServiceServer is the server API for ThrottleDataDiscoveryService service.
type ThrottleDataDiscoveryServiceServer interface {
	StreamThrottleData(ThrottleDataDiscoveryService_StreamThrottleDataServer) error
	FetchThrottleData(context.Context, *v3.DiscoveryRequest) (*v3.DiscoveryResponse, error)
}

// UnimplementedThrottleDataDiscoveryServiceServer can be embedded to have forward compatible implementations.
type UnimplementedThrottleDataDiscoveryServiceServer struct {
}

func (*UnimplementedThrottleDataDiscoveryServiceServer) StreamThrottleData(ThrottleDataDiscoveryService_StreamThrottleDataServer) error {
	return status.Errorf(codes.Unimplemented, "method StreamThrottleData not implemented")
}
func (*UnimplementedThrottleDataDiscoveryServiceServer) FetchThrottleData(context.Context, *v3.DiscoveryRequest) (*v3.DiscoveryResponse, error) {
	return nil, status.Errorf(codes.Unimplemented, "method FetchThrottleData not implemented")
}

func RegisterThrottleDataDiscoveryServiceServer(s *grpc.Server, srv ThrottleDataDiscoveryServiceServer) {
	s.RegisterService(&_ThrottleDataDiscoveryService_serviceDesc, srv)
}

func _ThrottleDataDiscoveryService_StreamThrottleData_Handler(srv interface{}, stream grpc.ServerStream) error {
	return srv.(ThrottleDataDiscoveryServiceServer).StreamThrottleData(&throttleDataDiscoveryServiceStreamThrottleDataServer{stream})
}

type ThrottleDataDiscoveryService_StreamThrottleDataServer interface {
	Send(*v3.DiscoveryResponse) error
	Recv() (*v3.DiscoveryRequest, error)
	grpc.ServerStream
}

type throttleDataDiscoveryServiceStreamThrottleDataServer struct {
	grpc.ServerStream
}

func (x *throttleDataDiscoveryServiceStreamThrottleDataServer) Send(m *v3.DiscoveryResponse) error {
	return x.ServerStream.SendMsg(m)
}

func (x *throttleDataDiscoveryServiceStreamThrottleDataServer) Recv() (*v3.DiscoveryRequest, error) {
	m := new(v3.DiscoveryRequest)
	if err := x.ServerStream.RecvMsg(m); err != nil {
		return nil, err
	}
	return m, nil
}

func _ThrottleDataDiscoveryService_FetchThrottleData_Handler(srv interface{}, ctx context.Context, dec func(interface{}) error, interceptor grpc.UnaryServerInterceptor) (interface{}, error) {
	in := new(v3.DiscoveryRequest)
	if err := dec(in); err != nil {
		return nil, err
	}
	if interceptor == nil {
		return srv.(ThrottleDataDiscoveryServiceServer).FetchThrottleData(ctx, in)
	}
	info := &grpc.UnaryServerInfo{
		Server:     srv,
		FullMethod: "/discovery.service.throttle.ThrottleDataDiscoveryService/FetchThrottleData",
	}
	handler := func(ctx context.Context, req interface{}) (interface{}, error) {
		return srv.(ThrottleDataDiscoveryServiceServer).FetchThrottleData(ctx, req.(*v3.DiscoveryRequest))
	}
	return interceptor(ctx, in, info, handler)
}

var _ThrottleDataDiscoveryService_serviceDesc = grpc.ServiceDesc{
	ServiceName: "discovery.service.throttle.ThrottleDataDiscoveryService",
	HandlerType: (*ThrottleDataDiscoveryServiceServer)(nil),
	Methods: []grpc.MethodDesc{
		{
			MethodName: "FetchThrottleData",
			Handler:    _ThrottleDataDiscoveryService_FetchThrottleData_Handler,
		},
	},
	Streams: []grpc.StreamDesc{
		{
			StreamName:    "StreamThrottleData",
			Handler:       _ThrottleDataDiscoveryService_StreamThrottleData_Handler,
			ServerStreams: true,
			ClientStreams: true,
		},
	},
	Metadata: "wso2/discovery/service/throttle/tdds.proto",
}
