OPTFLAGS ?= -O3 

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
CRYPTO_CFLAGS += -DUSE_ETHEREUM=1
CRYPTO_CFLAGS += -DUSE_GRAPHENE=1
CRYPTO_CFLAGS += -DUSE_KECCAK=1
CRYPTO_CFLAGS += -DUSE_MONERO=1
CRYPTO_CFLAGS += -DUSE_NEM=1
CRYPTO_CFLAGS += -DUSE_CARDANO=1
CRYPTO_CFLAGS += -DUSE_RFC6979

# use platform depedent random mechanism
CRYPTO_CFLAGS += -DRAND_PLATFORM_INDEPENDENT

# disable certain optimizations and features when small footprint is required
ifdef SMALL
CRYPTO_CFLAGS += -DUSE_PRECOMPUTED_CP=0
endif

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
