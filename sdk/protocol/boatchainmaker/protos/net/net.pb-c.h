/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: net/net.proto */

#ifndef PROTOBUF_C_net_2fnet_2eproto__INCLUDED
#define PROTOBUF_C_net_2fnet_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1003000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1003003 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _Net__Msg Net__Msg;
typedef struct _Net__NetMsg Net__NetMsg;


/* --- enums --- */

/*
 * specific net message types
 */
typedef enum _Net__NetMsg__MsgType {
  NET__NET_MSG__MSG_TYPE__INVALID_MSG = 0,
  NET__NET_MSG__MSG_TYPE__TX = 1,
  NET__NET_MSG__MSG_TYPE__TXS = 2,
  NET__NET_MSG__MSG_TYPE__BLOCK = 3,
  NET__NET_MSG__MSG_TYPE__BLOCKS = 4,
  NET__NET_MSG__MSG_TYPE__CONSENSUS_MSG = 5,
  NET__NET_MSG__MSG_TYPE__SYNC_BLOCK_MSG = 6
    PROTOBUF_C__FORCE_ENUM_TO_BE_INT_SIZE(NET__NET_MSG__MSG_TYPE)
} Net__NetMsg__MsgType;

/* --- messages --- */

/*
 * wrapped network message
 */
struct  _Net__Msg
{
  ProtobufCMessage base;
  Net__NetMsg *msg;
  char *chain_id;
  /*
   * 属于那个模块，判断消息类型
   */
  char *flag;
};
#define NET__MSG__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&net__msg__descriptor) \
    , NULL, (char *)protobuf_c_empty_string, (char *)protobuf_c_empty_string }


/*
 * net message
 */
struct  _Net__NetMsg
{
  ProtobufCMessage base;
  /*
   * payload of the message
   */
  ProtobufCBinaryData payload;
  /*
   * message type
   */
  Net__NetMsg__MsgType type;
  /*
   * nodeId
   */
  char *to;
};
#define NET__NET_MSG__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&net__net_msg__descriptor) \
    , {0,NULL}, NET__NET_MSG__MSG_TYPE__INVALID_MSG, (char *)protobuf_c_empty_string }


/* Net__Msg methods */
void   net__msg__init
                     (Net__Msg         *message);
size_t net__msg__get_packed_size
                     (const Net__Msg   *message);
size_t net__msg__pack
                     (const Net__Msg   *message,
                      uint8_t             *out);
size_t net__msg__pack_to_buffer
                     (const Net__Msg   *message,
                      ProtobufCBuffer     *buffer);
Net__Msg *
       net__msg__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   net__msg__free_unpacked
                     (Net__Msg *message,
                      ProtobufCAllocator *allocator);
/* Net__NetMsg methods */
void   net__net_msg__init
                     (Net__NetMsg         *message);
size_t net__net_msg__get_packed_size
                     (const Net__NetMsg   *message);
size_t net__net_msg__pack
                     (const Net__NetMsg   *message,
                      uint8_t             *out);
size_t net__net_msg__pack_to_buffer
                     (const Net__NetMsg   *message,
                      ProtobufCBuffer     *buffer);
Net__NetMsg *
       net__net_msg__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   net__net_msg__free_unpacked
                     (Net__NetMsg *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*Net__Msg_Closure)
                 (const Net__Msg *message,
                  void *closure_data);
typedef void (*Net__NetMsg_Closure)
                 (const Net__NetMsg *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor net__msg__descriptor;
extern const ProtobufCMessageDescriptor net__net_msg__descriptor;
extern const ProtobufCEnumDescriptor    net__net_msg__msg_type__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_net_2fnet_2eproto__INCLUDED */