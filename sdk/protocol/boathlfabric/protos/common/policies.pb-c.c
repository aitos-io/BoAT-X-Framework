/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: common/policies.proto */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C__NO_DEPRECATED
#define PROTOBUF_C__NO_DEPRECATED
#endif

#include "common/policies.pb-c.h"
void   common__policy__init
                     (Common__Policy         *message)
{
  static Common__Policy init_value = COMMON__POLICY__INIT;
  *message = init_value;
}
size_t common__policy__get_packed_size
                     (const Common__Policy *message)
{
  assert(message->base.descriptor == &common__policy__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t common__policy__pack
                     (const Common__Policy *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &common__policy__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t common__policy__pack_to_buffer
                     (const Common__Policy *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &common__policy__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Common__Policy *
       common__policy__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Common__Policy *)
     protobuf_c_message_unpack (&common__policy__descriptor,
                                allocator, len, data);
}
void   common__policy__free_unpacked
                     (Common__Policy *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &common__policy__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   common__signature_policy_envelope__init
                     (Common__SignaturePolicyEnvelope         *message)
{
  static Common__SignaturePolicyEnvelope init_value = COMMON__SIGNATURE_POLICY_ENVELOPE__INIT;
  *message = init_value;
}
size_t common__signature_policy_envelope__get_packed_size
                     (const Common__SignaturePolicyEnvelope *message)
{
  assert(message->base.descriptor == &common__signature_policy_envelope__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t common__signature_policy_envelope__pack
                     (const Common__SignaturePolicyEnvelope *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &common__signature_policy_envelope__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t common__signature_policy_envelope__pack_to_buffer
                     (const Common__SignaturePolicyEnvelope *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &common__signature_policy_envelope__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Common__SignaturePolicyEnvelope *
       common__signature_policy_envelope__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Common__SignaturePolicyEnvelope *)
     protobuf_c_message_unpack (&common__signature_policy_envelope__descriptor,
                                allocator, len, data);
}
void   common__signature_policy_envelope__free_unpacked
                     (Common__SignaturePolicyEnvelope *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &common__signature_policy_envelope__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   common__signature_policy__nout_of__init
                     (Common__SignaturePolicy__NOutOf         *message)
{
  static Common__SignaturePolicy__NOutOf init_value = COMMON__SIGNATURE_POLICY__NOUT_OF__INIT;
  *message = init_value;
}
void   common__signature_policy__init
                     (Common__SignaturePolicy         *message)
{
  static Common__SignaturePolicy init_value = COMMON__SIGNATURE_POLICY__INIT;
  *message = init_value;
}
size_t common__signature_policy__get_packed_size
                     (const Common__SignaturePolicy *message)
{
  assert(message->base.descriptor == &common__signature_policy__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t common__signature_policy__pack
                     (const Common__SignaturePolicy *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &common__signature_policy__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t common__signature_policy__pack_to_buffer
                     (const Common__SignaturePolicy *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &common__signature_policy__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Common__SignaturePolicy *
       common__signature_policy__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Common__SignaturePolicy *)
     protobuf_c_message_unpack (&common__signature_policy__descriptor,
                                allocator, len, data);
}
void   common__signature_policy__free_unpacked
                     (Common__SignaturePolicy *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &common__signature_policy__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   common__implicit_meta_policy__init
                     (Common__ImplicitMetaPolicy         *message)
{
  static Common__ImplicitMetaPolicy init_value = COMMON__IMPLICIT_META_POLICY__INIT;
  *message = init_value;
}
size_t common__implicit_meta_policy__get_packed_size
                     (const Common__ImplicitMetaPolicy *message)
{
  assert(message->base.descriptor == &common__implicit_meta_policy__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t common__implicit_meta_policy__pack
                     (const Common__ImplicitMetaPolicy *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &common__implicit_meta_policy__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t common__implicit_meta_policy__pack_to_buffer
                     (const Common__ImplicitMetaPolicy *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &common__implicit_meta_policy__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Common__ImplicitMetaPolicy *
       common__implicit_meta_policy__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Common__ImplicitMetaPolicy *)
     protobuf_c_message_unpack (&common__implicit_meta_policy__descriptor,
                                allocator, len, data);
}
void   common__implicit_meta_policy__free_unpacked
                     (Common__ImplicitMetaPolicy *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &common__implicit_meta_policy__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
static const ProtobufCEnumValue common__policy__policy_type__enum_values_by_number[4] =
{
  { "UNKNOWN", "COMMON__POLICY__POLICY_TYPE__UNKNOWN", 0 },
  { "SIGNATURE", "COMMON__POLICY__POLICY_TYPE__SIGNATURE", 1 },
  { "MSP", "COMMON__POLICY__POLICY_TYPE__MSP", 2 },
  { "IMPLICIT_META", "COMMON__POLICY__POLICY_TYPE__IMPLICIT_META", 3 },
};
static const ProtobufCIntRange common__policy__policy_type__value_ranges[] = {
{0, 0},{0, 4}
};
static const ProtobufCEnumValueIndex common__policy__policy_type__enum_values_by_name[4] =
{
  { "IMPLICIT_META", 3 },
  { "MSP", 2 },
  { "SIGNATURE", 1 },
  { "UNKNOWN", 0 },
};
const ProtobufCEnumDescriptor common__policy__policy_type__descriptor =
{
  PROTOBUF_C__ENUM_DESCRIPTOR_MAGIC,
  "common.Policy.PolicyType",
  "PolicyType",
  "Common__Policy__PolicyType",
  "common",
  4,
  common__policy__policy_type__enum_values_by_number,
  4,
  common__policy__policy_type__enum_values_by_name,
  1,
  common__policy__policy_type__value_ranges,
  NULL,NULL,NULL,NULL   /* reserved[1234] */
};
static const ProtobufCFieldDescriptor common__policy__field_descriptors[2] =
{
  {
    "type",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_INT32,
    offsetof(Common__Policy, has_type),
    offsetof(Common__Policy, type),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "value",
    2,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_BYTES,
    offsetof(Common__Policy, has_value),
    offsetof(Common__Policy, value),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned common__policy__field_indices_by_name[] = {
  0,   /* field[0] = type */
  1,   /* field[1] = value */
};
static const ProtobufCIntRange common__policy__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor common__policy__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "common.Policy",
  "Policy",
  "Common__Policy",
  "common",
  sizeof(Common__Policy),
  2,
  common__policy__field_descriptors,
  common__policy__field_indices_by_name,
  1,  common__policy__number_ranges,
  (ProtobufCMessageInit) common__policy__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor common__signature_policy_envelope__field_descriptors[3] =
{
  {
    "version",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_INT32,
    offsetof(Common__SignaturePolicyEnvelope, has_version),
    offsetof(Common__SignaturePolicyEnvelope, version),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "rule",
    2,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_MESSAGE,
    0,   /* quantifier_offset */
    offsetof(Common__SignaturePolicyEnvelope, rule),
    &common__signature_policy__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "identities",
    3,
    PROTOBUF_C_LABEL_REPEATED,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(Common__SignaturePolicyEnvelope, n_identities),
    offsetof(Common__SignaturePolicyEnvelope, identities),
    &common__mspprincipal__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned common__signature_policy_envelope__field_indices_by_name[] = {
  2,   /* field[2] = identities */
  1,   /* field[1] = rule */
  0,   /* field[0] = version */
};
static const ProtobufCIntRange common__signature_policy_envelope__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 3 }
};
const ProtobufCMessageDescriptor common__signature_policy_envelope__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "common.SignaturePolicyEnvelope",
  "SignaturePolicyEnvelope",
  "Common__SignaturePolicyEnvelope",
  "common",
  sizeof(Common__SignaturePolicyEnvelope),
  3,
  common__signature_policy_envelope__field_descriptors,
  common__signature_policy_envelope__field_indices_by_name,
  1,  common__signature_policy_envelope__number_ranges,
  (ProtobufCMessageInit) common__signature_policy_envelope__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor common__signature_policy__nout_of__field_descriptors[2] =
{
  {
    "n",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_INT32,
    offsetof(Common__SignaturePolicy__NOutOf, has_n),
    offsetof(Common__SignaturePolicy__NOutOf, n),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "rules",
    2,
    PROTOBUF_C_LABEL_REPEATED,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(Common__SignaturePolicy__NOutOf, n_rules),
    offsetof(Common__SignaturePolicy__NOutOf, rules),
    &common__signature_policy__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned common__signature_policy__nout_of__field_indices_by_name[] = {
  0,   /* field[0] = n */
  1,   /* field[1] = rules */
};
static const ProtobufCIntRange common__signature_policy__nout_of__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor common__signature_policy__nout_of__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "common.SignaturePolicy.NOutOf",
  "NOutOf",
  "Common__SignaturePolicy__NOutOf",
  "common",
  sizeof(Common__SignaturePolicy__NOutOf),
  2,
  common__signature_policy__nout_of__field_descriptors,
  common__signature_policy__nout_of__field_indices_by_name,
  1,  common__signature_policy__nout_of__number_ranges,
  (ProtobufCMessageInit) common__signature_policy__nout_of__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor common__signature_policy__field_descriptors[2] =
{
  {
    "signed_by",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_INT32,
    offsetof(Common__SignaturePolicy, type_case),
    offsetof(Common__SignaturePolicy, signed_by),
    NULL,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "n_out_of",
    2,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(Common__SignaturePolicy, type_case),
    offsetof(Common__SignaturePolicy, n_out_of),
    &common__signature_policy__nout_of__descriptor,
    NULL,
    0 | PROTOBUF_C_FIELD_FLAG_ONEOF,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned common__signature_policy__field_indices_by_name[] = {
  1,   /* field[1] = n_out_of */
  0,   /* field[0] = signed_by */
};
static const ProtobufCIntRange common__signature_policy__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor common__signature_policy__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "common.SignaturePolicy",
  "SignaturePolicy",
  "Common__SignaturePolicy",
  "common",
  sizeof(Common__SignaturePolicy),
  2,
  common__signature_policy__field_descriptors,
  common__signature_policy__field_indices_by_name,
  1,  common__signature_policy__number_ranges,
  (ProtobufCMessageInit) common__signature_policy__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCEnumValue common__implicit_meta_policy__rule__enum_values_by_number[3] =
{
  { "ANY", "COMMON__IMPLICIT_META_POLICY__RULE__ANY", 0 },
  { "ALL", "COMMON__IMPLICIT_META_POLICY__RULE__ALL", 1 },
  { "MAJORITY", "COMMON__IMPLICIT_META_POLICY__RULE__MAJORITY", 2 },
};
static const ProtobufCIntRange common__implicit_meta_policy__rule__value_ranges[] = {
{0, 0},{0, 3}
};
static const ProtobufCEnumValueIndex common__implicit_meta_policy__rule__enum_values_by_name[3] =
{
  { "ALL", 1 },
  { "ANY", 0 },
  { "MAJORITY", 2 },
};
const ProtobufCEnumDescriptor common__implicit_meta_policy__rule__descriptor =
{
  PROTOBUF_C__ENUM_DESCRIPTOR_MAGIC,
  "common.ImplicitMetaPolicy.Rule",
  "Rule",
  "Common__ImplicitMetaPolicy__Rule",
  "common",
  3,
  common__implicit_meta_policy__rule__enum_values_by_number,
  3,
  common__implicit_meta_policy__rule__enum_values_by_name,
  1,
  common__implicit_meta_policy__rule__value_ranges,
  NULL,NULL,NULL,NULL   /* reserved[1234] */
};
static const ProtobufCFieldDescriptor common__implicit_meta_policy__field_descriptors[2] =
{
  {
    "sub_policy",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Common__ImplicitMetaPolicy, sub_policy),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "rule",
    2,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_ENUM,
    offsetof(Common__ImplicitMetaPolicy, has_rule),
    offsetof(Common__ImplicitMetaPolicy, rule),
    &common__implicit_meta_policy__rule__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned common__implicit_meta_policy__field_indices_by_name[] = {
  1,   /* field[1] = rule */
  0,   /* field[0] = sub_policy */
};
static const ProtobufCIntRange common__implicit_meta_policy__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor common__implicit_meta_policy__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "common.ImplicitMetaPolicy",
  "ImplicitMetaPolicy",
  "Common__ImplicitMetaPolicy",
  "common",
  sizeof(Common__ImplicitMetaPolicy),
  2,
  common__implicit_meta_policy__field_descriptors,
  common__implicit_meta_policy__field_indices_by_name,
  1,  common__implicit_meta_policy__number_ranges,
  (ProtobufCMessageInit) common__implicit_meta_policy__init,
  NULL,NULL,NULL    /* reserved[123] */
};
