/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: common/collection.proto */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C__NO_DEPRECATED
#define PROTOBUF_C__NO_DEPRECATED
#endif

#include "common/collection.pb-c.h"
void   common__collection_config_package__init
                     (Common__CollectionConfigPackage         *message)
{
  static Common__CollectionConfigPackage init_value = COMMON__COLLECTION_CONFIG_PACKAGE__INIT;
  *message = init_value;
}
size_t common__collection_config_package__get_packed_size
                     (const Common__CollectionConfigPackage *message)
{
  assert(message->base.descriptor == &common__collection_config_package__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t common__collection_config_package__pack
                     (const Common__CollectionConfigPackage *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &common__collection_config_package__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t common__collection_config_package__pack_to_buffer
                     (const Common__CollectionConfigPackage *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &common__collection_config_package__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Common__CollectionConfigPackage *
       common__collection_config_package__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Common__CollectionConfigPackage *)
     protobuf_c_message_unpack (&common__collection_config_package__descriptor,
                                allocator, len, data);
}
void   common__collection_config_package__free_unpacked
                     (Common__CollectionConfigPackage *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &common__collection_config_package__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   common__collection_config__init
                     (Common__CollectionConfig         *message)
{
  static Common__CollectionConfig init_value = COMMON__COLLECTION_CONFIG__INIT;
  *message = init_value;
}
size_t common__collection_config__get_packed_size
                     (const Common__CollectionConfig *message)
{
  assert(message->base.descriptor == &common__collection_config__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t common__collection_config__pack
                     (const Common__CollectionConfig *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &common__collection_config__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t common__collection_config__pack_to_buffer
                     (const Common__CollectionConfig *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &common__collection_config__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Common__CollectionConfig *
       common__collection_config__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Common__CollectionConfig *)
     protobuf_c_message_unpack (&common__collection_config__descriptor,
                                allocator, len, data);
}
void   common__collection_config__free_unpacked
                     (Common__CollectionConfig *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &common__collection_config__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   common__static_collection_config__init
                     (Common__StaticCollectionConfig         *message)
{
  static Common__StaticCollectionConfig init_value = COMMON__STATIC_COLLECTION_CONFIG__INIT;
  *message = init_value;
}
size_t common__static_collection_config__get_packed_size
                     (const Common__StaticCollectionConfig *message)
{
  assert(message->base.descriptor == &common__static_collection_config__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t common__static_collection_config__pack
                     (const Common__StaticCollectionConfig *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &common__static_collection_config__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t common__static_collection_config__pack_to_buffer
                     (const Common__StaticCollectionConfig *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &common__static_collection_config__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Common__StaticCollectionConfig *
       common__static_collection_config__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Common__StaticCollectionConfig *)
     protobuf_c_message_unpack (&common__static_collection_config__descriptor,
                                allocator, len, data);
}
void   common__static_collection_config__free_unpacked
                     (Common__StaticCollectionConfig *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &common__static_collection_config__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   common__collection_policy_config__init
                     (Common__CollectionPolicyConfig         *message)
{
  static Common__CollectionPolicyConfig init_value = COMMON__COLLECTION_POLICY_CONFIG__INIT;
  *message = init_value;
}
size_t common__collection_policy_config__get_packed_size
                     (const Common__CollectionPolicyConfig *message)
{
  assert(message->base.descriptor == &common__collection_policy_config__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t common__collection_policy_config__pack
                     (const Common__CollectionPolicyConfig *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &common__collection_policy_config__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t common__collection_policy_config__pack_to_buffer
                     (const Common__CollectionPolicyConfig *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &common__collection_policy_config__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Common__CollectionPolicyConfig *
       common__collection_policy_config__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Common__CollectionPolicyConfig *)
     protobuf_c_message_unpack (&common__collection_policy_config__descriptor,
                                allocator, len, data);
}
void   common__collection_policy_config__free_unpacked
                     (Common__CollectionPolicyConfig *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &common__collection_policy_config__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   common__collection_criteria__init
                     (Common__CollectionCriteria         *message)
{
  static Common__CollectionCriteria init_value = COMMON__COLLECTION_CRITERIA__INIT;
  *message = init_value;
}
size_t common__collection_criteria__get_packed_size
                     (const Common__CollectionCriteria *message)
{
  assert(message->base.descriptor == &common__collection_criteria__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t common__collection_criteria__pack
                     (const Common__CollectionCriteria *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &common__collection_criteria__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t common__collection_criteria__pack_to_buffer
                     (const Common__CollectionCriteria *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &common__collection_criteria__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Common__CollectionCriteria *
       common__collection_criteria__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Common__CollectionCriteria *)
     protobuf_c_message_unpack (&common__collection_criteria__descriptor,
                                allocator, len, data);
}
void   common__collection_criteria__free_unpacked
                     (Common__CollectionCriteria *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &common__collection_criteria__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
static const ProtobufCFieldDescriptor common__collection_config_package__field_descriptors[1] =
{
  {
    "config",
    1,
    PROTOBUF_C_LABEL_REPEATED,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(Common__CollectionConfigPackage, n_config),
    offsetof(Common__CollectionConfigPackage, config),
    &common__collection_config__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned common__collection_config_package__field_indices_by_name[] = {
  0,   /* field[0] = config */
};
static const ProtobufCIntRange common__collection_config_package__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 1 }
};
const ProtobufCMessageDescriptor common__collection_config_package__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "common.CollectionConfigPackage",
  "CollectionConfigPackage",
  "Common__CollectionConfigPackage",
  "common",
  sizeof(Common__CollectionConfigPackage),
  1,
  common__collection_config_package__field_descriptors,
  common__collection_config_package__field_indices_by_name,
  1,  common__collection_config_package__number_ranges,
  (ProtobufCMessageInit) common__collection_config_package__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor common__collection_config__field_descriptors[1] =
{
  {
    "static_collection_config",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(Common__CollectionConfig, payload_case),
    offsetof(Common__CollectionConfig, static_collection_config),
    &common__static_collection_config__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned common__collection_config__field_indices_by_name[] = {
  0,   /* field[0] = static_collection_config */
};
static const ProtobufCIntRange common__collection_config__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 1 }
};
const ProtobufCMessageDescriptor common__collection_config__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "common.CollectionConfig",
  "CollectionConfig",
  "Common__CollectionConfig",
  "common",
  sizeof(Common__CollectionConfig),
  1,
  common__collection_config__field_descriptors,
  common__collection_config__field_indices_by_name,
  1,  common__collection_config__number_ranges,
  (ProtobufCMessageInit) common__collection_config__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor common__static_collection_config__field_descriptors[6] =
{
  {
    "name",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Common__StaticCollectionConfig, name),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "member_orgs_policy",
    2,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_MESSAGE,
    0,   /* quantifier_offset */
    offsetof(Common__StaticCollectionConfig, member_orgs_policy),
    &common__collection_policy_config__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "required_peer_count",
    3,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_INT32,
    offsetof(Common__StaticCollectionConfig, has_required_peer_count),
    offsetof(Common__StaticCollectionConfig, required_peer_count),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "maximum_peer_count",
    4,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_INT32,
    offsetof(Common__StaticCollectionConfig, has_maximum_peer_count),
    offsetof(Common__StaticCollectionConfig, maximum_peer_count),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "block_to_live",
    5,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_UINT64,
    offsetof(Common__StaticCollectionConfig, has_block_to_live),
    offsetof(Common__StaticCollectionConfig, block_to_live),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "member_only_read",
    6,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_BOOL,
    offsetof(Common__StaticCollectionConfig, has_member_only_read),
    offsetof(Common__StaticCollectionConfig, member_only_read),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned common__static_collection_config__field_indices_by_name[] = {
  4,   /* field[4] = block_to_live */
  3,   /* field[3] = maximum_peer_count */
  5,   /* field[5] = member_only_read */
  1,   /* field[1] = member_orgs_policy */
  0,   /* field[0] = name */
  2,   /* field[2] = required_peer_count */
};
static const ProtobufCIntRange common__static_collection_config__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 6 }
};
const ProtobufCMessageDescriptor common__static_collection_config__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "common.StaticCollectionConfig",
  "StaticCollectionConfig",
  "Common__StaticCollectionConfig",
  "common",
  sizeof(Common__StaticCollectionConfig),
  6,
  common__static_collection_config__field_descriptors,
  common__static_collection_config__field_indices_by_name,
  1,  common__static_collection_config__number_ranges,
  (ProtobufCMessageInit) common__static_collection_config__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor common__collection_policy_config__field_descriptors[1] =
{
  {
    "signature_policy",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(Common__CollectionPolicyConfig, payload_case),
    offsetof(Common__CollectionPolicyConfig, signature_policy),
    &common__signature_policy_envelope__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned common__collection_policy_config__field_indices_by_name[] = {
  0,   /* field[0] = signature_policy */
};
static const ProtobufCIntRange common__collection_policy_config__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 1 }
};
const ProtobufCMessageDescriptor common__collection_policy_config__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "common.CollectionPolicyConfig",
  "CollectionPolicyConfig",
  "Common__CollectionPolicyConfig",
  "common",
  sizeof(Common__CollectionPolicyConfig),
  1,
  common__collection_policy_config__field_descriptors,
  common__collection_policy_config__field_indices_by_name,
  1,  common__collection_policy_config__number_ranges,
  (ProtobufCMessageInit) common__collection_policy_config__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor common__collection_criteria__field_descriptors[4] =
{
  {
    "channel",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Common__CollectionCriteria, channel),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "tx_id",
    2,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Common__CollectionCriteria, tx_id),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "collection",
    3,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Common__CollectionCriteria, collection),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "namespace",
    4,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Common__CollectionCriteria, namespace_),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned common__collection_criteria__field_indices_by_name[] = {
  0,   /* field[0] = channel */
  2,   /* field[2] = collection */
  3,   /* field[3] = namespace */
  1,   /* field[1] = tx_id */
};
static const ProtobufCIntRange common__collection_criteria__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 4 }
};
const ProtobufCMessageDescriptor common__collection_criteria__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "common.CollectionCriteria",
  "CollectionCriteria",
  "Common__CollectionCriteria",
  "common",
  sizeof(Common__CollectionCriteria),
  4,
  common__collection_criteria__field_descriptors,
  common__collection_criteria__field_indices_by_name,
  1,  common__collection_criteria__number_ranges,
  (ProtobufCMessageInit) common__collection_criteria__init,
  NULL,NULL,NULL    /* reserved[123] */
};
