# Source and Objects
SRC_OBJECTS = $(wildcard $(BOAT_BUILD_DIR)/vendor/platform/$(PLATFORM_TARGET)/rpc/*.o) 

PORT_CRYPTO_OBJECTS = $(wildcard $(BOAT_BUILD_DIR)/vendor/common/crypto/keccak/*.o) \
		  			  $(wildcard $(BOAT_BUILD_DIR)/vendor/common/storage/*.o) 

ifeq ($(SOFT_CRYPTO), CRYPTO_DEFAULT)
    PORT_CRYPTO_OBJECTS += $(wildcard $(BOAT_BUILD_DIR)/vendor/common/crypto/crypto_default/*.o) \
			   $(wildcard $(BOAT_BUILD_DIR)/vendor/common/crypto/crypto_default/aes/*.o) \
	           $(wildcard $(BOAT_BUILD_DIR)/vendor/platform/$(PLATFORM_TARGET)/port_crypto_default/*.o) 
    PORT_CRYPTO_DIRECTORY = port_crypto_default
else ifeq ($(SOFT_CRYPTO), CRYPTO_MBEDTLS)
    PORT_CRYPTO_OBJECTS += $(wildcard $(BOAT_BUILD_DIR)/vendor/common/crypto/mbedtls/*.o) \
	           $(wildcard $(BOAT_BUILD_DIR)/vendor/platform/$(PLATFORM_TARGET)/port_mbedtls/*.o)
    PORT_CRYPTO_DIRECTORY = port_mbedtls
else
    PORT_CRYPTO_OBJECTS += 
    PORT_CRYPTO_DIRECTORY = 
endif

LIBNAME = $(BOAT_LIB_DIR)/libboatvendor.a

.PHONY: all $(CLEAN_SUBDIRS) boatvendor_objs clean

all: boatvendor_objs $(LIBNAME)

$(LIBNAME): boatvendor_objs
	$(AR) r $(LIBNAME) $(PORT_CRYPTO_OBJECTS) $(SRC_OBJECTS)


boatvendor_objs:
	make -C $(PORT_CRYPTO_DIRECTORY) all
	make -C rpc all


clean: $(CLEAN_SUBDIRS)
	-$(BOAT_RM) $(LIBNAME)
