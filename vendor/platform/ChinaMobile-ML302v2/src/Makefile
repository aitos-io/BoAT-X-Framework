# Source and Objects
SRC_OBJECTS = $(wildcard $(BOAT_BUILD_DIR)/vendor/platform/$(PLATFORM_TARGET)/src/rpc/*.o) 
SRC_OBJECTS += $(wildcard $(BOAT_BUILD_DIR)/vendor/platform/$(PLATFORM_TARGET)/cjson/*.o) 

PORT_CRYPTO_OBJECTS = $(wildcard $(BOAT_BUILD_DIR)/vendor/common/storage/*.o ) \
					$(wildcard $(BOAT_BUILD_DIR)/vendor/keystore/*.o) 

PORT_CRYPTO_OBJECTS += $(wildcard $(BOAT_BUILD_DIR)/vendor/common/crypto/crypto_default/*.o) \
				$(wildcard $(BOAT_BUILD_DIR)/vendor/common/crypto/crypto_default/aes/*.o) \
				$(wildcard $(BOAT_BUILD_DIR)/vendor/platform/$(PLATFORM_TARGET)/src/port_crypto_default/*.o) 
PORT_CRYPTO_DIRECTORY = port_crypto_default

LIBNAME = $(BOAT_LIB_DIR)/libboatvendor.a

.PHONY: all $(CLEAN_SUBDIRS) boatvendor_objs clean

all: boatvendor_objs $(LIBNAME)

$(LIBNAME): boatvendor_objs
	$(AR) r $(LIBNAME) $(PORT_CRYPTO_OBJECTS) $(SRC_OBJECTS)


boatvendor_objs:
	make -C $(PORT_CRYPTO_DIRECTORY) all
	make -C rpc all
	make -C cjson all


clean: $(CLEAN_SUBDIRS)
	-$(BOAT_RM) $(LIBNAME)
