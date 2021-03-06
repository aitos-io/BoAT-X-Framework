/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: orderer/cluster.proto */

#ifndef PROTOBUF_C_orderer_2fcluster_2eproto__INCLUDED
#define PROTOBUF_C_orderer_2fcluster_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1000000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1002001 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif

#include "common/common.pb-c.h"

typedef struct _Orderer__StepRequest Orderer__StepRequest;
typedef struct _Orderer__StepResponse Orderer__StepResponse;
typedef struct _Orderer__SubmitRequest Orderer__SubmitRequest;
typedef struct _Orderer__SubmitResponse Orderer__SubmitResponse;


/* --- enums --- */


/* --- messages --- */

/*
 * StepRequest wraps a consensus implementation
 * specific message that is sent to a cluster member
 */
struct  _Orderer__StepRequest
{
  ProtobufCMessage base;
  char *channel;
  protobuf_c_boolean has_payload;
  ProtobufCBinaryData payload;
};
#define ORDERER__STEP_REQUEST__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&orderer__step_request__descriptor) \
    , NULL, 0,{0,NULL} }


/*
 * StepResponse wraps a consensus implementation
 * specific message that is received from
 * a cluster member as a response to a StepRequest
 */
struct  _Orderer__StepResponse
{
  ProtobufCMessage base;
  protobuf_c_boolean has_payload;
  ProtobufCBinaryData payload;
};
#define ORDERER__STEP_RESPONSE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&orderer__step_response__descriptor) \
    , 0,{0,NULL} }


/*
 * SubmitRequest wraps a transaction to be sent for ordering
 */
struct  _Orderer__SubmitRequest
{
  ProtobufCMessage base;
  char *channel;
  /*
   * last_validation_seq denotes the last
   * configuration sequence at which the
   * sender validated this message
   */
  protobuf_c_boolean has_last_validation_seq;
  uint64_t last_validation_seq;
  /*
   * content is the fabric transaction
   * that is forwarded to the cluster member
   */
  Common__Envelope *content;
};
#define ORDERER__SUBMIT_REQUEST__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&orderer__submit_request__descriptor) \
    , NULL, 0,0, NULL }


/*
 * SubmitResponse returns a success
 * or failure status to the sender
 */
struct  _Orderer__SubmitResponse
{
  ProtobufCMessage base;
  /*
   * Status code, which may be used to programatically respond to success/failure
   */
  protobuf_c_boolean has_status;
  Common__Status status;
  /*
   * Info string which may contain additional information about the status returned
   */
  char *info;
};
#define ORDERER__SUBMIT_RESPONSE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&orderer__submit_response__descriptor) \
    , 0,0, NULL }


/* Orderer__StepRequest methods */
void   orderer__step_request__init
                     (Orderer__StepRequest         *message);
size_t orderer__step_request__get_packed_size
                     (const Orderer__StepRequest   *message);
size_t orderer__step_request__pack
                     (const Orderer__StepRequest   *message,
                      uint8_t             *out);
size_t orderer__step_request__pack_to_buffer
                     (const Orderer__StepRequest   *message,
                      ProtobufCBuffer     *buffer);
Orderer__StepRequest *
       orderer__step_request__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   orderer__step_request__free_unpacked
                     (Orderer__StepRequest *message,
                      ProtobufCAllocator *allocator);
/* Orderer__StepResponse methods */
void   orderer__step_response__init
                     (Orderer__StepResponse         *message);
size_t orderer__step_response__get_packed_size
                     (const Orderer__StepResponse   *message);
size_t orderer__step_response__pack
                     (const Orderer__StepResponse   *message,
                      uint8_t             *out);
size_t orderer__step_response__pack_to_buffer
                     (const Orderer__StepResponse   *message,
                      ProtobufCBuffer     *buffer);
Orderer__StepResponse *
       orderer__step_response__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   orderer__step_response__free_unpacked
                     (Orderer__StepResponse *message,
                      ProtobufCAllocator *allocator);
/* Orderer__SubmitRequest methods */
void   orderer__submit_request__init
                     (Orderer__SubmitRequest         *message);
size_t orderer__submit_request__get_packed_size
                     (const Orderer__SubmitRequest   *message);
size_t orderer__submit_request__pack
                     (const Orderer__SubmitRequest   *message,
                      uint8_t             *out);
size_t orderer__submit_request__pack_to_buffer
                     (const Orderer__SubmitRequest   *message,
                      ProtobufCBuffer     *buffer);
Orderer__SubmitRequest *
       orderer__submit_request__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   orderer__submit_request__free_unpacked
                     (Orderer__SubmitRequest *message,
                      ProtobufCAllocator *allocator);
/* Orderer__SubmitResponse methods */
void   orderer__submit_response__init
                     (Orderer__SubmitResponse         *message);
size_t orderer__submit_response__get_packed_size
                     (const Orderer__SubmitResponse   *message);
size_t orderer__submit_response__pack
                     (const Orderer__SubmitResponse   *message,
                      uint8_t             *out);
size_t orderer__submit_response__pack_to_buffer
                     (const Orderer__SubmitResponse   *message,
                      ProtobufCBuffer     *buffer);
Orderer__SubmitResponse *
       orderer__submit_response__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   orderer__submit_response__free_unpacked
                     (Orderer__SubmitResponse *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*Orderer__StepRequest_Closure)
                 (const Orderer__StepRequest *message,
                  void *closure_data);
typedef void (*Orderer__StepResponse_Closure)
                 (const Orderer__StepResponse *message,
                  void *closure_data);
typedef void (*Orderer__SubmitRequest_Closure)
                 (const Orderer__SubmitRequest *message,
                  void *closure_data);
typedef void (*Orderer__SubmitResponse_Closure)
                 (const Orderer__SubmitResponse *message,
                  void *closure_data);

/* --- services --- */

typedef struct _Orderer__Cluster_Service Orderer__Cluster_Service;
struct _Orderer__Cluster_Service
{
  ProtobufCService base;
  void (*submit)(Orderer__Cluster_Service *service,
                 const Orderer__SubmitRequest *input,
                 Orderer__SubmitResponse_Closure closure,
                 void *closure_data);
  void (*step)(Orderer__Cluster_Service *service,
               const Orderer__StepRequest *input,
               Orderer__StepResponse_Closure closure,
               void *closure_data);
};
typedef void (*Orderer__Cluster_ServiceDestroy)(Orderer__Cluster_Service *);
void orderer__cluster__init (Orderer__Cluster_Service *service,
                             Orderer__Cluster_ServiceDestroy destroy);
#define ORDERER__CLUSTER__BASE_INIT \
    { &orderer__cluster__descriptor, protobuf_c_service_invoke_internal, NULL }
#define ORDERER__CLUSTER__INIT(function_prefix__) \
    { ORDERER__CLUSTER__BASE_INIT,\
      function_prefix__ ## submit,\
      function_prefix__ ## step  }
void orderer__cluster__submit(ProtobufCService *service,
                              const Orderer__SubmitRequest *input,
                              Orderer__SubmitResponse_Closure closure,
                              void *closure_data);
void orderer__cluster__step(ProtobufCService *service,
                            const Orderer__StepRequest *input,
                            Orderer__StepResponse_Closure closure,
                            void *closure_data);

/* --- descriptors --- */

extern const ProtobufCMessageDescriptor orderer__step_request__descriptor;
extern const ProtobufCMessageDescriptor orderer__step_response__descriptor;
extern const ProtobufCMessageDescriptor orderer__submit_request__descriptor;
extern const ProtobufCMessageDescriptor orderer__submit_response__descriptor;
extern const ProtobufCServiceDescriptor orderer__cluster__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_orderer_2fcluster_2eproto__INCLUDED */
