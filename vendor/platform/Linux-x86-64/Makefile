## Select one of the hardware

#all:
#	make -C $(BOAT_BASE_DIR)/vendor/$(PLATFORM_TARGET) all

#clean:	
#	make -C $(BOAT_BASE_DIR)/vendor/$(PLATFORM_TARGET) clean


# Generate sub-directory list
SUBDIRS := $(shell $(BOAT_FIND) . -maxdepth 1 -type d)
SUBDIRS := $(basename $(patsubst ./%,%,$(SUBDIRS)))

# Add _clean_ prefix to avoid clean subdir target names being confused with compile subdir targets
CLEAN_SUBDIRS := $(addprefix _clean_,$(SUBDIRS) )

# Source and Objects

OBJECTS = $(wildcard $(BOAT_BUILD_DIR)/vendor/common/crypto/mbedtls/*.o) \
          $(wildcard $(BOAT_BUILD_DIR)/vendor/common/crypto/keccak/*.o) \
		  $(wildcard $(BOAT_BUILD_DIR)/vendor/common/storage/*.o) \
		  $(wildcard $(BOAT_BUILD_DIR)/vendor/$(PLATFORM_TARGET)/port_mbedTLS/*.o)

LIBNAME = $(BOAT_LIB_DIR)/libboathwdep.a

.PHONY: all $(SUBDIRS) $(CLEAN_SUBDIRS) boathwdep_objs clean

all: $(LIBNAME)

$(LIBNAME): boathwdep_objs
	@echo ----------------------------------------------------
	$(AR) r $(LIBNAME) $(OBJECTS)


boathwdep_objs: $(SUBDIRS)


$(SUBDIRS):
	make -C $@ all

	
$(CLEAN_SUBDIRS):
	make -C $(patsubst _clean_%,%,$@) clean

clean: $(CLEAN_SUBDIRS)
	-$(BOAT_RM) $(LIBNAME)


