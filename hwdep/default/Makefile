include hwdep.env

# Generate sub-directory list
SUBDIRS := $(shell $(BOAT_FIND) . -maxdepth 1 -type d)
SUBDIRS := $(basename $(patsubst ./%,%,$(SUBDIRS)))

# Add _clean_ prefix to avoid clean subdir target names being confused with compile subdir targets
CLEAN_SUBDIRS := $(addprefix _clean_,$(SUBDIRS) )

# Source and Objects

OBJECTS = $(wildcard $(BOAT_BUILD_DIR)/hwdep/crypto/*.o) \
          $(wildcard $(BOAT_BUILD_DIR)/hwdep/keccak/*.o) \
		  $(wildcard $(BOAT_BUILD_DIR)/hwdep/storage/*.o)

LIBNAME = $(BOAT_LIB_DIR)/libboathwdep.a

.PHONY: all $(SUBDIRS) $(CLEAN_SUBDIRS) boathwdep_objs clean

all: $(LIBNAME)

$(LIBNAME): boathwdep_objs
	$(AR) r $(LIBNAME) $(OBJECTS)


boathwdep_objs: $(SUBDIRS)


$(SUBDIRS):
	make -C $@ all

	
$(CLEAN_SUBDIRS):
	make -C $(patsubst _clean_%,%,$@) clean

clean: $(CLEAN_SUBDIRS)
	-$(BOAT_RM) $(LIBNAME)


