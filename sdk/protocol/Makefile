# Source and Objects

SOURCES = $(wildcard *.c)
OBJECTS_DIR = $(BOAT_BUILD_DIR)/sdk/protocol
OBJECTS = $(patsubst %.c,$(OBJECTS_DIR)/%.o,$(SOURCES))

.PHONY: all rotocol_common protocol_ethereum protocol_platone protocol_fiscobcos protocol_hlfabric

all: protocol_common protocol_ethereum protocol_platone protocol_fiscobcos protocol_hlfabric $(OBJECTS_DIR) $(OBJECTS)


protocol_common:
	make -C common all
	
protocol_ethereum:
ifeq ($(BOAT_PROTOCOL_USE_ETHEREUM), 1)
	make -C  boatethereum all
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

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)

$(OBJECTS_DIR)/%.o:%.c
	$(CC) -c $(BOAT_CFLAGS) $< -o $@


clean:
	-$(BOAT_RM) $(OBJECTS)
	make -C common clean
	make -C boatethereum  clean
	make -C boatplatone   clean
	make -C boatfiscobcos clean
ifeq ($(BOAT_PROTOCOL_USE_HLFABRIC), 1)	
	make -C boathlfabric  clean
endif