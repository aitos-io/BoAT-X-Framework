# BoAT SDK Library Objects

# Generate sub-directory list
ALLSUBDIRS := $(shell $(BOAT_FIND) . -maxdepth 1 -type d)
ALLSUBDIRS := $(basename $(patsubst ./%,%,$(ALLSUBDIRS)))

EXCLUDE_DIRS := include
SUBDIRS := $(filter-out $(EXCLUDE_DIRS),$(ALLSUBDIRS))

# Add _clean_ prefix to avoid clean subdir target names being confused with compile subdir targets
CLEAN_SUBDIRS := $(addprefix _clean_,$(SUBDIRS) )

OBJECTS = $(wildcard $(BOAT_BUILD_DIR)/sdk/protocol/*.o) \
          $(wildcard $(BOAT_BUILD_DIR)/sdk/utilities/*.o) \
          $(wildcard $(BOAT_BUILD_DIR)/sdk/keypair/*.o) \
		  $(wildcard $(BOAT_BUILD_DIR)/sdk/wallet/*.o)

ifeq ($(BOAT_PROTOCOL_USE_ETHEREUM),1)
	OBJECTS += $(wildcard $(BOAT_BUILD_DIR)/sdk/protocol/boatethereum/*.o) \
			   $(wildcard $(BOAT_BUILD_DIR)/sdk/network/ethereum/*.o)
endif
ifeq ($(BOAT_PROTOCOL_USE_PLATON),1)
	OBJECTS += $(wildcard $(BOAT_BUILD_DIR)/sdk/protocol/boatplaton/*.o) \
				$(wildcard $(BOAT_BUILD_DIR)/sdk/network/platon/*.o)
endif
ifeq ($(BOAT_PROTOCOL_USE_PLATONE),1)
	OBJECTS += $(wildcard $(BOAT_BUILD_DIR)/sdk/protocol/boatplatone/*.o) \
				$(wildcard $(BOAT_BUILD_DIR)/sdk/network/platone/*.o)
endif
ifeq ($(BOAT_PROTOCOL_USE_FISCOBCOS),1)
	OBJECTS += $(wildcard $(BOAT_BUILD_DIR)/sdk/protocol/boatfiscobcos/*.o) \
				$(wildcard $(BOAT_BUILD_DIR)/sdk/network/fiscobcos/*.o)
endif
ifeq ($(BOAT_PROTOCOL_USE_VENACHAIN),1)
	OBJECTS += $(wildcard $(BOAT_BUILD_DIR)/sdk/protocol/boatvenachain/*.o) \
				$(wildcard $(BOAT_BUILD_DIR)/sdk/network/venachain/*.o)
endif
ifeq ($(BOAT_PROTOCOL_USE_QUORUM),1)
	OBJECTS += $(wildcard $(BOAT_BUILD_DIR)/sdk/protocol/boatquorum/*.o) \
				$(wildcard $(BOAT_BUILD_DIR)/sdk/network/quorum/*.o)
endif
ifeq ($(BOAT_PROTOCOL_USE_HLFABRIC),1)
	OBJECTS += $(wildcard $(BOAT_BUILD_DIR)/sdk/protocol/boathlfabric/*.o) \
				$(wildcard $(BOAT_BUILD_DIR)/sdk/network/hlfabric/*.o) 

endif
ifeq ($(BOAT_PROTOCOL_USE_HWBCS),1)
	OBJECTS += $(wildcard $(BOAT_BUILD_DIR)/sdk/protocol/boathwbcs/*.o) \
				$(wildcard $(BOAT_BUILD_DIR)/sdk/protocol/boathwbcs/protos/common/*.o) \
				$(wildcard $(BOAT_BUILD_DIR)/sdk/network/hwbcs/*.o)
endif
ifeq ($(BOAT_PROTOCOL_USE_CHAINMAKER_V1),1)
	OBJECTS += $(wildcard $(BOAT_BUILD_DIR)/sdk/protocol/boatchainmaker_v1/*.o) \
			   $(wildcard $(BOAT_BUILD_DIR)/sdk/protocol/boatchainmaker_v1/protos/*/*.o) \
			   $(wildcard $(BOAT_BUILD_DIR)/sdk/network/chainmaker/*.o) 
endif
ifeq ($(BOAT_PROTOCOL_USE_CHAINMAKER_V2),1)
	OBJECTS += $(wildcard $(BOAT_BUILD_DIR)/sdk/protocol/boatchainmaker_v2/*.o) \
			   $(wildcard $(BOAT_BUILD_DIR)/sdk/protocol/boatchainmaker_v2/protos/*/*.o) \
			   $(wildcard $(BOAT_BUILD_DIR)/sdk/network/chainmaker/*.o) 
endif

ifeq ($(BOAT_PROTOCOL_USE_CITA),1)
	OBJECTS += $(wildcard $(BOAT_BUILD_DIR)/sdk/protocol/boatcita/*.o) \
			   $(wildcard $(BOAT_BUILD_DIR)/sdk/protocol/boatcita/protos/*.o) \
			   $(wildcard $(BOAT_BUILD_DIR)/sdk/network/cita/*.o) \
			   $(wildcard $(BOAT_BUILD_DIR)/sdk/third-party/protobuf-c/*.o) 
endif

ifneq ($(BOAT_PROTOCOL_USE_HLFABRIC)_$(BOAT_PROTOCOL_USE_HWBCS)_$(BOAT_PROTOCOL_USE_CHAINMAKER_V1)_$(BOAT_PROTOCOL_USE_CHAINMAKER_V2),0_0_0_0)
	OBJECTS += $(wildcard $(BOAT_BUILD_DIR)/sdk/third-party/protos/*/*.o) \
		  	   $(wildcard $(BOAT_BUILD_DIR)/sdk/third-party/protos/*/*/*.o) \
			   $(wildcard $(BOAT_BUILD_DIR)/sdk/third-party/nghttp2/*.o) \
			   $(wildcard $(BOAT_BUILD_DIR)/sdk/third-party/protobuf-c/*.o) \
			   $(wildcard $(BOAT_BUILD_DIR)/sdk/protocol/common/http2intf/*.o)
endif

ifneq ($(BOAT_PROTOCOL_USE_ETHEREUM)_$(BOAT_PROTOCOL_USE_PLATON)_$(BOAT_PROTOCOL_USE_PLATONE)_$(BOAT_PROTOCOL_USE_FISCOBCOS)_$(BOAT_PROTOCOL_USE_VENACHAIN)_$(BOAT_PROTOCOL_USE_QUORUM)_$(BOAT_PROTOCOL_USE_CITA),0_0_0_0_0_0_0)
	OBJECTS += $(wildcard $(BOAT_BUILD_DIR)/sdk/protocol/common/web3intf/*.o) \
				$(wildcard $(BOAT_BUILD_DIR)/sdk/rlp/*.o) 
endif




ifeq ($(CJSON_LIBRARY), CJSON_DEFAULT)
    OBJECTS += $(wildcard $(BOAT_BUILD_DIR)/sdk/third-party/cJSON/*.o)
endif

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



