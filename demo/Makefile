# Source and Objects

SOURCES = $(wildcard *.c)
OBJECTS_DIR = $(BOAT_BUILD_DIR)/demo
OBJECTS = $(patsubst %.c,$(OBJECTS_DIR)/%.o,$(SOURCES))

DEPENDENCE_LIBS = $(BOAT_LIB_DIR)/libboatcontract.a \
				  $(BOAT_LIB_DIR)/libboatwallet.a \
				  $(BOAT_LIB_DIR)/libboathwdep.a \
				  $(LINK_LIBS)

all: $(OBJECTS_DIR) $(SOURCES)
#Ethereum  storeread contract demo build	
#	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) -o $(BOAT_BUILD_DIR)/demo/demo_ethereum_storeread \
		$(BOAT_BUILD_DIR)/demo/demo_ethereum_storeread.o $(DEPENDENCE_LIBS)

#Ethereum transfer demo build	
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_ethereum_transfer.c \
		-DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/demo_ethereum_transfer_onetime_internalGen
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_ethereum_transfer.c \
		-DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/demo_ethereum_transfer_onetime_pkcs	
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_ethereum_transfer.c \
		-DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/demo_ethereum_transfer_onetime_native
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_ethereum_transfer.c \
		-DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/demo_ethereum_transfer_create_internalGen
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_ethereum_transfer.c \
		-DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/demo_ethereum_transfer_create_pkcs		
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_ethereum_transfer.c \
		-DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/demo_ethereum_transfer_create_native
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_ethereum_transfer.c \
		-DUSE_LOAD_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/demo_ethereum_transfer_load_internalGen
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_ethereum_transfer.c \
		-DUSE_LOAD_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/demo_ethereum_transfer_load_pkcs
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_ethereum_transfer.c \
		-DUSE_LOAD_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/demo_ethereum_transfer_load_native


#	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) -o $(BOAT_BUILD_DIR)/demo/demo_fabric_invoke \
		$(BOAT_BUILD_DIR)/demo/demo_fabric_invoke.o $(DEPENDENCE_LIBS)
		
#	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) -o $(BOAT_BUILD_DIR)/demo/demo_platone_mycontract \
		$(BOAT_BUILD_DIR)/demo/demo_platone_mycontract.o $(DEPENDENCE_LIBS)
		
#	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) -o $(BOAT_BUILD_DIR)/demo/demo_fiscobcos_helloworld \
		$(BOAT_BUILD_DIR)/demo/demo_fiscobcos_helloworld.o $(DEPENDENCE_LIBS)
		

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)

#$(OBJECTS_DIR)/%.o:%.c
#	$(CC) -c $(BOAT_CFLAGS) $(HWDEP_INC) $< -o $@

clean:
	-$(BOAT_RM) $(OBJECTS)
	-$(BOAT_RM) $(BOAT_BUILD_DIR)/demo/*
