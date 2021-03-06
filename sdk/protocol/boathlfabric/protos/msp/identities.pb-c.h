/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: msp/identities.proto */

#ifndef PROTOBUF_C_msp_2fidentities_2eproto__INCLUDED
#define PROTOBUF_C_msp_2fidentities_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1000000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1002001 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct _Msp__SerializedIdentity Msp__SerializedIdentity;
typedef struct _Msp__SerializedIdemixIdentity Msp__SerializedIdemixIdentity;


/* --- enums --- */


/* --- messages --- */

/*
 * This struct represents an Identity
 * (with its MSP identifier) to be used
 * to serialize it and deserialize it
 */
struct  _Msp__SerializedIdentity
{
  ProtobufCMessage base;
  /*
   * The identifier of the associated membership service provider
   */
  char *mspid;
  /*
   * the Identity, serialized according to the rules of its MPS
   */
  protobuf_c_boolean has_id_bytes;
  ProtobufCBinaryData id_bytes;
};
#define MSP__SERIALIZED_IDENTITY__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&msp__serialized_identity__descriptor) \
    , NULL, 0,{0,NULL} }


/*
 * This struct represents an Idemix Identity
 * to be used to serialize it and deserialize it.
 * The IdemixMSP will first serialize an idemix identity to bytes using
 * this proto, and then uses these bytes as id_bytes in SerializedIdentity
 */
struct  _Msp__SerializedIdemixIdentity
{
  ProtobufCMessage base;
  /*
   * nym_x is the X-component of the pseudonym elliptic curve point.
   * It is a []byte representation of an amcl.BIG
   * The pseudonym can be seen as a public key of the identity, it is used to verify signatures.
   */
  protobuf_c_boolean has_nym_x;
  ProtobufCBinaryData nym_x;
  /*
   * nym_y is the Y-component of the pseudonym elliptic curve point.
   * It is a []byte representation of an amcl.BIG
   * The pseudonym can be seen as a public key of the identity, it is used to verify signatures.
   */
  protobuf_c_boolean has_nym_y;
  ProtobufCBinaryData nym_y;
  /*
   * ou contains the organizational unit of the idemix identity
   */
  protobuf_c_boolean has_ou;
  ProtobufCBinaryData ou;
  /*
   * role contains the role of this identity (e.g., ADMIN or MEMBER)
   */
  protobuf_c_boolean has_role;
  ProtobufCBinaryData role;
  /*
   * proof contains the cryptographic evidence that this identity is valid
   */
  protobuf_c_boolean has_proof;
  ProtobufCBinaryData proof;
};
#define MSP__SERIALIZED_IDEMIX_IDENTITY__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&msp__serialized_idemix_identity__descriptor) \
    , 0,{0,NULL}, 0,{0,NULL}, 0,{0,NULL}, 0,{0,NULL}, 0,{0,NULL} }


/* Msp__SerializedIdentity methods */
void   msp__serialized_identity__init
                     (Msp__SerializedIdentity         *message);
size_t msp__serialized_identity__get_packed_size
                     (const Msp__SerializedIdentity   *message);
size_t msp__serialized_identity__pack
                     (const Msp__SerializedIdentity   *message,
                      uint8_t             *out);
size_t msp__serialized_identity__pack_to_buffer
                     (const Msp__SerializedIdentity   *message,
                      ProtobufCBuffer     *buffer);
Msp__SerializedIdentity *
       msp__serialized_identity__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   msp__serialized_identity__free_unpacked
                     (Msp__SerializedIdentity *message,
                      ProtobufCAllocator *allocator);
/* Msp__SerializedIdemixIdentity methods */
void   msp__serialized_idemix_identity__init
                     (Msp__SerializedIdemixIdentity         *message);
size_t msp__serialized_idemix_identity__get_packed_size
                     (const Msp__SerializedIdemixIdentity   *message);
size_t msp__serialized_idemix_identity__pack
                     (const Msp__SerializedIdemixIdentity   *message,
                      uint8_t             *out);
size_t msp__serialized_idemix_identity__pack_to_buffer
                     (const Msp__SerializedIdemixIdentity   *message,
                      ProtobufCBuffer     *buffer);
Msp__SerializedIdemixIdentity *
       msp__serialized_idemix_identity__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   msp__serialized_idemix_identity__free_unpacked
                     (Msp__SerializedIdemixIdentity *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*Msp__SerializedIdentity_Closure)
                 (const Msp__SerializedIdentity *message,
                  void *closure_data);
typedef void (*Msp__SerializedIdemixIdentity_Closure)
                 (const Msp__SerializedIdemixIdentity *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor msp__serialized_identity__descriptor;
extern const ProtobufCMessageDescriptor msp__serialized_idemix_identity__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_msp_2fidentities_2eproto__INCLUDED */
