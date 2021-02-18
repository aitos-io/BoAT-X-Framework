# Set Python parser
PY = python
export PY

# Generate sub-directory list
SUBDIRS := $(shell $(BOAT_FIND) . -maxdepth 1 -type d)
SUBDIRS := $(basename $(patsubst ./%,%,$(SUBDIRS)))

GENDIR := generated
ABIDIRS := $(filter-out $(GENDIR),$(SUBDIRS))

# Add _clean_ prefix to avoid clean ABI target names being confused with compile ABI targets
CLEANABI_TARGETS := $(addprefix _clean_,$(ABIDIRS) )



# Source and Objects

SOURCES = $(wildcard *.c)
OBJECTS_DIR = $(BOAT_BUILD_DIR)/contract
OBJECTS = $(patsubst %.c,$(OBJECTS_DIR)/%.o,$(SOURCES))


.PHONY: all clean GenerateSources $(ABIDIRS) CompileGeneratedC

all:  CompileGeneratedC

CompileGeneratedC: GenerateSources
	make -C generated all

GenerateSources: $(ABIDIRS)

$(ABIDIRS):
	make -C $@ all


clean: $(CLEANABI_TARGETS)
	-$(BOAT_RM) $(BOAT_LIB_DIR)/libboatcontract.a
	make -C generated clean

$(CLEANABI_TARGETS):
	make -C $(patsubst _clean_%,%,$@) clean




