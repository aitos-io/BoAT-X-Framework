# Source and Objects

SOURCES = $(wildcard *.c)
OBJECTS_DIR = $(BOAT_BUILD_DIR)/demo

DEPENDENCE_LIBS = $(BOAT_LIB_DIR)/libboatcontract.a \
				  $(BOAT_LIB_DIR)/libboatwallet.a \
				  $(BOAT_LIB_DIR)/libboathwdep.a \
				  $(LINK_LIBS)

.PHONY: all ethereum_demo platone_demo fiscbcos_demo fabric_demo 
all:$(OBJECTS_DIR) ethereum_demo platone_demo fiscbcos_demo fabric_demo

#ETHEREUM transfer demo build	
ethereum_demo: demo_ethereum_transfer.c demo_ethereum_storeread.c
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_ethereum_transfer.c \
		-DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/ethereum/demo_ethereum_transfer_onetime_internalGen
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_ethereum_transfer.c \
		-DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/ethereum/demo_ethereum_transfer_onetime_pkcs
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_ethereum_transfer.c \
		-DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/ethereum/demo_ethereum_transfer_onetime_native
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_ethereum_transfer.c \
		-DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/ethereum/demo_ethereum_transfer_create_internalGen
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_ethereum_transfer.c \
		-DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/ethereum/demo_ethereum_transfer_create_pkcs		
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_ethereum_transfer.c \
		-DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/ethereum/demo_ethereum_transfer_create_native
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_ethereum_transfer.c \
		-DUSE_LOAD_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/ethereum/demo_ethereum_transfer_load_internalGen
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_ethereum_transfer.c \
		-DUSE_LOAD_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/ethereum/demo_ethereum_transfer_load_pkcs
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_ethereum_transfer.c \
		-DUSE_LOAD_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/ethereum/demo_ethereum_transfer_load_native

	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_ethereum_storeread.c \
		-DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/ethereum/demo_ethereum_storeread_onetime_internalGen
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_ethereum_storeread.c \
		-DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/ethereum/demo_ethereum_storeread_onetime_pkcs
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_ethereum_storeread.c \
		-DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/ethereum/demo_ethereum_storeread_onetime_native
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_ethereum_storeread.c \
		-DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/ethereum/demo_ethereum_storeread_create_internalGen
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_ethereum_storeread.c \
		-DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/ethereum/demo_ethereum_storeread_create_pkcs		
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_ethereum_storeread.c \
		-DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/ethereum/demo_ethereum_storeread_create_native
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_ethereum_storeread.c \
		-DUSE_LOAD_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/ethereum/demo_ethereum_storeread_load_internalGen
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_ethereum_storeread.c \
		-DUSE_LOAD_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/ethereum/demo_ethereum_storeread_load_pkcs
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_ethereum_storeread.c \
		-DUSE_LOAD_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/ethereum/demo_ethereum_storeread_load_native		

#PLATONE transfer demo build	
platone_demo: demo_platone_mycontract.c
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_platone_mycontract.c \
		-DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/platone/demo_platone_mycontract_onetime_internalGen
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_platone_mycontract.c \
		-DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/platone/demo_platone_mycontract_onetime_pkcs
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_platone_mycontract.c \
		-DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/platone/demo_platone_mycontract_onetime_native
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_platone_mycontract.c \
		-DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/platone/demo_platone_mycontract_create_internalGen
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_platone_mycontract.c \
		-DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/platone/demo_platone_mycontract_create_pkcs		
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_platone_mycontract.c \
		-DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/platone/demo_platone_mycontract_create_native
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_platone_mycontract.c \
		-DUSE_LOAD_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/platone/demo_platone_mycontract_load_internalGen
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_platone_mycontract.c \
		-DUSE_LOAD_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/platone/demo_platone_mycontract_load_pkcs
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_platone_mycontract.c \
		-DUSE_LOAD_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/platone/demo_platone_mycontract_load_native

#Fiscobcos transfer demo build	
fiscbcos_demo: demo_fiscobcos_helloworld.c
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_fiscobcos_helloworld.c \
		-DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/fiscobcos/demo_fiscobcos_helloworld_onetime_internalGen
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_fiscobcos_helloworld.c \
		-DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/fiscobcos/demo_fiscobcos_helloworld_onetime_pkcs
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_fiscobcos_helloworld.c \
		-DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/fiscobcos/demo_fiscobcos_helloworld_onetime_native
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_fiscobcos_helloworld.c \
		-DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/fiscobcos/demo_fiscobcos_helloworld_create_internalGen
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_fiscobcos_helloworld.c \
		-DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/fiscobcos/demo_fiscobcos_helloworld_create_pkcs		
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_fiscobcos_helloworld.c \
		-DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/fiscobcos/demo_fiscobcos_helloworld_create_native
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_fiscobcos_helloworld.c \
		-DUSE_LOAD_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/fiscobcos/demo_fiscobcos_helloworld_load_internalGen
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_fiscobcos_helloworld.c \
		-DUSE_LOAD_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/fiscobcos/demo_fiscobcos_helloworld_load_pkcs
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_fiscobcos_helloworld.c \
		-DUSE_LOAD_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/fiscobcos/demo_fiscobcos_helloworld_load_native

#Fabric transfer demo build	
fabric_demo: demo_fabric_invoke.c
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_fabric_invoke.c \
		$(DEPENDENCE_LIBS) -o $(BOAT_BUILD_DIR)/demo/fabric/demo_fabric_invoke
	

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)/ethereum
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)/platone
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)/fiscobcos
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)/fabric

clean:
	-$(BOAT_RM) $(BOAT_BUILD_DIR)/demo/*
