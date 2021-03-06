/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: peer/lifecycle/lifecycle.proto */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C__NO_DEPRECATED
#define PROTOBUF_C__NO_DEPRECATED
#endif

#include "peer/lifecycle/lifecycle.pb-c.h"
void   lifecycle__install_chaincode_args__init
                     (Lifecycle__InstallChaincodeArgs         *message)
{
  static Lifecycle__InstallChaincodeArgs init_value = LIFECYCLE__INSTALL_CHAINCODE_ARGS__INIT;
  *message = init_value;
}
size_t lifecycle__install_chaincode_args__get_packed_size
                     (const Lifecycle__InstallChaincodeArgs *message)
{
  assert(message->base.descriptor == &lifecycle__install_chaincode_args__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t lifecycle__install_chaincode_args__pack
                     (const Lifecycle__InstallChaincodeArgs *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &lifecycle__install_chaincode_args__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t lifecycle__install_chaincode_args__pack_to_buffer
                     (const Lifecycle__InstallChaincodeArgs *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &lifecycle__install_chaincode_args__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Lifecycle__InstallChaincodeArgs *
       lifecycle__install_chaincode_args__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Lifecycle__InstallChaincodeArgs *)
     protobuf_c_message_unpack (&lifecycle__install_chaincode_args__descriptor,
                                allocator, len, data);
}
void   lifecycle__install_chaincode_args__free_unpacked
                     (Lifecycle__InstallChaincodeArgs *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &lifecycle__install_chaincode_args__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   lifecycle__install_chaincode_result__init
                     (Lifecycle__InstallChaincodeResult         *message)
{
  static Lifecycle__InstallChaincodeResult init_value = LIFECYCLE__INSTALL_CHAINCODE_RESULT__INIT;
  *message = init_value;
}
size_t lifecycle__install_chaincode_result__get_packed_size
                     (const Lifecycle__InstallChaincodeResult *message)
{
  assert(message->base.descriptor == &lifecycle__install_chaincode_result__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t lifecycle__install_chaincode_result__pack
                     (const Lifecycle__InstallChaincodeResult *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &lifecycle__install_chaincode_result__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t lifecycle__install_chaincode_result__pack_to_buffer
                     (const Lifecycle__InstallChaincodeResult *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &lifecycle__install_chaincode_result__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Lifecycle__InstallChaincodeResult *
       lifecycle__install_chaincode_result__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Lifecycle__InstallChaincodeResult *)
     protobuf_c_message_unpack (&lifecycle__install_chaincode_result__descriptor,
                                allocator, len, data);
}
void   lifecycle__install_chaincode_result__free_unpacked
                     (Lifecycle__InstallChaincodeResult *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &lifecycle__install_chaincode_result__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   lifecycle__query_installed_chaincode_args__init
                     (Lifecycle__QueryInstalledChaincodeArgs         *message)
{
  static Lifecycle__QueryInstalledChaincodeArgs init_value = LIFECYCLE__QUERY_INSTALLED_CHAINCODE_ARGS__INIT;
  *message = init_value;
}
size_t lifecycle__query_installed_chaincode_args__get_packed_size
                     (const Lifecycle__QueryInstalledChaincodeArgs *message)
{
  assert(message->base.descriptor == &lifecycle__query_installed_chaincode_args__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t lifecycle__query_installed_chaincode_args__pack
                     (const Lifecycle__QueryInstalledChaincodeArgs *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &lifecycle__query_installed_chaincode_args__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t lifecycle__query_installed_chaincode_args__pack_to_buffer
                     (const Lifecycle__QueryInstalledChaincodeArgs *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &lifecycle__query_installed_chaincode_args__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Lifecycle__QueryInstalledChaincodeArgs *
       lifecycle__query_installed_chaincode_args__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Lifecycle__QueryInstalledChaincodeArgs *)
     protobuf_c_message_unpack (&lifecycle__query_installed_chaincode_args__descriptor,
                                allocator, len, data);
}
void   lifecycle__query_installed_chaincode_args__free_unpacked
                     (Lifecycle__QueryInstalledChaincodeArgs *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &lifecycle__query_installed_chaincode_args__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   lifecycle__query_installed_chaincode_result__init
                     (Lifecycle__QueryInstalledChaincodeResult         *message)
{
  static Lifecycle__QueryInstalledChaincodeResult init_value = LIFECYCLE__QUERY_INSTALLED_CHAINCODE_RESULT__INIT;
  *message = init_value;
}
size_t lifecycle__query_installed_chaincode_result__get_packed_size
                     (const Lifecycle__QueryInstalledChaincodeResult *message)
{
  assert(message->base.descriptor == &lifecycle__query_installed_chaincode_result__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t lifecycle__query_installed_chaincode_result__pack
                     (const Lifecycle__QueryInstalledChaincodeResult *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &lifecycle__query_installed_chaincode_result__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t lifecycle__query_installed_chaincode_result__pack_to_buffer
                     (const Lifecycle__QueryInstalledChaincodeResult *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &lifecycle__query_installed_chaincode_result__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Lifecycle__QueryInstalledChaincodeResult *
       lifecycle__query_installed_chaincode_result__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Lifecycle__QueryInstalledChaincodeResult *)
     protobuf_c_message_unpack (&lifecycle__query_installed_chaincode_result__descriptor,
                                allocator, len, data);
}
void   lifecycle__query_installed_chaincode_result__free_unpacked
                     (Lifecycle__QueryInstalledChaincodeResult *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &lifecycle__query_installed_chaincode_result__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
static const ProtobufCFieldDescriptor lifecycle__install_chaincode_args__field_descriptors[3] =
{
  {
    "name",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Lifecycle__InstallChaincodeArgs, name),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "version",
    2,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Lifecycle__InstallChaincodeArgs, version),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "chaincode_install_package",
    3,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_BYTES,
    offsetof(Lifecycle__InstallChaincodeArgs, has_chaincode_install_package),
    offsetof(Lifecycle__InstallChaincodeArgs, chaincode_install_package),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned lifecycle__install_chaincode_args__field_indices_by_name[] = {
  2,   /* field[2] = chaincode_install_package */
  0,   /* field[0] = name */
  1,   /* field[1] = version */
};
static const ProtobufCIntRange lifecycle__install_chaincode_args__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 3 }
};
const ProtobufCMessageDescriptor lifecycle__install_chaincode_args__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "lifecycle.InstallChaincodeArgs",
  "InstallChaincodeArgs",
  "Lifecycle__InstallChaincodeArgs",
  "lifecycle",
  sizeof(Lifecycle__InstallChaincodeArgs),
  3,
  lifecycle__install_chaincode_args__field_descriptors,
  lifecycle__install_chaincode_args__field_indices_by_name,
  1,  lifecycle__install_chaincode_args__number_ranges,
  (ProtobufCMessageInit) lifecycle__install_chaincode_args__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor lifecycle__install_chaincode_result__field_descriptors[1] =
{
  {
    "hash",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_BYTES,
    offsetof(Lifecycle__InstallChaincodeResult, has_hash),
    offsetof(Lifecycle__InstallChaincodeResult, hash),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned lifecycle__install_chaincode_result__field_indices_by_name[] = {
  0,   /* field[0] = hash */
};
static const ProtobufCIntRange lifecycle__install_chaincode_result__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 1 }
};
const ProtobufCMessageDescriptor lifecycle__install_chaincode_result__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "lifecycle.InstallChaincodeResult",
  "InstallChaincodeResult",
  "Lifecycle__InstallChaincodeResult",
  "lifecycle",
  sizeof(Lifecycle__InstallChaincodeResult),
  1,
  lifecycle__install_chaincode_result__field_descriptors,
  lifecycle__install_chaincode_result__field_indices_by_name,
  1,  lifecycle__install_chaincode_result__number_ranges,
  (ProtobufCMessageInit) lifecycle__install_chaincode_result__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor lifecycle__query_installed_chaincode_args__field_descriptors[2] =
{
  {
    "name",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Lifecycle__QueryInstalledChaincodeArgs, name),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "version",
    2,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Lifecycle__QueryInstalledChaincodeArgs, version),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned lifecycle__query_installed_chaincode_args__field_indices_by_name[] = {
  0,   /* field[0] = name */
  1,   /* field[1] = version */
};
static const ProtobufCIntRange lifecycle__query_installed_chaincode_args__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor lifecycle__query_installed_chaincode_args__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "lifecycle.QueryInstalledChaincodeArgs",
  "QueryInstalledChaincodeArgs",
  "Lifecycle__QueryInstalledChaincodeArgs",
  "lifecycle",
  sizeof(Lifecycle__QueryInstalledChaincodeArgs),
  2,
  lifecycle__query_installed_chaincode_args__field_descriptors,
  lifecycle__query_installed_chaincode_args__field_indices_by_name,
  1,  lifecycle__query_installed_chaincode_args__number_ranges,
  (ProtobufCMessageInit) lifecycle__query_installed_chaincode_args__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor lifecycle__query_installed_chaincode_result__field_descriptors[1] =
{
  {
    "hash",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_BYTES,
    offsetof(Lifecycle__QueryInstalledChaincodeResult, has_hash),
    offsetof(Lifecycle__QueryInstalledChaincodeResult, hash),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned lifecycle__query_installed_chaincode_result__field_indices_by_name[] = {
  0,   /* field[0] = hash */
};
static const ProtobufCIntRange lifecycle__query_installed_chaincode_result__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 1 }
};
const ProtobufCMessageDescriptor lifecycle__query_installed_chaincode_result__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "lifecycle.QueryInstalledChaincodeResult",
  "QueryInstalledChaincodeResult",
  "Lifecycle__QueryInstalledChaincodeResult",
  "lifecycle",
  sizeof(Lifecycle__QueryInstalledChaincodeResult),
  1,
  lifecycle__query_installed_chaincode_result__field_descriptors,
  lifecycle__query_installed_chaincode_result__field_indices_by_name,
  1,  lifecycle__query_installed_chaincode_result__number_ranges,
  (ProtobufCMessageInit) lifecycle__query_installed_chaincode_result__init,
  NULL,NULL,NULL    /* reserved[123] */
};
