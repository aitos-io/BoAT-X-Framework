# Source and Objects

OBJECTS = $(wildcard $(BOAT_BUILD_DIR)/vendor/common/crypto/mbedtls/*.o) \
          $(wildcard $(BOAT_BUILD_DIR)/vendor/common/crypto/keccak/*.o) \
		  $(wildcard $(BOAT_BUILD_DIR)/vendor/common/storage/*.o) \
		  $(wildcard $(BOAT_BUILD_DIR)/vendor/platform/$(PLATFORM_TARGET)/port_mbedTLS/*.o)

LIBNAME = $(BOAT_LIB_DIR)/libboathwdep.a

.PHONY: all $(SUBDIRS) $(CLEAN_SUBDIRS) boathwdep_objs clean

all: boathwdep_objs $(LIBNAME)

$(LIBNAME): boathwdep_objs
	$(AR) r $(LIBNAME) $(OBJECTS)


boathwdep_objs:
	make -C port_mbedtls all


clean: $(CLEAN_SUBDIRS)
	-$(BOAT_RM) $(LIBNAME)
