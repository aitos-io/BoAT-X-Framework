/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: ledger/rwset/rwset.proto */

#ifndef PROTOBUF_C_ledger_2frwset_2frwset_2eproto__INCLUDED
#define PROTOBUF_C_ledger_2frwset_2frwset_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1000000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1002001 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _Rwset__TxReadWriteSet Rwset__TxReadWriteSet;
typedef struct _Rwset__NsReadWriteSet Rwset__NsReadWriteSet;
typedef struct _Rwset__CollectionHashedReadWriteSet Rwset__CollectionHashedReadWriteSet;
typedef struct _Rwset__TxPvtReadWriteSet Rwset__TxPvtReadWriteSet;
typedef struct _Rwset__NsPvtReadWriteSet Rwset__NsPvtReadWriteSet;
typedef struct _Rwset__CollectionPvtReadWriteSet Rwset__CollectionPvtReadWriteSet;


/* --- enums --- */

typedef enum _Rwset__TxReadWriteSet__DataModel {
  RWSET__TX_READ_WRITE_SET__DATA_MODEL__KV = 0
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(RWSET__TX_READ_WRITE_SET__DATA_MODEL)
} Rwset__TxReadWriteSet__DataModel;

/* --- messages --- */

/*
 * TxReadWriteSet encapsulates a read-write set for a transaction
 * DataModel specifies the enum value of the data model
 * ns_rwset field specifies a list of chaincode specific read-write set (one for each chaincode)
 */
struct  _Rwset__TxReadWriteSet
{
  ProtobufCMessage base;
  protobuf_c_boolean has_data_model;
  Rwset__TxReadWriteSet__DataModel data_model;
  size_t n_ns_rwset;
  Rwset__NsReadWriteSet **ns_rwset;
};
#define RWSET__TX_READ_WRITE_SET__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&rwset__tx_read_write_set__descriptor) \
    , 0,0, 0,NULL }


/*
 * NsReadWriteSet encapsulates the read-write set for a chaincode
 */
struct  _Rwset__NsReadWriteSet
{
  ProtobufCMessage base;
  char *namespace_;
  /*
   * Data model specific serialized proto message (e.g., kvrwset.KVRWSet for KV and Document data models)
   */
  protobuf_c_boolean has_rwset;
  ProtobufCBinaryData rwset;
  size_t n_collection_hashed_rwset;
  Rwset__CollectionHashedReadWriteSet **collection_hashed_rwset;
};
#define RWSET__NS_READ_WRITE_SET__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&rwset__ns_read_write_set__descriptor) \
    , NULL, 0,{0,NULL}, 0,NULL }


/*
 * CollectionHashedReadWriteSet encapsulate the hashed representation for the private read-write set for a collection
 */
struct  _Rwset__CollectionHashedReadWriteSet
{
  ProtobufCMessage base;
  char *collection_name;
  /*
   * Data model specific serialized proto message (e.g., kvrwset.HashedRWSet for KV and Document data models)
   */
  protobuf_c_boolean has_hashed_rwset;
  ProtobufCBinaryData hashed_rwset;
  /*
   * Hash of entire private read-write set for a specific collection. This helps in authenticating the private read-write set efficiently
   */
  protobuf_c_boolean has_pvt_rwset_hash;
  ProtobufCBinaryData pvt_rwset_hash;
};
#define RWSET__COLLECTION_HASHED_READ_WRITE_SET__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&rwset__collection_hashed_read_write_set__descriptor) \
    , NULL, 0,{0,NULL}, 0,{0,NULL} }


/*
 * TxPvtReadWriteSet encapsulate the private read-write set for a transaction
 */
struct  _Rwset__TxPvtReadWriteSet
{
  ProtobufCMessage base;
  protobuf_c_boolean has_data_model;
  Rwset__TxReadWriteSet__DataModel data_model;
  size_t n_ns_pvt_rwset;
  Rwset__NsPvtReadWriteSet **ns_pvt_rwset;
};
#define RWSET__TX_PVT_READ_WRITE_SET__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&rwset__tx_pvt_read_write_set__descriptor) \
    , 0,0, 0,NULL }


/*
 * NsPvtReadWriteSet encapsulates the private read-write set for a chaincode
 */
struct  _Rwset__NsPvtReadWriteSet
{
  ProtobufCMessage base;
  char *namespace_;
  size_t n_collection_pvt_rwset;
  Rwset__CollectionPvtReadWriteSet **collection_pvt_rwset;
};
#define RWSET__NS_PVT_READ_WRITE_SET__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&rwset__ns_pvt_read_write_set__descriptor) \
    , NULL, 0,NULL }


/*
 * CollectionPvtReadWriteSet encapsulates the private read-write set for a collection
 */
struct  _Rwset__CollectionPvtReadWriteSet
{
  ProtobufCMessage base;
  char *collection_name;
  /*
   * Data model specific serialized proto message (e.g., kvrwset.KVRWSet for KV and Document data models)
   */
  protobuf_c_boolean has_rwset;
  ProtobufCBinaryData rwset;
};
#define RWSET__COLLECTION_PVT_READ_WRITE_SET__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&rwset__collection_pvt_read_write_set__descriptor) \
    , NULL, 0,{0,NULL} }


/* Rwset__TxReadWriteSet methods */
void   rwset__tx_read_write_set__init
                     (Rwset__TxReadWriteSet         *message);
size_t rwset__tx_read_write_set__get_packed_size
                     (const Rwset__TxReadWriteSet   *message);
size_t rwset__tx_read_write_set__pack
                     (const Rwset__TxReadWriteSet   *message,
                      uint8_t             *out);
size_t rwset__tx_read_write_set__pack_to_buffer
                     (const Rwset__TxReadWriteSet   *message,
                      ProtobufCBuffer     *buffer);
Rwset__TxReadWriteSet *
       rwset__tx_read_write_set__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   rwset__tx_read_write_set__free_unpacked
                     (Rwset__TxReadWriteSet *message,
                      ProtobufCAllocator *allocator);
/* Rwset__NsReadWriteSet methods */
void   rwset__ns_read_write_set__init
                     (Rwset__NsReadWriteSet         *message);
size_t rwset__ns_read_write_set__get_packed_size
                     (const Rwset__NsReadWriteSet   *message);
size_t rwset__ns_read_write_set__pack
                     (const Rwset__NsReadWriteSet   *message,
                      uint8_t             *out);
size_t rwset__ns_read_write_set__pack_to_buffer
                     (const Rwset__NsReadWriteSet   *message,
                      ProtobufCBuffer     *buffer);
Rwset__NsReadWriteSet *
       rwset__ns_read_write_set__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   rwset__ns_read_write_set__free_unpacked
                     (Rwset__NsReadWriteSet *message,
                      ProtobufCAllocator *allocator);
/* Rwset__CollectionHashedReadWriteSet methods */
void   rwset__collection_hashed_read_write_set__init
                     (Rwset__CollectionHashedReadWriteSet         *message);
size_t rwset__collection_hashed_read_write_set__get_packed_size
                     (const Rwset__CollectionHashedReadWriteSet   *message);
size_t rwset__collection_hashed_read_write_set__pack
                     (const Rwset__CollectionHashedReadWriteSet   *message,
                      uint8_t             *out);
size_t rwset__collection_hashed_read_write_set__pack_to_buffer
                     (const Rwset__CollectionHashedReadWriteSet   *message,
                      ProtobufCBuffer     *buffer);
Rwset__CollectionHashedReadWriteSet *
       rwset__collection_hashed_read_write_set__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   rwset__collection_hashed_read_write_set__free_unpacked
                     (Rwset__CollectionHashedReadWriteSet *message,
                      ProtobufCAllocator *allocator);
/* Rwset__TxPvtReadWriteSet methods */
void   rwset__tx_pvt_read_write_set__init
                     (Rwset__TxPvtReadWriteSet         *message);
size_t rwset__tx_pvt_read_write_set__get_packed_size
                     (const Rwset__TxPvtReadWriteSet   *message);
size_t rwset__tx_pvt_read_write_set__pack
                     (const Rwset__TxPvtReadWriteSet   *message,
                      uint8_t             *out);
size_t rwset__tx_pvt_read_write_set__pack_to_buffer
                     (const Rwset__TxPvtReadWriteSet   *message,
                      ProtobufCBuffer     *buffer);
Rwset__TxPvtReadWriteSet *
       rwset__tx_pvt_read_write_set__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   rwset__tx_pvt_read_write_set__free_unpacked
                     (Rwset__TxPvtReadWriteSet *message,
                      ProtobufCAllocator *allocator);
/* Rwset__NsPvtReadWriteSet methods */
void   rwset__ns_pvt_read_write_set__init
                     (Rwset__NsPvtReadWriteSet         *message);
size_t rwset__ns_pvt_read_write_set__get_packed_size
                     (const Rwset__NsPvtReadWriteSet   *message);
size_t rwset__ns_pvt_read_write_set__pack
                     (const Rwset__NsPvtReadWriteSet   *message,
                      uint8_t             *out);
size_t rwset__ns_pvt_read_write_set__pack_to_buffer
                     (const Rwset__NsPvtReadWriteSet   *message,
                      ProtobufCBuffer     *buffer);
Rwset__NsPvtReadWriteSet *
       rwset__ns_pvt_read_write_set__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   rwset__ns_pvt_read_write_set__free_unpacked
                     (Rwset__NsPvtReadWriteSet *message,
                      ProtobufCAllocator *allocator);
/* Rwset__CollectionPvtReadWriteSet methods */
void   rwset__collection_pvt_read_write_set__init
                     (Rwset__CollectionPvtReadWriteSet         *message);
size_t rwset__collection_pvt_read_write_set__get_packed_size
                     (const Rwset__CollectionPvtReadWriteSet   *message);
size_t rwset__collection_pvt_read_write_set__pack
                     (const Rwset__CollectionPvtReadWriteSet   *message,
                      uint8_t             *out);
size_t rwset__collection_pvt_read_write_set__pack_to_buffer
                     (const Rwset__CollectionPvtReadWriteSet   *message,
                      ProtobufCBuffer     *buffer);
Rwset__CollectionPvtReadWriteSet *
       rwset__collection_pvt_read_write_set__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   rwset__collection_pvt_read_write_set__free_unpacked
                     (Rwset__CollectionPvtReadWriteSet *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*Rwset__TxReadWriteSet_Closure)
                 (const Rwset__TxReadWriteSet *message,
                  void *closure_data);
typedef void (*Rwset__NsReadWriteSet_Closure)
                 (const Rwset__NsReadWriteSet *message,
                  void *closure_data);
typedef void (*Rwset__CollectionHashedReadWriteSet_Closure)
                 (const Rwset__CollectionHashedReadWriteSet *message,
                  void *closure_data);
typedef void (*Rwset__TxPvtReadWriteSet_Closure)
                 (const Rwset__TxPvtReadWriteSet *message,
                  void *closure_data);
typedef void (*Rwset__NsPvtReadWriteSet_Closure)
                 (const Rwset__NsPvtReadWriteSet *message,
                  void *closure_data);
typedef void (*Rwset__CollectionPvtReadWriteSet_Closure)
                 (const Rwset__CollectionPvtReadWriteSet *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor rwset__tx_read_write_set__descriptor;
extern const ProtobufCEnumDescriptor    rwset__tx_read_write_set__data_model__descriptor;
extern const ProtobufCMessageDescriptor rwset__ns_read_write_set__descriptor;
extern const ProtobufCMessageDescriptor rwset__collection_hashed_read_write_set__descriptor;
extern const ProtobufCMessageDescriptor rwset__tx_pvt_read_write_set__descriptor;
extern const ProtobufCMessageDescriptor rwset__ns_pvt_read_write_set__descriptor;
extern const ProtobufCMessageDescriptor rwset__collection_pvt_read_write_set__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_ledger_2frwset_2frwset_2eproto__INCLUDED */
