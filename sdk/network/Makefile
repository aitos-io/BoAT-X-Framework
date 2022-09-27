# Source and Objects

#SOURCES = $(wildcard *.c)
# SOURCES =  boatversion.c
SOURCES = $(wildcard *.c)
OBJECTS_DIR = $(BOAT_BUILD_DIR)/sdk/network
OBJECTS = $(patsubst %.c,$(OBJECTS_DIR)/%.o,$(SOURCES))
		  
.PHONY: all  network_ethereum network_hlfabric network_chainmaker

all:  network_ethereum network_hlfabric network_chainmaker $(OBJECTS_DIR) $(OBJECTS)

network_ethereum:
ifeq ($(BOAT_PROTOCOL_USE_ETHEREUM), 1)
	make -C  ethereum all
endif

network_hlfabric:
ifeq ($(BOAT_PROTOCOL_USE_HLFABRIC), 1)
	make -C  hlfabric all
endif

network_chainmaker:
ifeq ($(BOAT_PROTOCOL_USE_CHAINMAKER), 1)
	make -C  chainmaker all
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
ifeq ($(BOAT_PROTOCOL_USE_CHAINMAKER), 1)
	make -C  chainmaker clean
endif

