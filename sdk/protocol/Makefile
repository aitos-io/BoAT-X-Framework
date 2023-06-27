# Source and Objects

SOURCES = $(wildcard *.c)
OBJECTS_DIR = $(BOAT_BUILD_DIR)/sdk/protocol
OBJECTS = $(patsubst %.c,$(OBJECTS_DIR)/%.o,$(SOURCES))

.PHONY: all protocol_common protocol_ethereum protocol_platon protocol_platone protocol_fiscobcos protocol_hlfabric protocol_hw_bcs protocol_chainmaker_v1 protocol_chainmaker_v2 protocol_venachain protocol_quorum protocol_cita

all: protocol_common protocol_ethereum protocol_platon protocol_platone protocol_fiscobcos protocol_hlfabric protocol_hw_bcs protocol_chainmaker_v1 protocol_chainmaker_v2 protocol_venachain protocol_quorum protocol_cita $(OBJECTS_DIR) $(OBJECTS)


protocol_common:
	make -C common all
	
protocol_ethereum:
ifeq ($(BOAT_PROTOCOL_USE_ETHEREUM), 1)
	make -C  boatethereum all
endif

protocol_platon:
ifeq ($(BOAT_PROTOCOL_USE_PLATON), 1)
	make -C  boatplaton all
endif

protocol_platone:
ifeq ($(BOAT_PROTOCOL_USE_PLATONE), 1)
	make -C  boatplatone all
endif

protocol_fiscobcos:
ifeq ($(BOAT_PROTOCOL_USE_FISCOBCOS), 1)
	make -C  boatfiscobcos all
endif	
	
protocol_hlfabric:
ifeq ($(BOAT_PROTOCOL_USE_HLFABRIC), 1)
	make -C  boathlfabric all
endif

protocol_hw_bcs:
ifeq ($(BOAT_PROTOCOL_USE_HWBCS), 1)
	make -C  boathwbcs all
endif

protocol_chainmaker_v1:
ifeq ($(BOAT_PROTOCOL_USE_CHAINMAKER_V1), 1)
	make -C  boatchainmaker_v1 all
endif

protocol_chainmaker_v2:
ifeq ($(BOAT_PROTOCOL_USE_CHAINMAKER_V2), 1)
	make -C  boatchainmaker_v2 all
endif

protocol_venachain:
ifeq ($(BOAT_PROTOCOL_USE_VENACHAIN), 1)
	make -C  boatvenachain all
endif

protocol_quorum:
ifeq ($(BOAT_PROTOCOL_USE_QUORUM), 1)
	make -C  boatquorum all
endif

protocol_cita:
ifeq ($(BOAT_PROTOCOL_USE_CITA), 1)
	make -C  boatcita all
endif

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)

$(OBJECTS_DIR)/%.o:%.c
	$(CC) -c $(BOAT_CFLAGS) $< -o $@


clean:
	-$(BOAT_RM) $(OBJECTS)
	make -C common clean
ifeq ($(PROTOCOL_USE_ETHEREUM), 1)	
	make -C boatethereum  clean
endif
ifeq ($(PROTOCOL_USE_PLATON), 1)
	make -C boatplaton    clean
endif
ifeq ($(PROTOCOL_USE_PLATONE), 1)
	make -C boatplatone   clean
endif
ifeq ($(PROTOCOL_USE_FISCOBCOS), 1)
	make -C boatfiscobcos clean
endif
ifeq ($(BOAT_PROTOCOL_USE_HLFABRIC), 1)	
	make -C boathlfabric  clean
endif
ifeq ($(BOAT_PROTOCOL_USE_HWBCS), 1)	
	make -C boathwbcs  clean
endif
ifeq ($(BOAT_PROTOCOL_USE_CHAINMAKER_v1), 1)	
	make -C boatchainmaker_v1 clean
endif
ifeq ($(BOAT_PROTOCOL_USE_CHAINMAKER_V2), 1)	
	make -C boatchainmaker_v2 clean
endif
ifeq ($(BOAT_PROTOCOL_USE_VENACHAIN), 1)
	make -C boatvenachain clean
endif
ifeq ($(BOAT_PROTOCOL_USE_QUORUM), 1)
	make -C boatquorum clean
endif
ifeq ($(BOAT_PROTOCOL_USE_CITA), 1)
	make -C boatcita clean
endif
