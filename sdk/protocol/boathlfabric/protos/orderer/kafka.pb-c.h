/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: orderer/kafka.proto */

#ifndef PROTOBUF_C_orderer_2fkafka_2eproto__INCLUDED
#define PROTOBUF_C_orderer_2fkafka_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1000000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1002001 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _Orderer__KafkaMessage Orderer__KafkaMessage;
typedef struct _Orderer__KafkaMessageRegular Orderer__KafkaMessageRegular;
typedef struct _Orderer__KafkaMessageTimeToCut Orderer__KafkaMessageTimeToCut;
typedef struct _Orderer__KafkaMessageConnect Orderer__KafkaMessageConnect;
typedef struct _Orderer__KafkaMetadata Orderer__KafkaMetadata;


/* --- enums --- */

typedef enum _Orderer__KafkaMessageRegular__Class {
  ORDERER__KAFKA_MESSAGE_REGULAR__CLASS__UNKNOWN = 0,
  ORDERER__KAFKA_MESSAGE_REGULAR__CLASS__NORMAL = 1,
  ORDERER__KAFKA_MESSAGE_REGULAR__CLASS__CONFIG = 2
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(ORDERER__KAFKA_MESSAGE_REGULAR__CLASS)
} Orderer__KafkaMessageRegular__Class;

/* --- messages --- */

typedef enum {
  ORDERER__KAFKA_MESSAGE__TYPE__NOT_SET = 0,
  ORDERER__KAFKA_MESSAGE__TYPE_REGULAR = 1,
  ORDERER__KAFKA_MESSAGE__TYPE_TIME_TO_CUT = 2,
  ORDERER__KAFKA_MESSAGE__TYPE_CONNECT = 3,
} Orderer__KafkaMessage__TypeCase;

/*
 * KafkaMessage is a wrapper type for the messages
 * that the Kafka-based orderer deals with.
 */
struct  _Orderer__KafkaMessage
{
  ProtobufCMessage base;
  Orderer__KafkaMessage__TypeCase type_case;
  union {
    Orderer__KafkaMessageRegular *regular;
    Orderer__KafkaMessageTimeToCut *time_to_cut;
    Orderer__KafkaMessageConnect *connect;
  };
};
#define ORDERER__KAFKA_MESSAGE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&orderer__kafka_message__descriptor) \
    , ORDERER__KAFKA_MESSAGE__TYPE__NOT_SET, {0} }


/*
 * KafkaMessageRegular wraps a marshalled envelope.
 */
struct  _Orderer__KafkaMessageRegular
{
  ProtobufCMessage base;
  protobuf_c_boolean has_payload;
  ProtobufCBinaryData payload;
  protobuf_c_boolean has_config_seq;
  uint64_t config_seq;
  protobuf_c_boolean has_class_;
  Orderer__KafkaMessageRegular__Class class_;
  protobuf_c_boolean has_original_offset;
  int64_t original_offset;
};
#define ORDERER__KAFKA_MESSAGE_REGULAR__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&orderer__kafka_message_regular__descriptor) \
    , 0,{0,NULL}, 0,0, 0,0, 0,0 }


/*
 * KafkaMessageTimeToCut is used to signal to the orderers
 * that it is time to cut block <block_number>.
 */
struct  _Orderer__KafkaMessageTimeToCut
{
  ProtobufCMessage base;
  protobuf_c_boolean has_block_number;
  uint64_t block_number;
};
#define ORDERER__KAFKA_MESSAGE_TIME_TO_CUT__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&orderer__kafka_message_time_to_cut__descriptor) \
    , 0,0 }


/*
 * KafkaMessageConnect is posted by an orderer upon booting up.
 * It is used to prevent the panic that would be caused if we
 * were to consume an empty partition. It is ignored by all
 * orderers when processing the partition.
 */
struct  _Orderer__KafkaMessageConnect
{
  ProtobufCMessage base;
  protobuf_c_boolean has_payload;
  ProtobufCBinaryData payload;
};
#define ORDERER__KAFKA_MESSAGE_CONNECT__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&orderer__kafka_message_connect__descriptor) \
    , 0,{0,NULL} }


/*
 * KafkaMetadata is encoded into the ORDERER block to keep track of
 * Kafka-related metadata associated with this block.
 */
struct  _Orderer__KafkaMetadata
{
  ProtobufCMessage base;
  /*
   * LastOffsetPersisted is the encoded value for the Metadata message
   * which is encoded in the ORDERER block metadata index for the case
   * of the Kafka-based orderer.
   */
  protobuf_c_boolean has_last_offset_persisted;
  int64_t last_offset_persisted;
  /*
   * LastOriginalOffsetProcessed is used to keep track of the newest
   * offset processed if a message is re-validated and re-ordered.
   * This value is used to deduplicate re-submitted messages from
   * multiple orderer so that we don't bother re-processing it again.
   */
  protobuf_c_boolean has_last_original_offset_processed;
  int64_t last_original_offset_processed;
  /*
   * LastResubmittedConfigOffset is used to capture the newest offset of
   * CONFIG kafka message, which is revalidated and resubmitted. By comparing
   * this with LastOriginalOffsetProcessed, we could detemine whether there
   * are still CONFIG messages that have been resubmitted but NOT processed
   * yet. It's used as condition to block ingress messages, so we could reduce
   * the overhead of repeatedly resubmitting messages as config seq keeps
   * advancing.
   */
  protobuf_c_boolean has_last_resubmitted_config_offset;
  int64_t last_resubmitted_config_offset;
};
#define ORDERER__KAFKA_METADATA__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&orderer__kafka_metadata__descriptor) \
    , 0,0, 0,0, 0,0 }


/* Orderer__KafkaMessage methods */
void   orderer__kafka_message__init
                     (Orderer__KafkaMessage         *message);
size_t orderer__kafka_message__get_packed_size
                     (const Orderer__KafkaMessage   *message);
size_t orderer__kafka_message__pack
                     (const Orderer__KafkaMessage   *message,
                      uint8_t             *out);
size_t orderer__kafka_message__pack_to_buffer
                     (const Orderer__KafkaMessage   *message,
                      ProtobufCBuffer     *buffer);
Orderer__KafkaMessage *
       orderer__kafka_message__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   orderer__kafka_message__free_unpacked
                     (Orderer__KafkaMessage *message,
                      ProtobufCAllocator *allocator);
/* Orderer__KafkaMessageRegular methods */
void   orderer__kafka_message_regular__init
                     (Orderer__KafkaMessageRegular         *message);
size_t orderer__kafka_message_regular__get_packed_size
                     (const Orderer__KafkaMessageRegular   *message);
size_t orderer__kafka_message_regular__pack
                     (const Orderer__KafkaMessageRegular   *message,
                      uint8_t             *out);
size_t orderer__kafka_message_regular__pack_to_buffer
                     (const Orderer__KafkaMessageRegular   *message,
                      ProtobufCBuffer     *buffer);
Orderer__KafkaMessageRegular *
       orderer__kafka_message_regular__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   orderer__kafka_message_regular__free_unpacked
                     (Orderer__KafkaMessageRegular *message,
                      ProtobufCAllocator *allocator);
/* Orderer__KafkaMessageTimeToCut methods */
void   orderer__kafka_message_time_to_cut__init
                     (Orderer__KafkaMessageTimeToCut         *message);
size_t orderer__kafka_message_time_to_cut__get_packed_size
                     (const Orderer__KafkaMessageTimeToCut   *message);
size_t orderer__kafka_message_time_to_cut__pack
                     (const Orderer__KafkaMessageTimeToCut   *message,
                      uint8_t             *out);
size_t orderer__kafka_message_time_to_cut__pack_to_buffer
                     (const Orderer__KafkaMessageTimeToCut   *message,
                      ProtobufCBuffer     *buffer);
Orderer__KafkaMessageTimeToCut *
       orderer__kafka_message_time_to_cut__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   orderer__kafka_message_time_to_cut__free_unpacked
                     (Orderer__KafkaMessageTimeToCut *message,
                      ProtobufCAllocator *allocator);
/* Orderer__KafkaMessageConnect methods */
void   orderer__kafka_message_connect__init
                     (Orderer__KafkaMessageConnect         *message);
size_t orderer__kafka_message_connect__get_packed_size
                     (const Orderer__KafkaMessageConnect   *message);
size_t orderer__kafka_message_connect__pack
                     (const Orderer__KafkaMessageConnect   *message,
                      uint8_t             *out);
size_t orderer__kafka_message_connect__pack_to_buffer
                     (const Orderer__KafkaMessageConnect   *message,
                      ProtobufCBuffer     *buffer);
Orderer__KafkaMessageConnect *
       orderer__kafka_message_connect__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   orderer__kafka_message_connect__free_unpacked
                     (Orderer__KafkaMessageConnect *message,
                      ProtobufCAllocator *allocator);
/* Orderer__KafkaMetadata methods */
void   orderer__kafka_metadata__init
                     (Orderer__KafkaMetadata         *message);
size_t orderer__kafka_metadata__get_packed_size
                     (const Orderer__KafkaMetadata   *message);
size_t orderer__kafka_metadata__pack
                     (const Orderer__KafkaMetadata   *message,
                      uint8_t             *out);
size_t orderer__kafka_metadata__pack_to_buffer
                     (const Orderer__KafkaMetadata   *message,
                      ProtobufCBuffer     *buffer);
Orderer__KafkaMetadata *
       orderer__kafka_metadata__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   orderer__kafka_metadata__free_unpacked
                     (Orderer__KafkaMetadata *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*Orderer__KafkaMessage_Closure)
                 (const Orderer__KafkaMessage *message,
                  void *closure_data);
typedef void (*Orderer__KafkaMessageRegular_Closure)
                 (const Orderer__KafkaMessageRegular *message,
                  void *closure_data);
typedef void (*Orderer__KafkaMessageTimeToCut_Closure)
                 (const Orderer__KafkaMessageTimeToCut *message,
                  void *closure_data);
typedef void (*Orderer__KafkaMessageConnect_Closure)
                 (const Orderer__KafkaMessageConnect *message,
                  void *closure_data);
typedef void (*Orderer__KafkaMetadata_Closure)
                 (const Orderer__KafkaMetadata *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor orderer__kafka_message__descriptor;
extern const ProtobufCMessageDescriptor orderer__kafka_message_regular__descriptor;
extern const ProtobufCEnumDescriptor    orderer__kafka_message_regular__class__descriptor;
extern const ProtobufCMessageDescriptor orderer__kafka_message_time_to_cut__descriptor;
extern const ProtobufCMessageDescriptor orderer__kafka_message_connect__descriptor;
extern const ProtobufCMessageDescriptor orderer__kafka_metadata__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_orderer_2fkafka_2eproto__INCLUDED */
