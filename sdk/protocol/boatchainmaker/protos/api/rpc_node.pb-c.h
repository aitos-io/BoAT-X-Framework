/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: api/rpc_node.proto */

#ifndef PROTOBUF_C_api_2frpc_5fnode_2eproto__INCLUDED
#define PROTOBUF_C_api_2frpc_5fnode_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1003000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1003003 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif

#include "common/request.pb-c.h"
#include "common/result.pb-c.h"
#include "config/local_config.pb-c.h"
#include "config/log_config.pb-c.h"
#include "config/chainmaker_server.pb-c.h"



/* --- enums --- */


/* --- messages --- */

/* --- per-message closures --- */


/* --- services --- */

typedef struct _Api__RpcNode_Service Api__RpcNode_Service;
struct _Api__RpcNode_Service
{
  ProtobufCService base;
  void (*send_request)(Api__RpcNode_Service *service,
                       const Common__TxRequest *input,
                       Common__TxResponse_Closure closure,
                       void *closure_data);
  void (*subscribe)(Api__RpcNode_Service *service,
                    const Common__TxRequest *input,
                    Common__SubscribeResult_Closure closure,
                    void *closure_data);
  void (*update_debug_config)(Api__RpcNode_Service *service,
                              const Config__DebugConfigRequest *input,
                              Config__DebugConfigResponse_Closure closure,
                              void *closure_data);
  void (*refresh_log_levels_config)(Api__RpcNode_Service *service,
                                    const Config__LogLevelsRequest *input,
                                    Config__LogLevelsResponse_Closure closure,
                                    void *closure_data);
  void (*get_chain_maker_version)(Api__RpcNode_Service *service,
                                  const Config__ChainMakerVersionRequest *input,
                                  Config__ChainMakerVersionResponse_Closure closure,
                                  void *closure_data);
  void (*check_new_block_chain_config)(Api__RpcNode_Service *service,
                                       const Config__CheckNewBlockChainConfigRequest *input,
                                       Config__CheckNewBlockChainConfigResponse_Closure closure,
                                       void *closure_data);
};
typedef void (*Api__RpcNode_ServiceDestroy)(Api__RpcNode_Service *);
void api__rpc_node__init (Api__RpcNode_Service *service,
                          Api__RpcNode_ServiceDestroy destroy);
#define API__RPC_NODE__BASE_INIT \
    { &api__rpc_node__descriptor, protobuf_c_service_invoke_internal, NULL }
#define API__RPC_NODE__INIT(function_prefix__) \
    { API__RPC_NODE__BASE_INIT,\
      function_prefix__ ## send_request,\
      function_prefix__ ## subscribe,\
      function_prefix__ ## update_debug_config,\
      function_prefix__ ## refresh_log_levels_config,\
      function_prefix__ ## get_chain_maker_version,\
      function_prefix__ ## check_new_block_chain_config  }
void api__rpc_node__send_request(ProtobufCService *service,
                                 const Common__TxRequest *input,
                                 Common__TxResponse_Closure closure,
                                 void *closure_data);
void api__rpc_node__subscribe(ProtobufCService *service,
                              const Common__TxRequest *input,
                              Common__SubscribeResult_Closure closure,
                              void *closure_data);
void api__rpc_node__update_debug_config(ProtobufCService *service,
                                        const Config__DebugConfigRequest *input,
                                        Config__DebugConfigResponse_Closure closure,
                                        void *closure_data);
void api__rpc_node__refresh_log_levels_config(ProtobufCService *service,
                                              const Config__LogLevelsRequest *input,
                                              Config__LogLevelsResponse_Closure closure,
                                              void *closure_data);
void api__rpc_node__get_chain_maker_version(ProtobufCService *service,
                                            const Config__ChainMakerVersionRequest *input,
                                            Config__ChainMakerVersionResponse_Closure closure,
                                            void *closure_data);
void api__rpc_node__check_new_block_chain_config(ProtobufCService *service,
                                                 const Config__CheckNewBlockChainConfigRequest *input,
                                                 Config__CheckNewBlockChainConfigResponse_Closure closure,
                                                 void *closure_data);

/* --- descriptors --- */

extern const ProtobufCServiceDescriptor api__rpc_node__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_api_2frpc_5fnode_2eproto__INCLUDED */