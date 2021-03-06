/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: common/common.proto */

#ifndef PROTOBUF_C_common_2fcommon_2eproto__INCLUDED
#define PROTOBUF_C_common_2fcommon_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1000000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1002001 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif

#include "google/protobuf/timestamp.pb-c.h"

typedef struct _Common__LastConfig Common__LastConfig;
typedef struct _Common__Metadata Common__Metadata;
typedef struct _Common__MetadataSignature Common__MetadataSignature;
typedef struct _Common__Header Common__Header;
typedef struct _Common__ChannelHeader Common__ChannelHeader;
typedef struct _Common__SignatureHeader Common__SignatureHeader;
typedef struct _Common__Payload Common__Payload;
typedef struct _Common__Envelope Common__Envelope;
typedef struct _Common__Block Common__Block;
typedef struct _Common__BlockHeader Common__BlockHeader;
typedef struct _Common__BlockData Common__BlockData;
typedef struct _Common__BlockMetadata Common__BlockMetadata;


/* --- enums --- */

/*
 * These status codes are intended to resemble selected HTTP status codes
 */
typedef enum _Common__Status {
  COMMON__STATUS__UNKNOWN = 0,
  COMMON__STATUS__SUCCESS = 200,
  COMMON__STATUS__BAD_REQUEST = 400,
  COMMON__STATUS__FORBIDDEN = 403,
  COMMON__STATUS__NOT_FOUND = 404,
  COMMON__STATUS__REQUEST_ENTITY_TOO_LARGE = 413,
  COMMON__STATUS__INTERNAL_SERVER_ERROR = 500,
  COMMON__STATUS__NOT_IMPLEMENTED = 501,
  COMMON__STATUS__SERVICE_UNAVAILABLE = 503
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(COMMON__STATUS)
} Common__Status;
typedef enum _Common__HeaderType {
  /*
   * Used for messages which are signed but opaque
   */
  COMMON__HEADER_TYPE__MESSAGE = 0,
  /*
   * Used for messages which express the channel config
   */
  COMMON__HEADER_TYPE__CONFIG = 1,
  /*
   * Used for transactions which update the channel config
   */
  COMMON__HEADER_TYPE__CONFIG_UPDATE = 2,
  /*
   * Used by the SDK to submit endorser based transactions
   */
  COMMON__HEADER_TYPE__ENDORSER_TRANSACTION = 3,
  /*
   * Used internally by the orderer for management
   */
  COMMON__HEADER_TYPE__ORDERER_TRANSACTION = 4,
  /*
   * Used as the type for Envelope messages submitted to instruct the Deliver API to seek
   */
  COMMON__HEADER_TYPE__DELIVER_SEEK_INFO = 5,
  /*
   * Used for packaging chaincode artifacts for install
   */
  COMMON__HEADER_TYPE__CHAINCODE_PACKAGE = 6,
  /*
   * Used for invoking an administrative operation on a peer
   */
  COMMON__HEADER_TYPE__PEER_ADMIN_OPERATION = 8,
  /*
   * Used to denote transactions that invoke token management operations
   */
  COMMON__HEADER_TYPE__TOKEN_TRANSACTION = 9
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(COMMON__HEADER_TYPE)
} Common__HeaderType;
/*
 * This enum enlists indexes of the block metadata array
 */
typedef enum _Common__BlockMetadataIndex {
  /*
   * Block metadata array position for block signatures
   */
  COMMON__BLOCK_METADATA_INDEX__SIGNATURES = 0,
  /*
   * Block metadata array position to store last configuration block sequence number
   */
  COMMON__BLOCK_METADATA_INDEX__LAST_CONFIG = 1,
  /*
   * Block metadata array position to store serialized bit array filter of invalid transactions
   */
  COMMON__BLOCK_METADATA_INDEX__TRANSACTIONS_FILTER = 2,
  /*
   * Block metadata array position to store operational metadata for orderers
   */
  COMMON__BLOCK_METADATA_INDEX__ORDERER = 3
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(COMMON__BLOCK_METADATA_INDEX)
} Common__BlockMetadataIndex;

/* --- messages --- */

/*
 * LastConfig is the encoded value for the Metadata message which is encoded in the LAST_CONFIGURATION block metadata index
 */
struct  _Common__LastConfig
{
  ProtobufCMessage base;
  protobuf_c_boolean has_index;
  uint64_t index;
};
#define COMMON__LAST_CONFIG__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&common__last_config__descriptor) \
    , 0,0 }


/*
 * Metadata is a common structure to be used to encode block metadata
 */
struct  _Common__Metadata
{
  ProtobufCMessage base;
  protobuf_c_boolean has_value;
  ProtobufCBinaryData value;
  size_t n_signatures;
  Common__MetadataSignature **signatures;
};
#define COMMON__METADATA__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&common__metadata__descriptor) \
    , 0,{0,NULL}, 0,NULL }


struct  _Common__MetadataSignature
{
  ProtobufCMessage base;
  /*
   * An encoded SignatureHeader
   */
  protobuf_c_boolean has_signature_header;
  ProtobufCBinaryData signature_header;
  /*
   * The signature over the concatenation of the Metadata value bytes, signatureHeader, and block header
   */
  protobuf_c_boolean has_signature;
  ProtobufCBinaryData signature;
};
#define COMMON__METADATA_SIGNATURE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&common__metadata_signature__descriptor) \
    , 0,{0,NULL}, 0,{0,NULL} }


struct  _Common__Header
{
  ProtobufCMessage base;
  protobuf_c_boolean has_channel_header;
  ProtobufCBinaryData channel_header;
  protobuf_c_boolean has_signature_header;
  ProtobufCBinaryData signature_header;
};
#define COMMON__HEADER__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&common__header__descriptor) \
    , 0,{0,NULL}, 0,{0,NULL} }


/*
 * Header is a generic replay prevention and identity message to include in a signed payload
 */
struct  _Common__ChannelHeader
{
  ProtobufCMessage base;
  /*
   * Header types 0-10000 are reserved and defined by HeaderType
   */
  protobuf_c_boolean has_type;
  int32_t type;
  /*
   * Version indicates message protocol version
   */
  protobuf_c_boolean has_version;
  int32_t version;
  /*
   * Timestamp is the local time when the message was created
   * by the sender
   */
  Google__Protobuf__Timestamp *timestamp;
  /*
   * Identifier of the channel this message is bound for
   */
  char *channel_id;
  /*
   * An unique identifier that is used end-to-end.
   *  -  set by higher layers such as end user or SDK
   *  -  passed to the endorser (which will check for uniqueness)
   *  -  as the header is passed along unchanged, it will be
   *     be retrieved by the committer (uniqueness check here as well)
   *  -  to be stored in the ledger
   */
  char *tx_id;
  /*
   * The epoch in which this header was generated, where epoch is defined based on block height
   * Epoch in which the response has been generated. This field identifies a
   * logical window of time. A proposal response is accepted by a peer only if
   * two conditions hold:
   * 1. the epoch specified in the message is the current epoch
   * 2. this message has been only seen once during this epoch (i.e. it hasn't
   *    been replayed)
   */
  protobuf_c_boolean has_epoch;
  uint64_t epoch;
  /*
   * Extension that may be attached based on the header type
   */
  protobuf_c_boolean has_extension;
  ProtobufCBinaryData extension;
  /*
   * If mutual TLS is employed, this represents
   * the hash of the client's TLS certificate
   */
  protobuf_c_boolean has_tls_cert_hash;
  ProtobufCBinaryData tls_cert_hash;
};
#define COMMON__CHANNEL_HEADER__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&common__channel_header__descriptor) \
    , 0,0, 0,0, NULL, NULL, NULL, 0,0, 0,{0,NULL}, 0,{0,NULL} }


struct  _Common__SignatureHeader
{
  ProtobufCMessage base;
  /*
   * Creator of the message, a marshaled msp.SerializedIdentity
   */
  protobuf_c_boolean has_creator;
  ProtobufCBinaryData creator;
  /*
   * Arbitrary number that may only be used once. Can be used to detect replay attacks.
   */
  protobuf_c_boolean has_nonce;
  ProtobufCBinaryData nonce;
};
#define COMMON__SIGNATURE_HEADER__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&common__signature_header__descriptor) \
    , 0,{0,NULL}, 0,{0,NULL} }


/*
 * Payload is the message contents (and header to allow for signing)
 */
struct  _Common__Payload
{
  ProtobufCMessage base;
  /*
   * Header is included to provide identity and prevent replay
   */
  Common__Header *header;
  /*
   * Data, the encoding of which is defined by the type in the header
   */
  protobuf_c_boolean has_data;
  ProtobufCBinaryData data;
};
#define COMMON__PAYLOAD__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&common__payload__descriptor) \
    , NULL, 0,{0,NULL} }


/*
 * Envelope wraps a Payload with a signature so that the message may be authenticated
 */
struct  _Common__Envelope
{
  ProtobufCMessage base;
  /*
   * A marshaled Payload
   */
  protobuf_c_boolean has_payload;
  ProtobufCBinaryData payload;
  /*
   * A signature by the creator specified in the Payload header
   */
  protobuf_c_boolean has_signature;
  ProtobufCBinaryData signature;
};
#define COMMON__ENVELOPE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&common__envelope__descriptor) \
    , 0,{0,NULL}, 0,{0,NULL} }


/*
 * This is finalized block structure to be shared among the orderer and peer
 * Note that the BlockHeader chains to the previous BlockHeader, and the BlockData hash is embedded
 * in the BlockHeader.  This makes it natural and obvious that the Data is included in the hash, but
 * the Metadata is not.
 */
struct  _Common__Block
{
  ProtobufCMessage base;
  Common__BlockHeader *header;
  Common__BlockData *data;
  Common__BlockMetadata *metadata;
};
#define COMMON__BLOCK__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&common__block__descriptor) \
    , NULL, NULL, NULL }


/*
 * BlockHeader is the element of the block which forms the block chain
 * The block header is hashed using the configured chain hashing algorithm
 * over the ASN.1 encoding of the BlockHeader
 */
struct  _Common__BlockHeader
{
  ProtobufCMessage base;
  /*
   * The position in the blockchain
   */
  protobuf_c_boolean has_number;
  uint64_t number;
  /*
   * The hash of the previous block header
   */
  protobuf_c_boolean has_previous_hash;
  ProtobufCBinaryData previous_hash;
  /*
   * The hash of the BlockData, by MerkleTree
   */
  protobuf_c_boolean has_data_hash;
  ProtobufCBinaryData data_hash;
};
#define COMMON__BLOCK_HEADER__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&common__block_header__descriptor) \
    , 0,0, 0,{0,NULL}, 0,{0,NULL} }


struct  _Common__BlockData
{
  ProtobufCMessage base;
  size_t n_data;
  ProtobufCBinaryData *data;
};
#define COMMON__BLOCK_DATA__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&common__block_data__descriptor) \
    , 0,NULL }


struct  _Common__BlockMetadata
{
  ProtobufCMessage base;
  size_t n_metadata;
  ProtobufCBinaryData *metadata;
};
#define COMMON__BLOCK_METADATA__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&common__block_metadata__descriptor) \
    , 0,NULL }


/* Common__LastConfig methods */
void   common__last_config__init
                     (Common__LastConfig         *message);
size_t common__last_config__get_packed_size
                     (const Common__LastConfig   *message);
size_t common__last_config__pack
                     (const Common__LastConfig   *message,
                      uint8_t             *out);
size_t common__last_config__pack_to_buffer
                     (const Common__LastConfig   *message,
                      ProtobufCBuffer     *buffer);
Common__LastConfig *
       common__last_config__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   common__last_config__free_unpacked
                     (Common__LastConfig *message,
                      ProtobufCAllocator *allocator);
/* Common__Metadata methods */
void   common__metadata__init
                     (Common__Metadata         *message);
size_t common__metadata__get_packed_size
                     (const Common__Metadata   *message);
size_t common__metadata__pack
                     (const Common__Metadata   *message,
                      uint8_t             *out);
size_t common__metadata__pack_to_buffer
                     (const Common__Metadata   *message,
                      ProtobufCBuffer     *buffer);
Common__Metadata *
       common__metadata__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   common__metadata__free_unpacked
                     (Common__Metadata *message,
                      ProtobufCAllocator *allocator);
/* Common__MetadataSignature methods */
void   common__metadata_signature__init
                     (Common__MetadataSignature         *message);
size_t common__metadata_signature__get_packed_size
                     (const Common__MetadataSignature   *message);
size_t common__metadata_signature__pack
                     (const Common__MetadataSignature   *message,
                      uint8_t             *out);
size_t common__metadata_signature__pack_to_buffer
                     (const Common__MetadataSignature   *message,
                      ProtobufCBuffer     *buffer);
Common__MetadataSignature *
       common__metadata_signature__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   common__metadata_signature__free_unpacked
                     (Common__MetadataSignature *message,
                      ProtobufCAllocator *allocator);
/* Common__Header methods */
void   common__header__init
                     (Common__Header         *message);
size_t common__header__get_packed_size
                     (const Common__Header   *message);
size_t common__header__pack
                     (const Common__Header   *message,
                      uint8_t             *out);
size_t common__header__pack_to_buffer
                     (const Common__Header   *message,
                      ProtobufCBuffer     *buffer);
Common__Header *
       common__header__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   common__header__free_unpacked
                     (Common__Header *message,
                      ProtobufCAllocator *allocator);
/* Common__ChannelHeader methods */
void   common__channel_header__init
                     (Common__ChannelHeader         *message);
size_t common__channel_header__get_packed_size
                     (const Common__ChannelHeader   *message);
size_t common__channel_header__pack
                     (const Common__ChannelHeader   *message,
                      uint8_t             *out);
size_t common__channel_header__pack_to_buffer
                     (const Common__ChannelHeader   *message,
                      ProtobufCBuffer     *buffer);
Common__ChannelHeader *
       common__channel_header__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   common__channel_header__free_unpacked
                     (Common__ChannelHeader *message,
                      ProtobufCAllocator *allocator);
/* Common__SignatureHeader methods */
void   common__signature_header__init
                     (Common__SignatureHeader         *message);
size_t common__signature_header__get_packed_size
                     (const Common__SignatureHeader   *message);
size_t common__signature_header__pack
                     (const Common__SignatureHeader   *message,
                      uint8_t             *out);
size_t common__signature_header__pack_to_buffer
                     (const Common__SignatureHeader   *message,
                      ProtobufCBuffer     *buffer);
Common__SignatureHeader *
       common__signature_header__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   common__signature_header__free_unpacked
                     (Common__SignatureHeader *message,
                      ProtobufCAllocator *allocator);
/* Common__Payload methods */
void   common__payload__init
                     (Common__Payload         *message);
size_t common__payload__get_packed_size
                     (const Common__Payload   *message);
size_t common__payload__pack
                     (const Common__Payload   *message,
                      uint8_t             *out);
size_t common__payload__pack_to_buffer
                     (const Common__Payload   *message,
                      ProtobufCBuffer     *buffer);
Common__Payload *
       common__payload__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   common__payload__free_unpacked
                     (Common__Payload *message,
                      ProtobufCAllocator *allocator);
/* Common__Envelope methods */
void   common__envelope__init
                     (Common__Envelope         *message);
size_t common__envelope__get_packed_size
                     (const Common__Envelope   *message);
size_t common__envelope__pack
                     (const Common__Envelope   *message,
                      uint8_t             *out);
size_t common__envelope__pack_to_buffer
                     (const Common__Envelope   *message,
                      ProtobufCBuffer     *buffer);
Common__Envelope *
       common__envelope__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   common__envelope__free_unpacked
                     (Common__Envelope *message,
                      ProtobufCAllocator *allocator);
/* Common__Block methods */
void   common__block__init
                     (Common__Block         *message);
size_t common__block__get_packed_size
                     (const Common__Block   *message);
size_t common__block__pack
                     (const Common__Block   *message,
                      uint8_t             *out);
size_t common__block__pack_to_buffer
                     (const Common__Block   *message,
                      ProtobufCBuffer     *buffer);
Common__Block *
       common__block__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   common__block__free_unpacked
                     (Common__Block *message,
                      ProtobufCAllocator *allocator);
/* Common__BlockHeader methods */
void   common__block_header__init
                     (Common__BlockHeader         *message);
size_t common__block_header__get_packed_size
                     (const Common__BlockHeader   *message);
size_t common__block_header__pack
                     (const Common__BlockHeader   *message,
                      uint8_t             *out);
size_t common__block_header__pack_to_buffer
                     (const Common__BlockHeader   *message,
                      ProtobufCBuffer     *buffer);
Common__BlockHeader *
       common__block_header__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   common__block_header__free_unpacked
                     (Common__BlockHeader *message,
                      ProtobufCAllocator *allocator);
/* Common__BlockData methods */
void   common__block_data__init
                     (Common__BlockData         *message);
size_t common__block_data__get_packed_size
                     (const Common__BlockData   *message);
size_t common__block_data__pack
                     (const Common__BlockData   *message,
                      uint8_t             *out);
size_t common__block_data__pack_to_buffer
                     (const Common__BlockData   *message,
                      ProtobufCBuffer     *buffer);
Common__BlockData *
       common__block_data__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   common__block_data__free_unpacked
                     (Common__BlockData *message,
                      ProtobufCAllocator *allocator);
/* Common__BlockMetadata methods */
void   common__block_metadata__init
                     (Common__BlockMetadata         *message);
size_t common__block_metadata__get_packed_size
                     (const Common__BlockMetadata   *message);
size_t common__block_metadata__pack
                     (const Common__BlockMetadata   *message,
                      uint8_t             *out);
size_t common__block_metadata__pack_to_buffer
                     (const Common__BlockMetadata   *message,
                      ProtobufCBuffer     *buffer);
Common__BlockMetadata *
       common__block_metadata__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   common__block_metadata__free_unpacked
                     (Common__BlockMetadata *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*Common__LastConfig_Closure)
                 (const Common__LastConfig *message,
                  void *closure_data);
typedef void (*Common__Metadata_Closure)
                 (const Common__Metadata *message,
                  void *closure_data);
typedef void (*Common__MetadataSignature_Closure)
                 (const Common__MetadataSignature *message,
                  void *closure_data);
typedef void (*Common__Header_Closure)
                 (const Common__Header *message,
                  void *closure_data);
typedef void (*Common__ChannelHeader_Closure)
                 (const Common__ChannelHeader *message,
                  void *closure_data);
typedef void (*Common__SignatureHeader_Closure)
                 (const Common__SignatureHeader *message,
                  void *closure_data);
typedef void (*Common__Payload_Closure)
                 (const Common__Payload *message,
                  void *closure_data);
typedef void (*Common__Envelope_Closure)
                 (const Common__Envelope *message,
                  void *closure_data);
typedef void (*Common__Block_Closure)
                 (const Common__Block *message,
                  void *closure_data);
typedef void (*Common__BlockHeader_Closure)
                 (const Common__BlockHeader *message,
                  void *closure_data);
typedef void (*Common__BlockData_Closure)
                 (const Common__BlockData *message,
                  void *closure_data);
typedef void (*Common__BlockMetadata_Closure)
                 (const Common__BlockMetadata *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCEnumDescriptor    common__status__descriptor;
extern const ProtobufCEnumDescriptor    common__header_type__descriptor;
extern const ProtobufCEnumDescriptor    common__block_metadata_index__descriptor;
extern const ProtobufCMessageDescriptor common__last_config__descriptor;
extern const ProtobufCMessageDescriptor common__metadata__descriptor;
extern const ProtobufCMessageDescriptor common__metadata_signature__descriptor;
extern const ProtobufCMessageDescriptor common__header__descriptor;
extern const ProtobufCMessageDescriptor common__channel_header__descriptor;
extern const ProtobufCMessageDescriptor common__signature_header__descriptor;
extern const ProtobufCMessageDescriptor common__payload__descriptor;
extern const ProtobufCMessageDescriptor common__envelope__descriptor;
extern const ProtobufCMessageDescriptor common__block__descriptor;
extern const ProtobufCMessageDescriptor common__block_header__descriptor;
extern const ProtobufCMessageDescriptor common__block_data__descriptor;
extern const ProtobufCMessageDescriptor common__block_metadata__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_common_2fcommon_2eproto__INCLUDED */
