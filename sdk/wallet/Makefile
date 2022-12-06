# Source and Objects

#SOURCES = $(wildcard *.c)
SOURCES = boatversion.c
		  
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
ifeq ($(findstring $(BOAT_PROTOCOL_USE_CHAINMAKER_V1)$(BOAT_PROTOCOL_USE_CHAINMAKER_V2), 00),)
	SOURCES += api_chainmaker.c
endif

ifeq ($(BOAT_PROTOCOL_USE_VENACHAIN), 1)
	SOURCES += api_venachain.c
endif
ifeq ($(BOAT_PROTOCOL_USE_QUORUM), 1)
	SOURCES += api_quorum.c
endif
ifeq ($(BOAT_PROTOCOL_USE_CITA), 1)
	SOURCES += api_cita.c
endif


OBJECTS_DIR = $(BOAT_BUILD_DIR)/sdk/wallet
OBJECTS = $(patsubst %.c,$(OBJECTS_DIR)/%.o,$(SOURCES))


all: $(OBJECTS_DIR) $(OBJECTS)

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)

$(OBJECTS_DIR)/%.o:%.c
	$(CC) -c $(BOAT_CFLAGS) $< -o $@
	$(BOAT_COPY) $(SOURCES) $(OBJECTS_DIR)


clean:
	-$(BOAT_RM) $(OBJECTS)

