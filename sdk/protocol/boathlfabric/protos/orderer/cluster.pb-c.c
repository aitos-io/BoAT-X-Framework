/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: orderer/cluster.proto */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C__NO_DEPRECATED
#define PROTOBUF_C__NO_DEPRECATED
#endif

#include "orderer/cluster.pb-c.h"
void   orderer__step_request__init
                     (Orderer__StepRequest         *message)
{
  static Orderer__StepRequest init_value = ORDERER__STEP_REQUEST__INIT;
  *message = init_value;
}
size_t orderer__step_request__get_packed_size
                     (const Orderer__StepRequest *message)
{
  assert(message->base.descriptor == &orderer__step_request__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t orderer__step_request__pack
                     (const Orderer__StepRequest *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &orderer__step_request__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t orderer__step_request__pack_to_buffer
                     (const Orderer__StepRequest *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &orderer__step_request__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Orderer__StepRequest *
       orderer__step_request__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Orderer__StepRequest *)
     protobuf_c_message_unpack (&orderer__step_request__descriptor,
                                allocator, len, data);
}
void   orderer__step_request__free_unpacked
                     (Orderer__StepRequest *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &orderer__step_request__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   orderer__step_response__init
                     (Orderer__StepResponse         *message)
{
  static Orderer__StepResponse init_value = ORDERER__STEP_RESPONSE__INIT;
  *message = init_value;
}
size_t orderer__step_response__get_packed_size
                     (const Orderer__StepResponse *message)
{
  assert(message->base.descriptor == &orderer__step_response__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t orderer__step_response__pack
                     (const Orderer__StepResponse *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &orderer__step_response__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t orderer__step_response__pack_to_buffer
                     (const Orderer__StepResponse *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &orderer__step_response__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Orderer__StepResponse *
       orderer__step_response__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Orderer__StepResponse *)
     protobuf_c_message_unpack (&orderer__step_response__descriptor,
                                allocator, len, data);
}
void   orderer__step_response__free_unpacked
                     (Orderer__StepResponse *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &orderer__step_response__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   orderer__submit_request__init
                     (Orderer__SubmitRequest         *message)
{
  static Orderer__SubmitRequest init_value = ORDERER__SUBMIT_REQUEST__INIT;
  *message = init_value;
}
size_t orderer__submit_request__get_packed_size
                     (const Orderer__SubmitRequest *message)
{
  assert(message->base.descriptor == &orderer__submit_request__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t orderer__submit_request__pack
                     (const Orderer__SubmitRequest *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &orderer__submit_request__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t orderer__submit_request__pack_to_buffer
                     (const Orderer__SubmitRequest *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &orderer__submit_request__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Orderer__SubmitRequest *
       orderer__submit_request__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Orderer__SubmitRequest *)
     protobuf_c_message_unpack (&orderer__submit_request__descriptor,
                                allocator, len, data);
}
void   orderer__submit_request__free_unpacked
                     (Orderer__SubmitRequest *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &orderer__submit_request__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   orderer__submit_response__init
                     (Orderer__SubmitResponse         *message)
{
  static Orderer__SubmitResponse init_value = ORDERER__SUBMIT_RESPONSE__INIT;
  *message = init_value;
}
size_t orderer__submit_response__get_packed_size
                     (const Orderer__SubmitResponse *message)
{
  assert(message->base.descriptor == &orderer__submit_response__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t orderer__submit_response__pack
                     (const Orderer__SubmitResponse *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &orderer__submit_response__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t orderer__submit_response__pack_to_buffer
                     (const Orderer__SubmitResponse *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &orderer__submit_response__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Orderer__SubmitResponse *
       orderer__submit_response__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Orderer__SubmitResponse *)
     protobuf_c_message_unpack (&orderer__submit_response__descriptor,
                                allocator, len, data);
}
void   orderer__submit_response__free_unpacked
                     (Orderer__SubmitResponse *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &orderer__submit_response__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
static const ProtobufCFieldDescriptor orderer__step_request__field_descriptors[2] =
{
  {
    "channel",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Orderer__StepRequest, channel),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "payload",
    2,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_BYTES,
    offsetof(Orderer__StepRequest, has_payload),
    offsetof(Orderer__StepRequest, payload),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned orderer__step_request__field_indices_by_name[] = {
  0,   /* field[0] = channel */
  1,   /* field[1] = payload */
};
static const ProtobufCIntRange orderer__step_request__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor orderer__step_request__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "orderer.StepRequest",
  "StepRequest",
  "Orderer__StepRequest",
  "orderer",
  sizeof(Orderer__StepRequest),
  2,
  orderer__step_request__field_descriptors,
  orderer__step_request__field_indices_by_name,
  1,  orderer__step_request__number_ranges,
  (ProtobufCMessageInit) orderer__step_request__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor orderer__step_response__field_descriptors[1] =
{
  {
    "payload",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_BYTES,
    offsetof(Orderer__StepResponse, has_payload),
    offsetof(Orderer__StepResponse, payload),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned orderer__step_response__field_indices_by_name[] = {
  0,   /* field[0] = payload */
};
static const ProtobufCIntRange orderer__step_response__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 1 }
};
const ProtobufCMessageDescriptor orderer__step_response__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "orderer.StepResponse",
  "StepResponse",
  "Orderer__StepResponse",
  "orderer",
  sizeof(Orderer__StepResponse),
  1,
  orderer__step_response__field_descriptors,
  orderer__step_response__field_indices_by_name,
  1,  orderer__step_response__number_ranges,
  (ProtobufCMessageInit) orderer__step_response__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor orderer__submit_request__field_descriptors[3] =
{
  {
    "channel",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Orderer__SubmitRequest, channel),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "last_validation_seq",
    2,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_UINT64,
    offsetof(Orderer__SubmitRequest, has_last_validation_seq),
    offsetof(Orderer__SubmitRequest, last_validation_seq),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "content",
    3,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_MESSAGE,
    0,   /* quantifier_offset */
    offsetof(Orderer__SubmitRequest, content),
    &common__envelope__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned orderer__submit_request__field_indices_by_name[] = {
  0,   /* field[0] = channel */
  2,   /* field[2] = content */
  1,   /* field[1] = last_validation_seq */
};
static const ProtobufCIntRange orderer__submit_request__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 3 }
};
const ProtobufCMessageDescriptor orderer__submit_request__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "orderer.SubmitRequest",
  "SubmitRequest",
  "Orderer__SubmitRequest",
  "orderer",
  sizeof(Orderer__SubmitRequest),
  3,
  orderer__submit_request__field_descriptors,
  orderer__submit_request__field_indices_by_name,
  1,  orderer__submit_request__number_ranges,
  (ProtobufCMessageInit) orderer__submit_request__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor orderer__submit_response__field_descriptors[2] =
{
  {
    "status",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_ENUM,
    offsetof(Orderer__SubmitResponse, has_status),
    offsetof(Orderer__SubmitResponse, status),
    &common__status__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "info",
    2,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Orderer__SubmitResponse, info),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned orderer__submit_response__field_indices_by_name[] = {
  1,   /* field[1] = info */
  0,   /* field[0] = status */
};
static const ProtobufCIntRange orderer__submit_response__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor orderer__submit_response__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "orderer.SubmitResponse",
  "SubmitResponse",
  "Orderer__SubmitResponse",
  "orderer",
  sizeof(Orderer__SubmitResponse),
  2,
  orderer__submit_response__field_descriptors,
  orderer__submit_response__field_indices_by_name,
  1,  orderer__submit_response__number_ranges,
  (ProtobufCMessageInit) orderer__submit_response__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCMethodDescriptor orderer__cluster__method_descriptors[2] =
{
  { "Submit", &orderer__submit_request__descriptor, &orderer__submit_response__descriptor },
  { "Step", &orderer__step_request__descriptor, &orderer__step_response__descriptor },
};
const unsigned orderer__cluster__method_indices_by_name[] = {
  1,        /* Step */
  0         /* Submit */
};
const ProtobufCServiceDescriptor orderer__cluster__descriptor =
{
  PROTOBUF_C__SERVICE_DESCRIPTOR_MAGIC,
  "orderer.Cluster",
  "Cluster",
  "Orderer__Cluster",
  "orderer",
  2,
  orderer__cluster__method_descriptors,
  orderer__cluster__method_indices_by_name
};
void orderer__cluster__submit(ProtobufCService *service,
                              const Orderer__SubmitRequest *input,
                              Orderer__SubmitResponse_Closure closure,
                              void *closure_data)
{
  assert(service->descriptor == &orderer__cluster__descriptor);
  service->invoke(service, 0, (const ProtobufCMessage *) input, (ProtobufCClosure) closure, closure_data);
}
void orderer__cluster__step(ProtobufCService *service,
                            const Orderer__StepRequest *input,
                            Orderer__StepResponse_Closure closure,
                            void *closure_data)
{
  assert(service->descriptor == &orderer__cluster__descriptor);
  service->invoke(service, 1, (const ProtobufCMessage *) input, (ProtobufCClosure) closure, closure_data);
}
void orderer__cluster__init (Orderer__Cluster_Service *service,
                             Orderer__Cluster_ServiceDestroy destroy)
{
  protobuf_c_service_generated_init (&service->base,
                                     &orderer__cluster__descriptor,
                                     (ProtobufCServiceDestroy) destroy);
}
