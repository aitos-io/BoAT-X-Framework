/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: peer/query.proto */

#ifndef PROTOBUF_C_peer_2fquery_2eproto__INCLUDED
#define PROTOBUF_C_peer_2fquery_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1000000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1002001 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _Protos__ChaincodeQueryResponse Protos__ChaincodeQueryResponse;
typedef struct _Protos__ChaincodeInfo Protos__ChaincodeInfo;
typedef struct _Protos__ChannelQueryResponse Protos__ChannelQueryResponse;
typedef struct _Protos__ChannelInfo Protos__ChannelInfo;


/* --- enums --- */


/* --- messages --- */

/*
 * ChaincodeQueryResponse returns information about each chaincode that pertains
 * to a query in lscc.go, such as GetChaincodes (returns all chaincodes
 * instantiated on a channel), and GetInstalledChaincodes (returns all chaincodes
 * installed on a peer)
 */
struct  _Protos__ChaincodeQueryResponse
{
  ProtobufCMessage base;
  size_t n_chaincodes;
  Protos__ChaincodeInfo **chaincodes;
};
#define PROTOS__CHAINCODE_QUERY_RESPONSE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&protos__chaincode_query_response__descriptor) \
    , 0,NULL }


/*
 * ChaincodeInfo contains general information about an installed/instantiated
 * chaincode
 */
struct  _Protos__ChaincodeInfo
{
  ProtobufCMessage base;
  char *name;
  char *version;
  /*
   * the path as specified by the install/instantiate transaction
   */
  char *path;
  /*
   * the chaincode function upon instantiation and its arguments. This will be
   * blank if the query is returning information about installed chaincodes.
   */
  char *input;
  /*
   * the name of the ESCC for this chaincode. This will be
   * blank if the query is returning information about installed chaincodes.
   */
  char *escc;
  /*
   * the name of the VSCC for this chaincode. This will be
   * blank if the query is returning information about installed chaincodes.
   */
  char *vscc;
  /*
   * the chaincode unique id.
   * computed as: H(
   *                H(name || version) ||
   *                H(CodePackage)
   *              )
   */
  protobuf_c_boolean has_id;
  ProtobufCBinaryData id;
};
#define PROTOS__CHAINCODE_INFO__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&protos__chaincode_info__descriptor) \
    , NULL, NULL, NULL, NULL, NULL, NULL, 0,{0,NULL} }


/*
 * ChannelQueryResponse returns information about each channel that pertains
 * to a query in lscc.go, such as GetChannels (returns all channels for a
 * given peer)
 */
struct  _Protos__ChannelQueryResponse
{
  ProtobufCMessage base;
  size_t n_channels;
  Protos__ChannelInfo **channels;
};
#define PROTOS__CHANNEL_QUERY_RESPONSE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&protos__channel_query_response__descriptor) \
    , 0,NULL }


/*
 * ChannelInfo contains general information about channels
 */
struct  _Protos__ChannelInfo
{
  ProtobufCMessage base;
  char *channel_id;
};
#define PROTOS__CHANNEL_INFO__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&protos__channel_info__descriptor) \
    , NULL }


/* Protos__ChaincodeQueryResponse methods */
void   protos__chaincode_query_response__init
                     (Protos__ChaincodeQueryResponse         *message);
size_t protos__chaincode_query_response__get_packed_size
                     (const Protos__ChaincodeQueryResponse   *message);
size_t protos__chaincode_query_response__pack
                     (const Protos__ChaincodeQueryResponse   *message,
                      uint8_t             *out);
size_t protos__chaincode_query_response__pack_to_buffer
                     (const Protos__ChaincodeQueryResponse   *message,
                      ProtobufCBuffer     *buffer);
Protos__ChaincodeQueryResponse *
       protos__chaincode_query_response__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   protos__chaincode_query_response__free_unpacked
                     (Protos__ChaincodeQueryResponse *message,
                      ProtobufCAllocator *allocator);
/* Protos__ChaincodeInfo methods */
void   protos__chaincode_info__init
                     (Protos__ChaincodeInfo         *message);
size_t protos__chaincode_info__get_packed_size
                     (const Protos__ChaincodeInfo   *message);
size_t protos__chaincode_info__pack
                     (const Protos__ChaincodeInfo   *message,
                      uint8_t             *out);
size_t protos__chaincode_info__pack_to_buffer
                     (const Protos__ChaincodeInfo   *message,
                      ProtobufCBuffer     *buffer);
Protos__ChaincodeInfo *
       protos__chaincode_info__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   protos__chaincode_info__free_unpacked
                     (Protos__ChaincodeInfo *message,
                      ProtobufCAllocator *allocator);
/* Protos__ChannelQueryResponse methods */
void   protos__channel_query_response__init
                     (Protos__ChannelQueryResponse         *message);
size_t protos__channel_query_response__get_packed_size
                     (const Protos__ChannelQueryResponse   *message);
size_t protos__channel_query_response__pack
                     (const Protos__ChannelQueryResponse   *message,
                      uint8_t             *out);
size_t protos__channel_query_response__pack_to_buffer
                     (const Protos__ChannelQueryResponse   *message,
                      ProtobufCBuffer     *buffer);
Protos__ChannelQueryResponse *
       protos__channel_query_response__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   protos__channel_query_response__free_unpacked
                     (Protos__ChannelQueryResponse *message,
                      ProtobufCAllocator *allocator);
/* Protos__ChannelInfo methods */
void   protos__channel_info__init
                     (Protos__ChannelInfo         *message);
size_t protos__channel_info__get_packed_size
                     (const Protos__ChannelInfo   *message);
size_t protos__channel_info__pack
                     (const Protos__ChannelInfo   *message,
                      uint8_t             *out);
size_t protos__channel_info__pack_to_buffer
                     (const Protos__ChannelInfo   *message,
                      ProtobufCBuffer     *buffer);
Protos__ChannelInfo *
       protos__channel_info__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   protos__channel_info__free_unpacked
                     (Protos__ChannelInfo *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*Protos__ChaincodeQueryResponse_Closure)
                 (const Protos__ChaincodeQueryResponse *message,
                  void *closure_data);
typedef void (*Protos__ChaincodeInfo_Closure)
                 (const Protos__ChaincodeInfo *message,
                  void *closure_data);
typedef void (*Protos__ChannelQueryResponse_Closure)
                 (const Protos__ChannelQueryResponse *message,
                  void *closure_data);
typedef void (*Protos__ChannelInfo_Closure)
                 (const Protos__ChannelInfo *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor protos__chaincode_query_response__descriptor;
extern const ProtobufCMessageDescriptor protos__chaincode_info__descriptor;
extern const ProtobufCMessageDescriptor protos__channel_query_response__descriptor;
extern const ProtobufCMessageDescriptor protos__channel_info__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_peer_2fquery_2eproto__INCLUDED */
