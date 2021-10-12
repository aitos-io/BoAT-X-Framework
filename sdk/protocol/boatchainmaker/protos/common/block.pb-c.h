/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: common/block.proto */

#ifndef PROTOBUF_C_common_2fblock_2eproto__INCLUDED
#define PROTOBUF_C_common_2fblock_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1003000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1003003 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif

#include "common/transaction.pb-c.h"
#include "common/rwset.pb-c.h"

typedef struct _Common__Block Common__Block;
typedef struct _Common__BlockInfo Common__BlockInfo;
typedef struct _Common__AdditionalData Common__AdditionalData;
typedef struct _Common__AdditionalData__ExtraDataEntry Common__AdditionalData__ExtraDataEntry;
typedef struct _Common__BlockHeader Common__BlockHeader;
typedef struct _Common__DAG Common__DAG;
typedef struct _Common__DAG__Neighbor Common__DAG__Neighbor;


/* --- enums --- */


/* --- messages --- */

/*
 * Block definition
 */
struct  _Common__Block
{
  ProtobufCMessage base;
  /*
   * header of the block
   */
  Common__BlockHeader *header;
  /*
   * execution sequence of intra block transactions is generated by proposer
   */
  /*
   * 本块交易的DAG
   */
  Common__DAG *dag;
  size_t n_txs;
  Common__Transaction **txs;
  /*
   * stores the voting information of the current block
   * not included in block hash value calculation
   */
  Common__AdditionalData *additional_data;
};
#define COMMON__BLOCK__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&common__block__descriptor) \
    , NULL, NULL, 0,NULL, NULL }


/*
 * block information
 */
struct  _Common__BlockInfo
{
  ProtobufCMessage base;
  /*
   * block
   */
  Common__Block *block;
  /*
   * The read / write set list corresponding to the transaction contained in the block
   */
  size_t n_rwset_list;
  Common__TxRWSet **rwset_list;
};
#define COMMON__BLOCK_INFO__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&common__block_info__descriptor) \
    , NULL, 0,NULL }


struct  _Common__AdditionalData__ExtraDataEntry
{
  ProtobufCMessage base;
  char *key;
  ProtobufCBinaryData value;
};
#define COMMON__ADDITIONAL_DATA__EXTRA_DATA_ENTRY__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&common__additional_data__extra_data_entry__descriptor) \
    , (char *)protobuf_c_empty_string, {0,NULL} }


/*
 * block additional data
 */
struct  _Common__AdditionalData
{
  ProtobufCMessage base;
  /*
   * extra data, with map type, excluded in hash calculation
   */
  size_t n_extra_data;
  Common__AdditionalData__ExtraDataEntry **extra_data;
};
#define COMMON__ADDITIONAL_DATA__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&common__additional_data__descriptor) \
    , 0,NULL }


/*
 * header of the block
 */
struct  _Common__BlockHeader
{
  ProtobufCMessage base;
  /*
   * blockchain identifier
   */
  char *chain_id;
  /*
   * block height
   */
  int64_t block_height;
  /*
   * previous block hash
   */
  ProtobufCBinaryData pre_block_hash;
  /*
   * block hash (block identifier)
   */
  /*
   * 本块哈希（块标识），除block_hash和signature以外，其余字段参与block_hash计算
   */
  ProtobufCBinaryData block_hash;
  /*
   * previous config block height, used to trace anc check if chain config is valid
   */
  int64_t pre_conf_height;
  /*
   * block version
   */
  ProtobufCBinaryData block_version;
  /*
   * Save the DAG feature summary, and hash the DAG after Pb serialization
   * hash of serialized DAG
   */
  ProtobufCBinaryData dag_hash;
  /*
   * The root hash of Merkle tree generated by read_write_set_digest in the result of each transaction in the block
   * used to verify the read-write set of the block
   */
  ProtobufCBinaryData rw_set_root;
  /*
   * merkle root of transactions
   * used to verify the existence of this transactions
   */
  ProtobufCBinaryData tx_root;
  /*
   * the time stamp of the block
   */
  int64_t block_timestamp;
  /*
   * proposal node identifier
   */
  ProtobufCBinaryData proposer;
  /*
   * consensus parameters
   * used to store information include in block hash calculation
   */
  ProtobufCBinaryData consensus_args;
  /*
   * count of transactions
   */
  int64_t tx_count;
  /*
   * signature of proposer
   */
  ProtobufCBinaryData signature;
};
#define COMMON__BLOCK_HEADER__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&common__block_header__descriptor) \
    , (char *)protobuf_c_empty_string, 0, {0,NULL}, {0,NULL}, 0, {0,NULL}, {0,NULL}, {0,NULL}, {0,NULL}, 0, {0,NULL}, {0,NULL}, 0, {0,NULL} }


/*
 * Neighbor node: related party transactions with reading and writing conflicts
 */
struct  _Common__DAG__Neighbor
{
  ProtobufCMessage base;
  size_t n_neighbors;
  int32_t *neighbors;
};
#define COMMON__DAG__NEIGHBOR__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&common__dag__neighbor__descriptor) \
    , 0,NULL }


/*
 * transaction execution sequence
 * Using adjacency table storage
 */
struct  _Common__DAG
{
  ProtobufCMessage base;
  /*
   * sequence number of transaction topological sort
   *the sequence number of the transaction topological sort associated with the transaction
   */
  size_t n_vertexes;
  Common__DAG__Neighbor **vertexes;
};
#define COMMON__DAG__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&common__dag__descriptor) \
    , 0,NULL }


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
/* Common__BlockInfo methods */
void   common__block_info__init
                     (Common__BlockInfo         *message);
size_t common__block_info__get_packed_size
                     (const Common__BlockInfo   *message);
size_t common__block_info__pack
                     (const Common__BlockInfo   *message,
                      uint8_t             *out);
size_t common__block_info__pack_to_buffer
                     (const Common__BlockInfo   *message,
                      ProtobufCBuffer     *buffer);
Common__BlockInfo *
       common__block_info__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   common__block_info__free_unpacked
                     (Common__BlockInfo *message,
                      ProtobufCAllocator *allocator);
/* Common__AdditionalData__ExtraDataEntry methods */
void   common__additional_data__extra_data_entry__init
                     (Common__AdditionalData__ExtraDataEntry         *message);
/* Common__AdditionalData methods */
void   common__additional_data__init
                     (Common__AdditionalData         *message);
size_t common__additional_data__get_packed_size
                     (const Common__AdditionalData   *message);
size_t common__additional_data__pack
                     (const Common__AdditionalData   *message,
                      uint8_t             *out);
size_t common__additional_data__pack_to_buffer
                     (const Common__AdditionalData   *message,
                      ProtobufCBuffer     *buffer);
Common__AdditionalData *
       common__additional_data__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   common__additional_data__free_unpacked
                     (Common__AdditionalData *message,
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
/* Common__DAG__Neighbor methods */
void   common__dag__neighbor__init
                     (Common__DAG__Neighbor         *message);
/* Common__DAG methods */
void   common__dag__init
                     (Common__DAG         *message);
size_t common__dag__get_packed_size
                     (const Common__DAG   *message);
size_t common__dag__pack
                     (const Common__DAG   *message,
                      uint8_t             *out);
size_t common__dag__pack_to_buffer
                     (const Common__DAG   *message,
                      ProtobufCBuffer     *buffer);
Common__DAG *
       common__dag__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   common__dag__free_unpacked
                     (Common__DAG *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*Common__Block_Closure)
                 (const Common__Block *message,
                  void *closure_data);
typedef void (*Common__BlockInfo_Closure)
                 (const Common__BlockInfo *message,
                  void *closure_data);
typedef void (*Common__AdditionalData__ExtraDataEntry_Closure)
                 (const Common__AdditionalData__ExtraDataEntry *message,
                  void *closure_data);
typedef void (*Common__AdditionalData_Closure)
                 (const Common__AdditionalData *message,
                  void *closure_data);
typedef void (*Common__BlockHeader_Closure)
                 (const Common__BlockHeader *message,
                  void *closure_data);
typedef void (*Common__DAG__Neighbor_Closure)
                 (const Common__DAG__Neighbor *message,
                  void *closure_data);
typedef void (*Common__DAG_Closure)
                 (const Common__DAG *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor common__block__descriptor;
extern const ProtobufCMessageDescriptor common__block_info__descriptor;
extern const ProtobufCMessageDescriptor common__additional_data__descriptor;
extern const ProtobufCMessageDescriptor common__additional_data__extra_data_entry__descriptor;
extern const ProtobufCMessageDescriptor common__block_header__descriptor;
extern const ProtobufCMessageDescriptor common__dag__descriptor;
extern const ProtobufCMessageDescriptor common__dag__neighbor__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_common_2fblock_2eproto__INCLUDED */
