/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: msp/msp_principal.proto */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C__NO_DEPRECATED
#define PROTOBUF_C__NO_DEPRECATED
#endif

#include "msp/msp_principal.pb-c.h"
void   common__mspprincipal__init
                     (Common__MSPPrincipal         *message)
{
  static Common__MSPPrincipal init_value = COMMON__MSPPRINCIPAL__INIT;
  *message = init_value;
}
size_t common__mspprincipal__get_packed_size
                     (const Common__MSPPrincipal *message)
{
  assert(message->base.descriptor == &common__mspprincipal__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t common__mspprincipal__pack
                     (const Common__MSPPrincipal *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &common__mspprincipal__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t common__mspprincipal__pack_to_buffer
                     (const Common__MSPPrincipal *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &common__mspprincipal__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Common__MSPPrincipal *
       common__mspprincipal__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Common__MSPPrincipal *)
     protobuf_c_message_unpack (&common__mspprincipal__descriptor,
                                allocator, len, data);
}
void   common__mspprincipal__free_unpacked
                     (Common__MSPPrincipal *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &common__mspprincipal__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   common__organization_unit__init
                     (Common__OrganizationUnit         *message)
{
  static Common__OrganizationUnit init_value = COMMON__ORGANIZATION_UNIT__INIT;
  *message = init_value;
}
size_t common__organization_unit__get_packed_size
                     (const Common__OrganizationUnit *message)
{
  assert(message->base.descriptor == &common__organization_unit__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t common__organization_unit__pack
                     (const Common__OrganizationUnit *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &common__organization_unit__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t common__organization_unit__pack_to_buffer
                     (const Common__OrganizationUnit *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &common__organization_unit__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Common__OrganizationUnit *
       common__organization_unit__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Common__OrganizationUnit *)
     protobuf_c_message_unpack (&common__organization_unit__descriptor,
                                allocator, len, data);
}
void   common__organization_unit__free_unpacked
                     (Common__OrganizationUnit *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &common__organization_unit__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   common__msprole__init
                     (Common__MSPRole         *message)
{
  static Common__MSPRole init_value = COMMON__MSPROLE__INIT;
  *message = init_value;
}
size_t common__msprole__get_packed_size
                     (const Common__MSPRole *message)
{
  assert(message->base.descriptor == &common__msprole__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t common__msprole__pack
                     (const Common__MSPRole *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &common__msprole__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t common__msprole__pack_to_buffer
                     (const Common__MSPRole *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &common__msprole__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Common__MSPRole *
       common__msprole__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Common__MSPRole *)
     protobuf_c_message_unpack (&common__msprole__descriptor,
                                allocator, len, data);
}
void   common__msprole__free_unpacked
                     (Common__MSPRole *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &common__msprole__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   common__mspidentity_anonymity__init
                     (Common__MSPIdentityAnonymity         *message)
{
  static Common__MSPIdentityAnonymity init_value = COMMON__MSPIDENTITY_ANONYMITY__INIT;
  *message = init_value;
}
size_t common__mspidentity_anonymity__get_packed_size
                     (const Common__MSPIdentityAnonymity *message)
{
  assert(message->base.descriptor == &common__mspidentity_anonymity__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t common__mspidentity_anonymity__pack
                     (const Common__MSPIdentityAnonymity *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &common__mspidentity_anonymity__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t common__mspidentity_anonymity__pack_to_buffer
                     (const Common__MSPIdentityAnonymity *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &common__mspidentity_anonymity__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Common__MSPIdentityAnonymity *
       common__mspidentity_anonymity__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Common__MSPIdentityAnonymity *)
     protobuf_c_message_unpack (&common__mspidentity_anonymity__descriptor,
                                allocator, len, data);
}
void   common__mspidentity_anonymity__free_unpacked
                     (Common__MSPIdentityAnonymity *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &common__mspidentity_anonymity__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   common__combined_principal__init
                     (Common__CombinedPrincipal         *message)
{
  static Common__CombinedPrincipal init_value = COMMON__COMBINED_PRINCIPAL__INIT;
  *message = init_value;
}
size_t common__combined_principal__get_packed_size
                     (const Common__CombinedPrincipal *message)
{
  assert(message->base.descriptor == &common__combined_principal__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t common__combined_principal__pack
                     (const Common__CombinedPrincipal *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &common__combined_principal__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t common__combined_principal__pack_to_buffer
                     (const Common__CombinedPrincipal *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &common__combined_principal__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Common__CombinedPrincipal *
       common__combined_principal__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Common__CombinedPrincipal *)
     protobuf_c_message_unpack (&common__combined_principal__descriptor,
                                allocator, len, data);
}
void   common__combined_principal__free_unpacked
                     (Common__CombinedPrincipal *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &common__combined_principal__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
static const ProtobufCEnumValue common__mspprincipal__classification__enum_values_by_number[5] =
{
  { "ROLE", "COMMON__MSPPRINCIPAL__CLASSIFICATION__ROLE", 0 },
  { "ORGANIZATION_UNIT", "COMMON__MSPPRINCIPAL__CLASSIFICATION__ORGANIZATION_UNIT", 1 },
  { "IDENTITY", "COMMON__MSPPRINCIPAL__CLASSIFICATION__IDENTITY", 2 },
  { "ANONYMITY", "COMMON__MSPPRINCIPAL__CLASSIFICATION__ANONYMITY", 3 },
  { "COMBINED", "COMMON__MSPPRINCIPAL__CLASSIFICATION__COMBINED", 4 },
};
static const ProtobufCIntRange common__mspprincipal__classification__value_ranges[] = {
{0, 0},{0, 5}
};
static const ProtobufCEnumValueIndex common__mspprincipal__classification__enum_values_by_name[5] =
{
  { "ANONYMITY", 3 },
  { "COMBINED", 4 },
  { "IDENTITY", 2 },
  { "ORGANIZATION_UNIT", 1 },
  { "ROLE", 0 },
};
const ProtobufCEnumDescriptor common__mspprincipal__classification__descriptor =
{
  PROTOBUF_C__ENUM_DESCRIPTOR_MAGIC,
  "common.MSPPrincipal.Classification",
  "Classification",
  "Common__MSPPrincipal__Classification",
  "common",
  5,
  common__mspprincipal__classification__enum_values_by_number,
  5,
  common__mspprincipal__classification__enum_values_by_name,
  1,
  common__mspprincipal__classification__value_ranges,
  NULL,NULL,NULL,NULL   /* reserved[1234] */
};
static const ProtobufCFieldDescriptor common__mspprincipal__field_descriptors[2] =
{
  {
    "principal_classification",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_ENUM,
    offsetof(Common__MSPPrincipal, has_principal_classification),
    offsetof(Common__MSPPrincipal, principal_classification),
    &common__mspprincipal__classification__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "principal",
    2,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_BYTES,
    offsetof(Common__MSPPrincipal, has_principal),
    offsetof(Common__MSPPrincipal, principal),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned common__mspprincipal__field_indices_by_name[] = {
  1,   /* field[1] = principal */
  0,   /* field[0] = principal_classification */
};
static const ProtobufCIntRange common__mspprincipal__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor common__mspprincipal__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "common.MSPPrincipal",
  "MSPPrincipal",
  "Common__MSPPrincipal",
  "common",
  sizeof(Common__MSPPrincipal),
  2,
  common__mspprincipal__field_descriptors,
  common__mspprincipal__field_indices_by_name,
  1,  common__mspprincipal__number_ranges,
  (ProtobufCMessageInit) common__mspprincipal__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor common__organization_unit__field_descriptors[3] =
{
  {
    "msp_identifier",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Common__OrganizationUnit, msp_identifier),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "organizational_unit_identifier",
    2,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Common__OrganizationUnit, organizational_unit_identifier),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "certifiers_identifier",
    3,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_BYTES,
    offsetof(Common__OrganizationUnit, has_certifiers_identifier),
    offsetof(Common__OrganizationUnit, certifiers_identifier),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned common__organization_unit__field_indices_by_name[] = {
  2,   /* field[2] = certifiers_identifier */
  0,   /* field[0] = msp_identifier */
  1,   /* field[1] = organizational_unit_identifier */
};
static const ProtobufCIntRange common__organization_unit__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 3 }
};
const ProtobufCMessageDescriptor common__organization_unit__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "common.OrganizationUnit",
  "OrganizationUnit",
  "Common__OrganizationUnit",
  "common",
  sizeof(Common__OrganizationUnit),
  3,
  common__organization_unit__field_descriptors,
  common__organization_unit__field_indices_by_name,
  1,  common__organization_unit__number_ranges,
  (ProtobufCMessageInit) common__organization_unit__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCEnumValue common__msprole__msprole_type__enum_values_by_number[4] =
{
  { "MEMBER", "COMMON__MSPROLE__MSPROLE_TYPE__MEMBER", 0 },
  { "ADMIN", "COMMON__MSPROLE__MSPROLE_TYPE__ADMIN", 1 },
  { "CLIENT", "COMMON__MSPROLE__MSPROLE_TYPE__CLIENT", 2 },
  { "PEER", "COMMON__MSPROLE__MSPROLE_TYPE__PEER", 3 },
};
static const ProtobufCIntRange common__msprole__msprole_type__value_ranges[] = {
{0, 0},{0, 4}
};
static const ProtobufCEnumValueIndex common__msprole__msprole_type__enum_values_by_name[4] =
{
  { "ADMIN", 1 },
  { "CLIENT", 2 },
  { "MEMBER", 0 },
  { "PEER", 3 },
};
const ProtobufCEnumDescriptor common__msprole__msprole_type__descriptor =
{
  PROTOBUF_C__ENUM_DESCRIPTOR_MAGIC,
  "common.MSPRole.MSPRoleType",
  "MSPRoleType",
  "Common__MSPRole__MSPRoleType",
  "common",
  4,
  common__msprole__msprole_type__enum_values_by_number,
  4,
  common__msprole__msprole_type__enum_values_by_name,
  1,
  common__msprole__msprole_type__value_ranges,
  NULL,NULL,NULL,NULL   /* reserved[1234] */
};
static const ProtobufCFieldDescriptor common__msprole__field_descriptors[2] =
{
  {
    "msp_identifier",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(Common__MSPRole, msp_identifier),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "role",
    2,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_ENUM,
    offsetof(Common__MSPRole, has_role),
    offsetof(Common__MSPRole, role),
    &common__msprole__msprole_type__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned common__msprole__field_indices_by_name[] = {
  0,   /* field[0] = msp_identifier */
  1,   /* field[1] = role */
};
static const ProtobufCIntRange common__msprole__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor common__msprole__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "common.MSPRole",
  "MSPRole",
  "Common__MSPRole",
  "common",
  sizeof(Common__MSPRole),
  2,
  common__msprole__field_descriptors,
  common__msprole__field_indices_by_name,
  1,  common__msprole__number_ranges,
  (ProtobufCMessageInit) common__msprole__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCEnumValue common__mspidentity_anonymity__mspidentity_anonymity_type__enum_values_by_number[2] =
{
  { "NOMINAL", "COMMON__MSPIDENTITY_ANONYMITY__MSPIDENTITY_ANONYMITY_TYPE__NOMINAL", 0 },
  { "ANONYMOUS", "COMMON__MSPIDENTITY_ANONYMITY__MSPIDENTITY_ANONYMITY_TYPE__ANONYMOUS", 1 },
};
static const ProtobufCIntRange common__mspidentity_anonymity__mspidentity_anonymity_type__value_ranges[] = {
{0, 0},{0, 2}
};
static const ProtobufCEnumValueIndex common__mspidentity_anonymity__mspidentity_anonymity_type__enum_values_by_name[2] =
{
  { "ANONYMOUS", 1 },
  { "NOMINAL", 0 },
};
const ProtobufCEnumDescriptor common__mspidentity_anonymity__mspidentity_anonymity_type__descriptor =
{
  PROTOBUF_C__ENUM_DESCRIPTOR_MAGIC,
  "common.MSPIdentityAnonymity.MSPIdentityAnonymityType",
  "MSPIdentityAnonymityType",
  "Common__MSPIdentityAnonymity__MSPIdentityAnonymityType",
  "common",
  2,
  common__mspidentity_anonymity__mspidentity_anonymity_type__enum_values_by_number,
  2,
  common__mspidentity_anonymity__mspidentity_anonymity_type__enum_values_by_name,
  1,
  common__mspidentity_anonymity__mspidentity_anonymity_type__value_ranges,
  NULL,NULL,NULL,NULL   /* reserved[1234] */
};
static const ProtobufCFieldDescriptor common__mspidentity_anonymity__field_descriptors[1] =
{
  {
    "anonymity_type",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_ENUM,
    offsetof(Common__MSPIdentityAnonymity, has_anonymity_type),
    offsetof(Common__MSPIdentityAnonymity, anonymity_type),
    &common__mspidentity_anonymity__mspidentity_anonymity_type__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned common__mspidentity_anonymity__field_indices_by_name[] = {
  0,   /* field[0] = anonymity_type */
};
static const ProtobufCIntRange common__mspidentity_anonymity__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 1 }
};
const ProtobufCMessageDescriptor common__mspidentity_anonymity__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "common.MSPIdentityAnonymity",
  "MSPIdentityAnonymity",
  "Common__MSPIdentityAnonymity",
  "common",
  sizeof(Common__MSPIdentityAnonymity),
  1,
  common__mspidentity_anonymity__field_descriptors,
  common__mspidentity_anonymity__field_indices_by_name,
  1,  common__mspidentity_anonymity__number_ranges,
  (ProtobufCMessageInit) common__mspidentity_anonymity__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor common__combined_principal__field_descriptors[1] =
{
  {
    "principals",
    1,
    PROTOBUF_C_LABEL_REPEATED,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(Common__CombinedPrincipal, n_principals),
    offsetof(Common__CombinedPrincipal, principals),
    &common__mspprincipal__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned common__combined_principal__field_indices_by_name[] = {
  0,   /* field[0] = principals */
};
static const ProtobufCIntRange common__combined_principal__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 1 }
};
const ProtobufCMessageDescriptor common__combined_principal__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "common.CombinedPrincipal",
  "CombinedPrincipal",
  "Common__CombinedPrincipal",
  "common",
  sizeof(Common__CombinedPrincipal),
  1,
  common__combined_principal__field_descriptors,
  common__combined_principal__field_indices_by_name,
  1,  common__combined_principal__number_ranges,
  (ProtobufCMessageInit) common__combined_principal__init,
  NULL,NULL,NULL    /* reserved[123] */
};
