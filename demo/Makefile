# Source and Objects

# Generate sub-directory list
OBJECTS_DIR = $(BOAT_BUILD_DIR)/demo

.PHONY: all demo_ethereum demo_platon demo_platone demo_fiscobcos demo_fabric demo_hw_bcs demo_chainmaker_v1 demo_chainmaker_v2 demo_venachain demo_quorum demo_cita
all: $(OBJECTS_DIR) demo_ethereum demo_platon demo_platone demo_fiscobcos demo_fabric demo_hw_bcs demo_chainmaker_v1 demo_chainmaker_v2 demo_venachain demo_quorum demo_cita

demo_ethereum:
ifeq ($(BOAT_PROTOCOL_USE_ETHEREUM), 1)
	make -C $(BOAT_BASE_DIR)/demo/demo_ethereum all
endif

demo_platon:
ifeq ($(BOAT_PROTOCOL_USE_PLATON), 1)
	make -C $(BOAT_BASE_DIR)/demo/demo_platon all
endif	

demo_platone:
ifeq ($(BOAT_PROTOCOL_USE_PLATONE), 1)
	make -C $(BOAT_BASE_DIR)/demo/demo_platone all
endif	

demo_fiscobcos:
ifeq ($(BOAT_PROTOCOL_USE_FISCOBCOS), 1)
	make -C $(BOAT_BASE_DIR)/demo/demo_fiscobcos all
endif

demo_fabric:
ifeq ($(BOAT_PROTOCOL_USE_HLFABRIC), 1)
	make -C $(BOAT_BASE_DIR)/demo/demo_fabric all	
endif

demo_hw_bcs:
ifeq ($(BOAT_PROTOCOL_USE_HWBCS), 1)
	make -C $(BOAT_BASE_DIR)/demo/demo_hw_bcs all	
endif

demo_chainmaker_v1:
ifeq ($(BOAT_PROTOCOL_USE_CHAINMAKER_V1), 1)
	make -C $(BOAT_BASE_DIR)/demo/demo_chainmaker all	
endif

demo_chainmaker_v2:
ifeq ($(BOAT_PROTOCOL_USE_CHAINMAKER_V2), 1)
	make -C $(BOAT_BASE_DIR)/demo/demo_chainmaker all	
endif

demo_venachain:
ifeq ($(BOAT_PROTOCOL_USE_VENACHAIN), 1)
	make -C $(BOAT_BASE_DIR)/demo/demo_venachain all
endif

demo_quorum:
ifeq ($(BOAT_PROTOCOL_USE_QUORUM), 1)
	make -C $(BOAT_BASE_DIR)/demo/demo_quorum all
endif

demo_cita:
ifeq ($(BOAT_PROTOCOL_USE_CITA), 1)
	make -C $(BOAT_BASE_DIR)/demo/demo_cita all
endif

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)

clean:
ifeq ($(BOAT_PROTOCOL_USE_ETHEREUM), 1)
	make -C $(BOAT_BASE_DIR)/demo/demo_ethereum clean
endif
ifeq ($(BOAT_PROTOCOL_USE_PLATON), 1)
	make -C $(BOAT_BASE_DIR)/demo/demo_platon clean
endif
ifeq ($(BOAT_PROTOCOL_USE_PLATONE), 1)
	make -C $(BOAT_BASE_DIR)/demo/demo_platone clean
endif
ifeq ($(BOAT_PROTOCOL_USE_FISCOBCOS), 1)
	make -C $(BOAT_BASE_DIR)/demo/demo_fiscobcos clean
endif
ifeq ($(BOAT_PROTOCOL_USE_HLFABRIC), 1)
	make -C $(BOAT_BASE_DIR)/demo/demo_fabric clean
endif
ifeq ($(BOAT_PROTOCOL_USE_HWBCS), 1)
	make -C $(BOAT_BASE_DIR)/demo/demo_hw_bcs clean
endif
ifeq ($(BOAT_PROTOCOL_USE_CHAINMAKER_V1), 1)
	make -C $(BOAT_BASE_DIR)/demo/demo_chainmaker clean
endif
ifeq ($(BOAT_PROTOCOL_USE_CHAINMAKER_V2), 1)
	make -C $(BOAT_BASE_DIR)/demo/demo_chainmaker clean
endif
ifeq ($(BOAT_PROTOCOL_USE_VENACHAIN), 1)
	make -C $(BOAT_BASE_DIR)/demo/demo_venachain clean
endif
ifeq ($(BOAT_PROTOCOL_USE_QUORUM), 1)
	make -C $(BOAT_BASE_DIR)/demo/demo_quorum clean
endif
ifeq ($(BOAT_PROTOCOL_USE_CITA), 1)
	make -C $(BOAT_BASE_DIR)/demo/demo_cita clean
endif
	-$(BOAT_RM) $(BOAT_BUILD_DIR)/demo
