OPTFLAGS ?= -O3

# ifneq ($(BOAT_PROTOCOL_USE_ETHEREUM)_$(BOAT_PROTOCOL_USE_PLATONE)_$(BOAT_PROTOCOL_USE_VENACHAIN)_$(BOAT_PROTOCOL_USE_PLATON), 0_0_0_0)
    KECCAK_HASH_TYPE = 1
# endif
ifeq ($(BOAT_PROTOCOL_USE_HLFABRIC), 1)
    SHA256_HASH_TYPE = 1
endif
ifeq ($(BOAT_PROTOCOL_USE_CHAINMAKER_V1), 1)
    SHA256_HASH_TYPE = 1
endif
ifeq ($(BOAT_PROTOCOL_USE_CHAINMAKER_V2), 1)
    SHA256_HASH_TYPE = 1
endif
ifeq ($(BOAT_PROTOCOL_USE_HWBCS), 1)
    SHA256_HASH_TYPE = 1
endif

CRYPTO_CFLAGS   =  $(BOAT_CFLAGS) \
            -W \
            -Wall \
            -Wextra \
            -Wimplicit-function-declaration \
            -Wredundant-decls \
            -Wstrict-prototypes \
            -Wundef \
            -Wshadow \
            -Wpointer-arith \
            -Wformat \
            -Wreturn-type \
            -Wsign-compare \
            -Wmultichar \
            -Wformat-nonliteral \
            -Winit-self \
            -Wuninitialized \
            -Wformat-security \
            -Wno-error=missing-braces \
            -Wno-error=missing-field-initializers \
#            -Werror \
#            $(OPTFLAGS) \
#            -std=gnu99 \

VALGRIND ?= 1

CRYPTO_CFLAGS += -I.
CRYPTO_CFLAGS += -DNDEBUG  # Disable <assert.h>
CRYPTO_CFLAGS += -DVALGRIND=$(VALGRIND)
CRYPTO_CFLAGS += -DUSE_GRAPHENE=1

ifeq ($(KECCAK_HASH_TYPE), 1)
    CRYPTO_CFLAGS += -DUSE_KECCAK=1
endif
ifeq ($(SHA256_HASH_TYPE), 1)
    CRYPTO_CFLAGS += -DUSE_SHA2=1
endif

CRYPTO_CFLAGS += -DUSE_MONERO=1
CRYPTO_CFLAGS += -DUSE_CARDANO=1
CRYPTO_CFLAGS += -DUSE_RFC6979

# use platform depedent random mechanism
CRYPTO_CFLAGS += -DRAND_PLATFORM_INDEPENDENT

# disable certain optimizations and features when small footprint is required
CRYPTO_CFLAGS += -DUSE_PRECOMPUTED_CP=0


# Source and Objects

SOURCES = $(wildcard *.c) $(wildcard aes/*.c)
OBJECTS_DIR = $(BOAT_BUILD_DIR)/vendor/common/crypto/crypto_default
OBJECTS = $(patsubst %.c,$(OBJECTS_DIR)/%.o,$(SOURCES))


all: $(OBJECTS_DIR) $(OBJECTS)

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)/aes

$(OBJECTS_DIR)/%.o:%.c
	$(CC) -c $(CRYPTO_CFLAGS) $< -o $@


clean:	
	-$(BOAT_RM) $(OBJECTS)
