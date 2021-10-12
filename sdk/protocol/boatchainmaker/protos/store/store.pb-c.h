/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: store/store.proto */

#ifndef PROTOBUF_C_store_2fstore_2eproto__INCLUDED
#define PROTOBUF_C_store_2fstore_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1003000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1003003 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif

#include "common/block.pb-c.h"
#include "common/rwset.pb-c.h"
#include "common/result.pb-c.h"

typedef struct _Store__SerializedBlock Store__SerializedBlock;
typedef struct _Store__BlockWithRWSet Store__BlockWithRWSet;


/* --- enums --- */

typedef enum _Store__DbType {
  STORE__DB_TYPE__INVALID_DB = 0,
  STORE__DB_TYPE__BLOCK_DB = 1,
  STORE__DB_TYPE__BLOCK_INDEX_DB = 2,
  STORE__DB_TYPE__TX_DB = 3,
  STORE__DB_TYPE__TX_INDEX_DB = 4,
  STORE__DB_TYPE__SOFT_DB = 5,
  STORE__DB_TYPE__STATE_DB = 6,
  STORE__DB_TYPE__READ_WRITE_DB = 7
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(STORE__DB_TYPE)
} Store__DbType;

/* --- messages --- */

/*
 * block structure used in serialization
 */
struct  _Store__SerializedBlock
{
  ProtobufCMessage base;
  /*
   * header of block
   */
  Common__BlockHeader *header;
  /*
   * transaction execution sequence of the block, described by DAG
   */
  Common__DAG *dag;
  /*
   * transaction id list within the block
   */
  size_t n_tx_ids;
  char **tx_ids;
  /*
   * block additional data, not included in block hash calculation
   */
  Common__AdditionalData *additional_data;
};
#define STORE__SERIALIZED_BLOCK__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&store__serialized_block__descriptor) \
    , NULL, NULL, 0,NULL, NULL }


/*
 * Block and its read/write set information
 */
struct  _Store__BlockWithRWSet
{
  ProtobufCMessage base;
  /*
   * block data
   */
  Common__Block *block;
  /*
   * transaction read/write set of blocks
   */
  size_t n_txrwsets;
  Common__TxRWSet **txrwsets;
  /*
   * contract event info
   */
  size_t n_contract_events;
  Common__ContractEvent **contract_events;
};
#define STORE__BLOCK_WITH_RWSET__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&store__block_with_rwset__descriptor) \
    , NULL, 0,NULL, 0,NULL }


/* Store__SerializedBlock methods */
void   store__serialized_block__init
                     (Store__SerializedBlock         *message);
size_t store__serialized_block__get_packed_size
                     (const Store__SerializedBlock   *message);
size_t store__serialized_block__pack
                     (const Store__SerializedBlock   *message,
                      uint8_t             *out);
size_t store__serialized_block__pack_to_buffer
                     (const Store__SerializedBlock   *message,
                      ProtobufCBuffer     *buffer);
Store__SerializedBlock *
       store__serialized_block__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   store__serialized_block__free_unpacked
                     (Store__SerializedBlock *message,
                      ProtobufCAllocator *allocator);
/* Store__BlockWithRWSet methods */
void   store__block_with_rwset__init
                     (Store__BlockWithRWSet         *message);
size_t store__block_with_rwset__get_packed_size
                     (const Store__BlockWithRWSet   *message);
size_t store__block_with_rwset__pack
                     (const Store__BlockWithRWSet   *message,
                      uint8_t             *out);
size_t store__block_with_rwset__pack_to_buffer
                     (const Store__BlockWithRWSet   *message,
                      ProtobufCBuffer     *buffer);
Store__BlockWithRWSet *
       store__block_with_rwset__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   store__block_with_rwset__free_unpacked
                     (Store__BlockWithRWSet *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*Store__SerializedBlock_Closure)
                 (const Store__SerializedBlock *message,
                  void *closure_data);
typedef void (*Store__BlockWithRWSet_Closure)
                 (const Store__BlockWithRWSet *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCEnumDescriptor    store__db_type__descriptor;
extern const ProtobufCMessageDescriptor store__serialized_block__descriptor;
extern const ProtobufCMessageDescriptor store__block_with_rwset__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_store_2fstore_2eproto__INCLUDED */
