# Source and Objects
SRC_OBJECTS = $(wildcard $(BOAT_BUILD_DIR)/vendor/platform/$(PLATFORM_TARGET)/src/rpc/*.o) \
			  $(wildcard $(BOAT_BUILD_DIR)/vendor/platform/$(PLATFORM_TARGET)/src/log/*.o) 

PORT_CRYPTO_OBJECTS = $(wildcard $(BOAT_BUILD_DIR)/vendor/common/crypto/keccak/*.o) \
		  			  $(wildcard $(BOAT_BUILD_DIR)/vendor/common/storage/*.o) 

PORT_CRYPTO_OBJECTS += $(wildcard $(BOAT_BUILD_DIR)/vendor/common/crypto/crypto_default/*.o) \
			$(wildcard $(BOAT_BUILD_DIR)/vendor/common/crypto/crypto_default/aes/*.o) \
			$(wildcard $(BOAT_BUILD_DIR)/vendor/platform/$(PLATFORM_TARGET)/src/port_crypto_default/*.o) 


LIBNAME = $(BOAT_LIB_DIR)/libboatvendor.a

.PHONY: all $(CLEAN_SUBDIRS) boatvendor_objs clean

all: boatvendor_objs $(LIBNAME)

$(LIBNAME): boatvendor_objs
	$(AR) r $(LIBNAME) $(PORT_CRYPTO_OBJECTS) $(SRC_OBJECTS)


boatvendor_objs:
	make -C port_crypto_default all
	make -C rpc all
	make -C log all


clean: $(CLEAN_SUBDIRS)
	-$(BOAT_RM) $(LIBNAME)
