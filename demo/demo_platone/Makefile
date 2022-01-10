# Source and Objects

# Specify demo folder name
DEMO_FOLDER    = demo_platone

# Source and Objects
CONTRACT_SRC_DIR     			= $(BOAT_BASE_DIR)/demo/$(DEMO_FOLDER)/demo_contract
OBJECTS_DIR          			= $(BOAT_BUILD_DIR)/demo/$(DEMO_FOLDER)
CONTRACT_WASM_OBJECTS_DIR 		= $(BOAT_BUILD_DIR)/demo/$(DEMO_FOLDER)/demo_contract/WASM
CONTRACT_SOLIDITY_OBJECTS_DIR 	= $(BOAT_BUILD_DIR)/demo/$(DEMO_FOLDER)/demo_contract/Solidity
DEMO_SOURCES         			= $(wildcard *.c) 
CONTRACT_SOURCES     			= $(wildcard $(CONTRACT_WASM_OBJECTS_DIR)/*.c) \
								  $(wildcard $(CONTRACT_SOLIDITY_OBJECTS_DIR)/*.c)
CONTRACT_WASM_INCLUDE     		= -I$(BOAT_BUILD_DIR)/demo/$(DEMO_FOLDER)/demo_contract/WASM
CONTRACT_SOLIDITY_INCLUDE     	= -I$(BOAT_BUILD_DIR)/demo/$(DEMO_FOLDER)/demo_contract/Solidity

#dependence libs
DEPENDENCE_LIBS = $(BOAT_LIB_DIR)/libboatwallet.a \
				  $(BOAT_LIB_DIR)/libboatvendor.a \
				  $(LINK_LIBS)

.PHONY: all contract_generate
all: $(OBJECTS_DIR) contract_generate	
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(CONTRACT_WASM_INCLUDE) $(CONTRACT_SOLIDITY_INCLUDE) $(CONTRACT_SOURCES) demo_platone_mycontract.c \
		-DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_platone_mycontract_onetime_internalGen
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(CONTRACT_WASM_INCLUDE) $(CONTRACT_SOLIDITY_INCLUDE) $(CONTRACT_SOURCES) demo_platone_mycontract.c \
		-DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_platone_mycontract_onetime_pkcs
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(CONTRACT_WASM_INCLUDE) $(CONTRACT_SOLIDITY_INCLUDE) $(CONTRACT_SOURCES) demo_platone_mycontract.c \
		-DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_platone_mycontract_onetime_native
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(CONTRACT_WASM_INCLUDE) $(CONTRACT_SOLIDITY_INCLUDE) $(CONTRACT_SOURCES) demo_platone_mycontract.c \
		-DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_platone_mycontract_create_internalGen
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(CONTRACT_WASM_INCLUDE) $(CONTRACT_SOLIDITY_INCLUDE) $(CONTRACT_SOURCES) demo_platone_mycontract.c \
		-DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_platone_mycontract_create_pkcs		
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(CONTRACT_WASM_INCLUDE) $(CONTRACT_SOLIDITY_INCLUDE) $(CONTRACT_SOURCES) demo_platone_mycontract.c \
		-DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_platone_mycontract_create_native
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(CONTRACT_WASM_INCLUDE) $(CONTRACT_SOLIDITY_INCLUDE) $(CONTRACT_SOURCES) demo_platone_mycontract.c \
		-DUSE_LOAD_PERSIST_WALLET \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_platone_mycontract_load		 

contract_generate:
	make -C $(CONTRACT_SRC_DIR) all

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)/demo_contract
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)/demo_contract/WASM
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)/demo_contract/Solidity
	
clean:
	-$(BOAT_RM) $(OBJECTS_DIR)/*
