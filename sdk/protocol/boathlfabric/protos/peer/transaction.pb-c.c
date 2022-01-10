/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: peer/transaction.proto */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C__NO_DEPRECATED
#define PROTOBUF_C__NO_DEPRECATED
#endif

#include "peer/transaction.pb-c.h"
void   protos__signed_transaction__init
                     (Protos__SignedTransaction         *message)
{
  static Protos__SignedTransaction init_value = PROTOS__SIGNED_TRANSACTION__INIT;
  *message = init_value;
}
size_t protos__signed_transaction__get_packed_size
                     (const Protos__SignedTransaction *message)
{
  assert(message->base.descriptor == &protos__signed_transaction__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t protos__signed_transaction__pack
                     (const Protos__SignedTransaction *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &protos__signed_transaction__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t protos__signed_transaction__pack_to_buffer
                     (const Protos__SignedTransaction *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &protos__signed_transaction__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Protos__SignedTransaction *
       protos__signed_transaction__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Protos__SignedTransaction *)
     protobuf_c_message_unpack (&protos__signed_transaction__descriptor,
                                allocator, len, data);
}
void   protos__signed_transaction__free_unpacked
                     (Protos__SignedTransaction *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &protos__signed_transaction__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   protos__processed_transaction__init
                     (Protos__ProcessedTransaction         *message)
{
  static Protos__ProcessedTransaction init_value = PROTOS__PROCESSED_TRANSACTION__INIT;
  *message = init_value;
}
size_t protos__processed_transaction__get_packed_size
                     (const Protos__ProcessedTransaction *message)
{
  assert(message->base.descriptor == &protos__processed_transaction__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t protos__processed_transaction__pack
                     (const Protos__ProcessedTransaction *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &protos__processed_transaction__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t protos__processed_transaction__pack_to_buffer
                     (const Protos__ProcessedTransaction *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &protos__processed_transaction__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Protos__ProcessedTransaction *
       protos__processed_transaction__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Protos__ProcessedTransaction *)
     protobuf_c_message_unpack (&protos__processed_transaction__descriptor,
                                allocator, len, data);
}
void   protos__processed_transaction__free_unpacked
                     (Protos__ProcessedTransaction *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &protos__processed_transaction__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   protos__transaction__init
                     (Protos__Transaction         *message)
{
  static Protos__Transaction init_value = PROTOS__TRANSACTION__INIT;
  *message = init_value;
}
size_t protos__transaction__get_packed_size
                     (const Protos__Transaction *message)
{
  assert(message->base.descriptor == &protos__transaction__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t protos__transaction__pack
                     (const Protos__Transaction *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &protos__transaction__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t protos__transaction__pack_to_buffer
                     (const Protos__Transaction *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &protos__transaction__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Protos__Transaction *
       protos__transaction__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Protos__Transaction *)
     protobuf_c_message_unpack (&protos__transaction__descriptor,
                                allocator, len, data);
}
void   protos__transaction__free_unpacked
                     (Protos__Transaction *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &protos__transaction__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   protos__transaction_action__init
                     (Protos__TransactionAction         *message)
{
  static Protos__TransactionAction init_value = PROTOS__TRANSACTION_ACTION__INIT;
  *message = init_value;
}
size_t protos__transaction_action__get_packed_size
                     (const Protos__TransactionAction *message)
{
  assert(message->base.descriptor == &protos__transaction_action__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t protos__transaction_action__pack
                     (const Protos__TransactionAction *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &protos__transaction_action__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t protos__transaction_action__pack_to_buffer
                     (const Protos__TransactionAction *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &protos__transaction_action__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Protos__TransactionAction *
       protos__transaction_action__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Protos__TransactionAction *)
     protobuf_c_message_unpack (&protos__transaction_action__descriptor,
                                allocator, len, data);
}
void   protos__transaction_action__free_unpacked
                     (Protos__TransactionAction *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &protos__transaction_action__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   protos__chaincode_action_payload__init
                     (Protos__ChaincodeActionPayload         *message)
{
  static Protos__ChaincodeActionPayload init_value = PROTOS__CHAINCODE_ACTION_PAYLOAD__INIT;
  *message = init_value;
}
size_t protos__chaincode_action_payload__get_packed_size
                     (const Protos__ChaincodeActionPayload *message)
{
  assert(message->base.descriptor == &protos__chaincode_action_payload__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t protos__chaincode_action_payload__pack
                     (const Protos__ChaincodeActionPayload *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &protos__chaincode_action_payload__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t protos__chaincode_action_payload__pack_to_buffer
                     (const Protos__ChaincodeActionPayload *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &protos__chaincode_action_payload__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Protos__ChaincodeActionPayload *
       protos__chaincode_action_payload__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Protos__ChaincodeActionPayload *)
     protobuf_c_message_unpack (&protos__chaincode_action_payload__descriptor,
                                allocator, len, data);
}
void   protos__chaincode_action_payload__free_unpacked
                     (Protos__ChaincodeActionPayload *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &protos__chaincode_action_payload__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   protos__chaincode_endorsed_action__init
                     (Protos__ChaincodeEndorsedAction         *message)
{
  static Protos__ChaincodeEndorsedAction init_value = PROTOS__CHAINCODE_ENDORSED_ACTION__INIT;
  *message = init_value;
}
size_t protos__chaincode_endorsed_action__get_packed_size
                     (const Protos__ChaincodeEndorsedAction *message)
{
  assert(message->base.descriptor == &protos__chaincode_endorsed_action__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t protos__chaincode_endorsed_action__pack
                     (const Protos__ChaincodeEndorsedAction *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &protos__chaincode_endorsed_action__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t protos__chaincode_endorsed_action__pack_to_buffer
                     (const Protos__ChaincodeEndorsedAction *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &protos__chaincode_endorsed_action__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
Protos__ChaincodeEndorsedAction *
       protos__chaincode_endorsed_action__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (Protos__ChaincodeEndorsedAction *)
     protobuf_c_message_unpack (&protos__chaincode_endorsed_action__descriptor,
                                allocator, len, data);
}
void   protos__chaincode_endorsed_action__free_unpacked
                     (Protos__ChaincodeEndorsedAction *message,
                      ProtobufCAllocator *allocator)
{
  assert(message->base.descriptor == &protos__chaincode_endorsed_action__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
static const ProtobufCFieldDescriptor protos__signed_transaction__field_descriptors[2] =
{
  {
    "transaction_bytes",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_BYTES,
    offsetof(Protos__SignedTransaction, has_transaction_bytes),
    offsetof(Protos__SignedTransaction, transaction_bytes),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "signature",
    2,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_BYTES,
    offsetof(Protos__SignedTransaction, has_signature),
    offsetof(Protos__SignedTransaction, signature),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned protos__signed_transaction__field_indices_by_name[] = {
  1,   /* field[1] = signature */
  0,   /* field[0] = transaction_bytes */
};
static const ProtobufCIntRange protos__signed_transaction__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor protos__signed_transaction__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "protos.SignedTransaction",
  "SignedTransaction",
  "Protos__SignedTransaction",
  "protos",
  sizeof(Protos__SignedTransaction),
  2,
  protos__signed_transaction__field_descriptors,
  protos__signed_transaction__field_indices_by_name,
  1,  protos__signed_transaction__number_ranges,
  (ProtobufCMessageInit) protos__signed_transaction__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor protos__processed_transaction__field_descriptors[2] =
{
  {
    "transactionEnvelope",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_MESSAGE,
    0,   /* quantifier_offset */
    offsetof(Protos__ProcessedTransaction, transactionenvelope),
    &common__envelope__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "validationCode",
    2,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_INT32,
    offsetof(Protos__ProcessedTransaction, has_validationcode),
    offsetof(Protos__ProcessedTransaction, validationcode),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned protos__processed_transaction__field_indices_by_name[] = {
  0,   /* field[0] = transactionEnvelope */
  1,   /* field[1] = validationCode */
};
static const ProtobufCIntRange protos__processed_transaction__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor protos__processed_transaction__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "protos.ProcessedTransaction",
  "ProcessedTransaction",
  "Protos__ProcessedTransaction",
  "protos",
  sizeof(Protos__ProcessedTransaction),
  2,
  protos__processed_transaction__field_descriptors,
  protos__processed_transaction__field_indices_by_name,
  1,  protos__processed_transaction__number_ranges,
  (ProtobufCMessageInit) protos__processed_transaction__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor protos__transaction__field_descriptors[1] =
{
  {
    "actions",
    1,
    PROTOBUF_C_LABEL_REPEATED,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(Protos__Transaction, n_actions),
    offsetof(Protos__Transaction, actions),
    &protos__transaction_action__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned protos__transaction__field_indices_by_name[] = {
  0,   /* field[0] = actions */
};
static const ProtobufCIntRange protos__transaction__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 1 }
};
const ProtobufCMessageDescriptor protos__transaction__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "protos.Transaction",
  "Transaction",
  "Protos__Transaction",
  "protos",
  sizeof(Protos__Transaction),
  1,
  protos__transaction__field_descriptors,
  protos__transaction__field_indices_by_name,
  1,  protos__transaction__number_ranges,
  (ProtobufCMessageInit) protos__transaction__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor protos__transaction_action__field_descriptors[2] =
{
  {
    "header",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_BYTES,
    offsetof(Protos__TransactionAction, has_header),
    offsetof(Protos__TransactionAction, header),
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
    offsetof(Protos__TransactionAction, has_payload),
    offsetof(Protos__TransactionAction, payload),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned protos__transaction_action__field_indices_by_name[] = {
  0,   /* field[0] = header */
  1,   /* field[1] = payload */
};
static const ProtobufCIntRange protos__transaction_action__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor protos__transaction_action__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "protos.TransactionAction",
  "TransactionAction",
  "Protos__TransactionAction",
  "protos",
  sizeof(Protos__TransactionAction),
  2,
  protos__transaction_action__field_descriptors,
  protos__transaction_action__field_indices_by_name,
  1,  protos__transaction_action__number_ranges,
  (ProtobufCMessageInit) protos__transaction_action__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor protos__chaincode_action_payload__field_descriptors[2] =
{
  {
    "chaincode_proposal_payload",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_BYTES,
    offsetof(Protos__ChaincodeActionPayload, has_chaincode_proposal_payload),
    offsetof(Protos__ChaincodeActionPayload, chaincode_proposal_payload),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "action",
    2,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_MESSAGE,
    0,   /* quantifier_offset */
    offsetof(Protos__ChaincodeActionPayload, action),
    &protos__chaincode_endorsed_action__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned protos__chaincode_action_payload__field_indices_by_name[] = {
  1,   /* field[1] = action */
  0,   /* field[0] = chaincode_proposal_payload */
};
static const ProtobufCIntRange protos__chaincode_action_payload__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor protos__chaincode_action_payload__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "protos.ChaincodeActionPayload",
  "ChaincodeActionPayload",
  "Protos__ChaincodeActionPayload",
  "protos",
  sizeof(Protos__ChaincodeActionPayload),
  2,
  protos__chaincode_action_payload__field_descriptors,
  protos__chaincode_action_payload__field_indices_by_name,
  1,  protos__chaincode_action_payload__number_ranges,
  (ProtobufCMessageInit) protos__chaincode_action_payload__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor protos__chaincode_endorsed_action__field_descriptors[2] =
{
  {
    "proposal_response_payload",
    1,
    PROTOBUF_C_LABEL_OPTIONAL,
    PROTOBUF_C_TYPE_BYTES,
    offsetof(Protos__ChaincodeEndorsedAction, has_proposal_response_payload),
    offsetof(Protos__ChaincodeEndorsedAction, proposal_response_payload),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "endorsements",
    2,
    PROTOBUF_C_LABEL_REPEATED,
    PROTOBUF_C_TYPE_MESSAGE,
    offsetof(Protos__ChaincodeEndorsedAction, n_endorsements),
    offsetof(Protos__ChaincodeEndorsedAction, endorsements),
    &protos__endorsement__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned protos__chaincode_endorsed_action__field_indices_by_name[] = {
  1,   /* field[1] = endorsements */
  0,   /* field[0] = proposal_response_payload */
};
static const ProtobufCIntRange protos__chaincode_endorsed_action__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor protos__chaincode_endorsed_action__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "protos.ChaincodeEndorsedAction",
  "ChaincodeEndorsedAction",
  "Protos__ChaincodeEndorsedAction",
  "protos",
  sizeof(Protos__ChaincodeEndorsedAction),
  2,
  protos__chaincode_endorsed_action__field_descriptors,
  protos__chaincode_endorsed_action__field_indices_by_name,
  1,  protos__chaincode_endorsed_action__number_ranges,
  (ProtobufCMessageInit) protos__chaincode_endorsed_action__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCEnumValue protos__tx_validation_code__enum_values_by_number[27] =
{
  { "VALID", "PROTOS__TX_VALIDATION_CODE__VALID", 0 },
  { "NIL_ENVELOPE", "PROTOS__TX_VALIDATION_CODE__NIL_ENVELOPE", 1 },
  { "BAD_PAYLOAD", "PROTOS__TX_VALIDATION_CODE__BAD_PAYLOAD", 2 },
  { "BAD_COMMON_HEADER", "PROTOS__TX_VALIDATION_CODE__BAD_COMMON_HEADER", 3 },
  { "BAD_CREATOR_SIGNATURE", "PROTOS__TX_VALIDATION_CODE__BAD_CREATOR_SIGNATURE", 4 },
  { "INVALID_ENDORSER_TRANSACTION", "PROTOS__TX_VALIDATION_CODE__INVALID_ENDORSER_TRANSACTION", 5 },
  { "INVALID_CONFIG_TRANSACTION", "PROTOS__TX_VALIDATION_CODE__INVALID_CONFIG_TRANSACTION", 6 },
  { "UNSUPPORTED_TX_PAYLOAD", "PROTOS__TX_VALIDATION_CODE__UNSUPPORTED_TX_PAYLOAD", 7 },
  { "BAD_PROPOSAL_TXID", "PROTOS__TX_VALIDATION_CODE__BAD_PROPOSAL_TXID", 8 },
  { "DUPLICATE_TXID", "PROTOS__TX_VALIDATION_CODE__DUPLICATE_TXID", 9 },
  { "ENDORSEMENT_POLICY_FAILURE", "PROTOS__TX_VALIDATION_CODE__ENDORSEMENT_POLICY_FAILURE", 10 },
  { "MVCC_READ_CONFLICT", "PROTOS__TX_VALIDATION_CODE__MVCC_READ_CONFLICT", 11 },
  { "PHANTOM_READ_CONFLICT", "PROTOS__TX_VALIDATION_CODE__PHANTOM_READ_CONFLICT", 12 },
  { "UNKNOWN_TX_TYPE", "PROTOS__TX_VALIDATION_CODE__UNKNOWN_TX_TYPE", 13 },
  { "TARGET_CHAIN_NOT_FOUND", "PROTOS__TX_VALIDATION_CODE__TARGET_CHAIN_NOT_FOUND", 14 },
  { "MARSHAL_TX_ERROR", "PROTOS__TX_VALIDATION_CODE__MARSHAL_TX_ERROR", 15 },
  { "NIL_TXACTION", "PROTOS__TX_VALIDATION_CODE__NIL_TXACTION", 16 },
  { "EXPIRED_CHAINCODE", "PROTOS__TX_VALIDATION_CODE__EXPIRED_CHAINCODE", 17 },
  { "CHAINCODE_VERSION_CONFLICT", "PROTOS__TX_VALIDATION_CODE__CHAINCODE_VERSION_CONFLICT", 18 },
  { "BAD_HEADER_EXTENSION", "PROTOS__TX_VALIDATION_CODE__BAD_HEADER_EXTENSION", 19 },
  { "BAD_CHANNEL_HEADER", "PROTOS__TX_VALIDATION_CODE__BAD_CHANNEL_HEADER", 20 },
  { "BAD_RESPONSE_PAYLOAD", "PROTOS__TX_VALIDATION_CODE__BAD_RESPONSE_PAYLOAD", 21 },
  { "BAD_RWSET", "PROTOS__TX_VALIDATION_CODE__BAD_RWSET", 22 },
  { "ILLEGAL_WRITESET", "PROTOS__TX_VALIDATION_CODE__ILLEGAL_WRITESET", 23 },
  { "INVALID_WRITESET", "PROTOS__TX_VALIDATION_CODE__INVALID_WRITESET", 24 },
  { "NOT_VALIDATED", "PROTOS__TX_VALIDATION_CODE__NOT_VALIDATED", 254 },
  { "INVALID_OTHER_REASON", "PROTOS__TX_VALIDATION_CODE__INVALID_OTHER_REASON", 255 },
};
static const ProtobufCIntRange protos__tx_validation_code__value_ranges[] = {
{0, 0},{254, 25},{0, 27}
};
static const ProtobufCEnumValueIndex protos__tx_validation_code__enum_values_by_name[27] =
{
  { "BAD_CHANNEL_HEADER", 20 },
  { "BAD_COMMON_HEADER", 3 },
  { "BAD_CREATOR_SIGNATURE", 4 },
  { "BAD_HEADER_EXTENSION", 19 },
  { "BAD_PAYLOAD", 2 },
  { "BAD_PROPOSAL_TXID", 8 },
  { "BAD_RESPONSE_PAYLOAD", 21 },
  { "BAD_RWSET", 22 },
  { "CHAINCODE_VERSION_CONFLICT", 18 },
  { "DUPLICATE_TXID", 9 },
  { "ENDORSEMENT_POLICY_FAILURE", 10 },
  { "EXPIRED_CHAINCODE", 17 },
  { "ILLEGAL_WRITESET", 23 },
  { "INVALID_CONFIG_TRANSACTION", 6 },
  { "INVALID_ENDORSER_TRANSACTION", 5 },
  { "INVALID_OTHER_REASON", 26 },
  { "INVALID_WRITESET", 24 },
  { "MARSHAL_TX_ERROR", 15 },
  { "MVCC_READ_CONFLICT", 11 },
  { "NIL_ENVELOPE", 1 },
  { "NIL_TXACTION", 16 },
  { "NOT_VALIDATED", 25 },
  { "PHANTOM_READ_CONFLICT", 12 },
  { "TARGET_CHAIN_NOT_FOUND", 14 },
  { "UNKNOWN_TX_TYPE", 13 },
  { "UNSUPPORTED_TX_PAYLOAD", 7 },
  { "VALID", 0 },
};
const ProtobufCEnumDescriptor protos__tx_validation_code__descriptor =
{
  PROTOBUF_C__ENUM_DESCRIPTOR_MAGIC,
  "protos.TxValidationCode",
  "TxValidationCode",
  "Protos__TxValidationCode",
  "protos",
  27,
  protos__tx_validation_code__enum_values_by_number,
  27,
  protos__tx_validation_code__enum_values_by_name,
  2,
  protos__tx_validation_code__value_ranges,
  NULL,NULL,NULL,NULL   /* reserved[1234] */
};
static const ProtobufCEnumValue protos__meta_data_keys__enum_values_by_number[1] =
{
  { "VALIDATION_PARAMETER", "PROTOS__META_DATA_KEYS__VALIDATION_PARAMETER", 0 },
};
static const ProtobufCIntRange protos__meta_data_keys__value_ranges[] = {
{0, 0},{0, 1}
};
static const ProtobufCEnumValueIndex protos__meta_data_keys__enum_values_by_name[1] =
{
  { "VALIDATION_PARAMETER", 0 },
};
const ProtobufCEnumDescriptor protos__meta_data_keys__descriptor =
{
  PROTOBUF_C__ENUM_DESCRIPTOR_MAGIC,
  "protos.MetaDataKeys",
  "MetaDataKeys",
  "Protos__MetaDataKeys",
  "protos",
  1,
  protos__meta_data_keys__enum_values_by_number,
  1,
  protos__meta_data_keys__enum_values_by_name,
  1,
  protos__meta_data_keys__value_ranges,
  NULL,NULL,NULL,NULL   /* reserved[1234] */
};
