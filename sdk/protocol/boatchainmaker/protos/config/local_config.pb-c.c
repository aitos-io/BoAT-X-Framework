/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: config/local_config.proto */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C__NO_DEPRECATED
#define PROTOBUF_C__NO_DEPRECATED
#endif

#include "config/local_config.pb-c.h"
void   config__debug_config_request__init
                     (Config__DebugConfigRequest         *message)
{
  static const Config__DebugConfigRequest init_value = CONFIG__DEBUG_CONFIG_REQUEST__INIT;
  *message = init_value;
}
size_t config__debug_config_request__get_packed_size
                     (const Config__DebugConfigRequest *message)
{
  assert(message->base.descriptor == &config__debug_config_request__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t config__debug_config_request__pack
                     (const Config__DebugConfigRequest *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &config__debug_config_request__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t config__debug_config_request__pack_to_buffer
                     (const Config__DebugConfigRequest *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &config__debug_config_request__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Config__DebugConfigRequest *
       config__debug_config_request__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Config__DebugConfigRequest *)
     protobuf_c_message_unpack (&config__debug_config_request__descriptor,
                                allocator, len, data);
}
void   config__debug_config_request__free_unpacked
                     (Config__DebugConfigRequest *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &config__debug_config_request__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   config__debug_config_response__init
                     (Config__DebugConfigResponse         *message)
{
  static const Config__DebugConfigResponse init_value = CONFIG__DEBUG_CONFIG_RESPONSE__INIT;
  *message = init_value;
}
size_t config__debug_config_response__get_packed_size
                     (const Config__DebugConfigResponse *message)
{
  assert(message->base.descriptor == &config__debug_config_response__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t config__debug_config_response__pack
                     (const Config__DebugConfigResponse *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &config__debug_config_response__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t config__debug_config_response__pack_to_buffer
                     (const Config__DebugConfigResponse *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &config__debug_config_response__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Config__DebugConfigResponse *
       config__debug_config_response__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Config__DebugConfigResponse *)
     protobuf_c_message_unpack (&config__debug_config_response__descriptor,
                                allocator, len, data);
}
void   config__debug_config_response__free_unpacked
                     (Config__DebugConfigResponse *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &config__debug_config_response__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   config__check_new_block_chain_config_request__init
                     (Config__CheckNewBlockChainConfigRequest         *message)
{
  static const Config__CheckNewBlockChainConfigRequest init_value = CONFIG__CHECK_NEW_BLOCK_CHAIN_CONFIG_REQUEST__INIT;
  *message = init_value;
}
size_t config__check_new_block_chain_config_request__get_packed_size
                     (const Config__CheckNewBlockChainConfigRequest *message)
{
  assert(message->base.descriptor == &config__check_new_block_chain_config_request__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t config__check_new_block_chain_config_request__pack
                     (const Config__CheckNewBlockChainConfigRequest *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &config__check_new_block_chain_config_request__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t config__check_new_block_chain_config_request__pack_to_buffer
                     (const Config__CheckNewBlockChainConfigRequest *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &config__check_new_block_chain_config_request__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Config__CheckNewBlockChainConfigRequest *
       config__check_new_block_chain_config_request__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Config__CheckNewBlockChainConfigRequest *)
     protobuf_c_message_unpack (&config__check_new_block_chain_config_request__descriptor,
                                allocator, len, data);
}
void   config__check_new_block_chain_config_request__free_unpacked
                     (Config__CheckNewBlockChainConfigRequest *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &config__check_new_block_chain_config_request__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   config__check_new_block_chain_config_response__init
                     (Config__CheckNewBlockChainConfigResponse         *message)
{
  static const Config__CheckNewBlockChainConfigResponse init_value = CONFIG__CHECK_NEW_BLOCK_CHAIN_CONFIG_RESPONSE__INIT;
  *message = init_value;
}
size_t config__check_new_block_chain_config_response__get_packed_size
                     (const Config__CheckNewBlockChainConfigResponse *message)
{
  assert(message->base.descriptor == &config__check_new_block_chain_config_response__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t config__check_new_block_chain_config_response__pack
                     (const Config__CheckNewBlockChainConfigResponse *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &config__check_new_block_chain_config_response__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t config__check_new_block_chain_config_response__pack_to_buffer
                     (const Config__CheckNewBlockChainConfigResponse *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &config__check_new_block_chain_config_response__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Config__CheckNewBlockChainConfigResponse *
       config__check_new_block_chain_config_response__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Config__CheckNewBlockChainConfigResponse *)
     protobuf_c_message_unpack (&config__check_new_block_chain_config_response__descriptor,
                                allocator, len, data);
}
void   config__check_new_block_chain_config_response__free_unpacked
                     (Config__CheckNewBlockChainConfigResponse *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &config__check_new_block_chain_config_response__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
static const ProtobufCFieldDescriptor config__debug_config_request__field_descriptors[1] =
{
  {
    "pairs",
    1,
    PROTOBUF_C_LABEL_REPEATED,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(Config__DebugConfigRequest, n_pairs),
    offsetof(Config__DebugConfigRequest, pairs),
    &common__key_value_pair__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned config__debug_config_request__field_indices_by_name[] = {
  0,   /* field[0] = pairs */
};
static const ProtobufCIntRange config__debug_config_request__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 1 }
};
const ProtobufCMessageDescriptor config__debug_config_request__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "config.DebugConfigRequest",
  "DebugConfigRequest",
  "Config__DebugConfigRequest",
  "config",
  sizeof(Config__DebugConfigRequest),
  1,
  config__debug_config_request__field_descriptors,
  config__debug_config_request__field_indices_by_name,
  1,  config__debug_config_request__number_ranges,
  (ProtobufCMessageInit) config__debug_config_request__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor config__debug_config_response__field_descriptors[2] =
{
  {
    "code",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_INT32,
    0,   /* quantifier_offset */
    offsetof(Config__DebugConfigResponse, code),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "message",
    2,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Config__DebugConfigResponse, message),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned config__debug_config_response__field_indices_by_name[] = {
  0,   /* field[0] = code */
  1,   /* field[1] = message */
};
static const ProtobufCIntRange config__debug_config_response__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor config__debug_config_response__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "config.DebugConfigResponse",
  "DebugConfigResponse",
  "Config__DebugConfigResponse",
  "config",
  sizeof(Config__DebugConfigResponse),
  2,
  config__debug_config_response__field_descriptors,
  config__debug_config_response__field_indices_by_name,
  1,  config__debug_config_response__number_ranges,
  (ProtobufCMessageInit) config__debug_config_response__init,
  NULL,NULL,NULL    /* reserved[123] */
};
#define config__check_new_block_chain_config_request__field_descriptors NULL
#define config__check_new_block_chain_config_request__field_indices_by_name NULL
#define config__check_new_block_chain_config_request__number_ranges NULL
const ProtobufCMessageDescriptor config__check_new_block_chain_config_request__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "config.CheckNewBlockChainConfigRequest",
  "CheckNewBlockChainConfigRequest",
  "Config__CheckNewBlockChainConfigRequest",
  "config",
  sizeof(Config__CheckNewBlockChainConfigRequest),
  0,
  config__check_new_block_chain_config_request__field_descriptors,
  config__check_new_block_chain_config_request__field_indices_by_name,
  0,  config__check_new_block_chain_config_request__number_ranges,
  (ProtobufCMessageInit) config__check_new_block_chain_config_request__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor config__check_new_block_chain_config_response__field_descriptors[2] =
{
  {
    "code",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_INT32,
    0,   /* quantifier_offset */
    offsetof(Config__CheckNewBlockChainConfigResponse, code),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "message",
    2,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Config__CheckNewBlockChainConfigResponse, message),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned config__check_new_block_chain_config_response__field_indices_by_name[] = {
  0,   /* field[0] = code */
  1,   /* field[1] = message */
};
static const ProtobufCIntRange config__check_new_block_chain_config_response__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor config__check_new_block_chain_config_response__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "config.CheckNewBlockChainConfigResponse",
  "CheckNewBlockChainConfigResponse",
  "Config__CheckNewBlockChainConfigResponse",
  "config",
  sizeof(Config__CheckNewBlockChainConfigResponse),
  2,
  config__check_new_block_chain_config_response__field_descriptors,
  config__check_new_block_chain_config_response__field_indices_by_name,
  1,  config__check_new_block_chain_config_response__number_ranges,
  (ProtobufCMessageInit) config__check_new_block_chain_config_response__init,
  NULL,NULL,NULL    /* reserved[123] */
};
