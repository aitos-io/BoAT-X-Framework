# Source and Objects

OBJECTS = $(wildcard $(BOAT_BUILD_DIR)/vendor/common/crypto/keccak/*.o) \
		  $(wildcard $(BOAT_BUILD_DIR)/vendor/common/storage/*.o) 

ifeq ($(SOFT_CRYPTO_DEP), CRYPTO_DEFAULT)
    OBJECTS += $(wildcard $(BOAT_BUILD_DIR)/vendor/common/crypto/crypto_default/*.o) \
			   $(wildcard $(BOAT_BUILD_DIR)/vendor/common/crypto/crypto_default/aes/*.o) \
	           $(wildcard $(BOAT_BUILD_DIR)/vendor/platform/$(PLATFORM_TARGET)/port_crypto_default/*.o) 
    CRYPTO_PORT_DIRECTORY = port_crypto_default
else ifeq ($(SOFT_CRYPTO_DEP), CRYPTO_MBEDTLS)
    OBJECTS += $(wildcard $(BOAT_BUILD_DIR)/vendor/common/crypto/mbedtls/*.o) \
	           $(wildcard $(BOAT_BUILD_DIR)/vendor/platform/$(PLATFORM_TARGET)/port_mbedTLS/*.o)
    CRYPTO_PORT_DIRECTORY = port_mbedtls
else
    OBJECTS += 
    CRYPTO_PORT_DIRECTORY = 
endif

LIBNAME = $(BOAT_LIB_DIR)/libboatvendor.a

.PHONY: all $(CLEAN_SUBDIRS) boatvendor_objs clean

all: boatvendor_objs $(LIBNAME)

$(LIBNAME): boatvendor_objs
	$(AR) r $(LIBNAME) $(OBJECTS)


boatvendor_objs:
	make -C $(CRYPTO_PORT_DIRECTORY) all


clean: $(CLEAN_SUBDIRS)
	-$(BOAT_RM) $(LIBNAME)
