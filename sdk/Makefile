# BoAT SDK Library Objects

# Generate sub-directory list
ALLSUBDIRS := $(shell $(BOAT_FIND) . -maxdepth 1 -type d)
ALLSUBDIRS := $(basename $(patsubst ./%,%,$(ALLSUBDIRS)))

EXCLUDE_DIRS := include
SUBDIRS := $(filter-out $(EXCLUDE_DIRS),$(ALLSUBDIRS))

# Add _clean_ prefix to avoid clean subdir target names being confused with compile subdir targets
CLEAN_SUBDIRS := $(addprefix _clean_,$(SUBDIRS) )

OBJECTS = $(wildcard $(BOAT_BUILD_DIR)/sdk/cJSON/*.o) \
          $(wildcard $(BOAT_BUILD_DIR)/sdk/protocol/*.o) \
          $(wildcard $(BOAT_BUILD_DIR)/sdk/protocol/common/*.o) \
          $(wildcard $(BOAT_BUILD_DIR)/sdk/rlp/*.o) \
          $(wildcard $(BOAT_BUILD_DIR)/sdk/rpc/*.o) \
          $(wildcard $(BOAT_BUILD_DIR)/sdk/utilities/*.o) \
          $(wildcard $(BOAT_BUILD_DIR)/sdk/wallet/*.o)

LIBNAME = $(BOAT_LIB_DIR)/libboatwallet.a

.PHONY: all $(SUBDIRS) boatwallet_objs clean

all: $(LIBNAME)

$(LIBNAME): boatwallet_objs
	$(AR) r $(LIBNAME) $(OBJECTS)


boatwallet_objs: $(SUBDIRS)


$(SUBDIRS):
	make -C $@ all

	
$(CLEAN_SUBDIRS):
	make -C $(patsubst _clean_%,%,$@) clean

clean: $(CLEAN_SUBDIRS)
	-$(BOAT_RM) $(LIBNAME)



