# Source and Objects

#SOURCES = $(wildcard *.c)
SOURCES = boatwallet.c \
          boatversion.c
		  
ifeq ($(BOAT_PROTOCOL_USE_ETHEREUM), 1)
	SOURCES += api_ethereum.c
endif
ifeq ($(BOAT_PROTOCOL_USE_PLATON), 1)
	SOURCES += api_platon.c
endif		  
ifeq ($(BOAT_PROTOCOL_USE_PLATONE), 1)
	SOURCES += api_platone.c
endif	
ifeq ($(BOAT_PROTOCOL_USE_FISCOBCOS), 1)
	SOURCES += api_fiscobcos.c
endif
ifeq ($(BOAT_PROTOCOL_USE_HLFABRIC), 1)
	SOURCES += api_hlfabric.c
endif
ifeq ($(BOAT_PROTOCOL_USE_HWBCS), 1)
	SOURCES += api_hw_bcs.c
endif
ifeq ($(BOAT_PROTOCOL_USE_CHAINMAKER), 1)
	SOURCES += api_chainmaker.c
endif

OBJECTS_DIR = $(BOAT_BUILD_DIR)/sdk/wallet
OBJECTS = $(patsubst %.c,$(OBJECTS_DIR)/%.o,$(SOURCES))


all: $(OBJECTS_DIR) $(OBJECTS)

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)

$(OBJECTS_DIR)/%.o:%.c
	$(CC) -c $(BOAT_CFLAGS) $< -o $@


clean:
	-$(BOAT_RM) $(OBJECTS)

