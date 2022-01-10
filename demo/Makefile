# Source and Objects

# Generate sub-directory list
OBJECTS_DIR = $(BOAT_BUILD_DIR)/demo

.PHONY: all demo_ethereum demo_platon demo_platone demo_fiscobcos demo_fabric demo_hw_bcs demo_chainmaker
all: $(OBJECTS_DIR) demo_ethereum demo_platon demo_platone demo_fiscobcos demo_fabric demo_hw_bcs demo_chainmaker

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

demo_chainmaker:
ifeq ($(BOAT_PROTOCOL_USE_CHAINMAKER), 1)
	make -C $(BOAT_BASE_DIR)/demo/demo_chainmaker all	
endif

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)

clean:
	make -C $(BOAT_BASE_DIR)/demo/demo_ethereum clean
	make -C $(BOAT_BASE_DIR)/demo/demo_platon clean
	make -C $(BOAT_BASE_DIR)/demo/demo_platone clean
	make -C $(BOAT_BASE_DIR)/demo/demo_fiscobcos clean
	make -C $(BOAT_BASE_DIR)/demo/demo_fabric clean
	make -C $(BOAT_BASE_DIR)/demo/demo_hw_bcs clean
	make -C $(BOAT_BASE_DIR)/demo/demo_chainmaker clean
	-$(BOAT_RM) $(BOAT_BUILD_DIR)/demo
