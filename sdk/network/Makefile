# Source and Objects

#SOURCES = $(wildcard *.c)
# SOURCES =  boatversion.c
SOURCES = $(wildcard *.c)
OBJECTS_DIR = $(BOAT_BUILD_DIR)/sdk/network
OBJECTS = $(patsubst %.c,$(OBJECTS_DIR)/%.o,$(SOURCES))
		  
.PHONY: all  network_ethereum network_hlfabric network_platone network_venachain network_platon network_fiscobcos network_quorum network_hwbcs network_chainmaker network_cita

all:  network_ethereum network_hlfabric network_platone network_venachain network_platon network_fiscobcos network_quorum network_hwbcs network_chainmaker network_cita $(OBJECTS_DIR) $(OBJECTS)

network_ethereum:
ifeq ($(BOAT_PROTOCOL_USE_ETHEREUM), 1)
	make -C  ethereum all
endif

network_hlfabric:
ifeq ($(BOAT_PROTOCOL_USE_HLFABRIC), 1)
	make -C  hlfabric all
endif

network_platone:
ifeq ($(BOAT_PROTOCOL_USE_PLATONE), 1)
	make -C  platone all
endif

network_venachain:
ifeq ($(BOAT_PROTOCOL_USE_VENACHAIN), 1)
	make -C  venachain all
endif

network_platon:
ifeq ($(BOAT_PROTOCOL_USE_PLATON),1)
	make -C platon all
endif

network_fiscobcos:
ifeq ($(BOAT_PROTOCOL_USE_FISCOBCOS),1)
	make -C fiscobcos all
endif

network_quorum:
ifeq ($(BOAT_PROTOCOL_USE_QUORUM),1)
	make -C quorum all
endif

network_hwbcs:
ifeq ($(BOAT_PROTOCOL_USE_HWBCS),1)
	make -C hwbcs all
endif

network_chainmaker:
ifneq ($(BOAT_PROTOCOL_USE_CHAINMAKER_V1)_$(BOAT_PROTOCOL_USE_CHAINMAKER_V2),0_0)
	make -C chainmaker all
endif

network_cita:
ifeq ($(BOAT_PROTOCOL_USE_CITA),1)
	make -C cita all
endif

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)

$(OBJECTS_DIR)/%.o:%.c
	$(CC) -c $(BOAT_CFLAGS) $< -o $@


clean:
	-$(BOAT_RM) $(OBJECTS)
ifeq ($(PROTOCOL_USE_ETHEREUM), 1)	
	make -C ethereum  clean
endif
ifeq ($(BOAT_PROTOCOL_USE_HLFABRIC), 1)
	make -C  hlfabric clean
endif
ifeq ($(BOAT_PROTOCOL_USE_PLATONE), 1)
	make -C  platone clean
endif
ifeq ($(BOAT_PROTOCOL_USE_VENACHAIN), 1)
	make -C  venachain clean
endif
ifeq ($(BOAT_PROTOCOL_USE_PLATON),1)
	make -C platon clean
endif
ifeq ($(BOAT_PROTOCOL_USE_FISCOBCOS),1)
	make -C fiscobcos clean
endif
ifeq ($(BOAT_PROTOCOL_USE_QUORUM),1)
	make -C quorum clean
endif
ifeq ($(BOAT_PROTOCOL_USE_HWBCS),1)
	make -C hwbcs clean
endif
ifneq ($(BOAT_PROTOCOL_USE_CHAINMAKER_V1)_$(BOAT_PROTOCOL_USE_CHAINMAKER_V2),0_0)
	make -C chainmaker clean
endif
ifeq ($(BOAT_PROTOCOL_USE_CITA),1)
	make -C cita clean
endif