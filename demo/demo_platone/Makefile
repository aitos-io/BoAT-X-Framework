# Source and Objects

# Specify demo folder name
DEMO_FOLDER    = demo_platone

# Specify generator script
PY             = python
AUTOGEN_SCRIPT = $(BOAT_BASE_DIR)/tools/platone2c.py

# Source and Objects
CONTRACT_SRC_DIR     = $(BOAT_BASE_DIR)/demo/$(DEMO_FOLDER)/demo_contract
OBJECTS_DIR          = $(BOAT_BUILD_DIR)/demo/$(DEMO_FOLDER)
CONTRACT_OBJECTS_DIR = $(BOAT_BUILD_DIR)/demo/$(DEMO_FOLDER)/demo_contract
DEMO_SOURCES         = $(wildcard *.c) 
CONTRACT_SOURCES     = $(wildcard $(CONTRACT_OBJECTS_DIR)/*.c) 
CONTRACT_INCLUDE     = -I$(BOAT_BUILD_DIR)/demo/$(DEMO_FOLDER)/demo_contract

# Search all files with .json extension
ABIJSONS = $(wildcard $(CONTRACT_SRC_DIR)/*.json)

#generate contract source files
OUTPUT_CONTRACT_SOURCES = $(patsubst %.json, %.c, $(ABIJSONS)) $(patsubst %.json, %.h, $(ABIJSONS))

#dependence libs
DEPENDENCE_LIBS = $(BOAT_LIB_DIR)/libboatwallet.a \
				  $(BOAT_LIB_DIR)/libboatvendor.a \
				  $(LINK_LIBS)

all: $(OBJECTS_DIR) $(OUTPUT_CONTRACT_SOURCES)	
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(CONTRACT_INCLUDE) $(CONTRACT_SOURCES) demo_platone_mycontract.c \
		-DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_platone_mycontract_onetime_internalGen
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(CONTRACT_INCLUDE) $(CONTRACT_SOURCES) demo_platone_mycontract.c \
		-DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_platone_mycontract_onetime_pkcs
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(CONTRACT_INCLUDE) $(CONTRACT_SOURCES) demo_platone_mycontract.c \
		-DUSE_ONETIME_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_platone_mycontract_onetime_native
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(CONTRACT_INCLUDE) $(CONTRACT_SOURCES) demo_platone_mycontract.c \
		-DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_INTERNAL_GENERATION \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_platone_mycontract_create_internalGen
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(CONTRACT_INCLUDE) $(CONTRACT_SOURCES) demo_platone_mycontract.c \
		-DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_PKCS \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_platone_mycontract_create_pkcs		
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(CONTRACT_INCLUDE) $(CONTRACT_SOURCES) demo_platone_mycontract.c \
		-DUSE_CREATE_PERSIST_WALLET -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_platone_mycontract_create_native
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(CONTRACT_INCLUDE) $(CONTRACT_SOURCES) demo_platone_mycontract.c \
		-DUSE_LOAD_PERSIST_WALLET \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_platone_mycontract_load		 

$(OUTPUT_CONTRACT_SOURCES): $(ABIJSONS)
	$(PY) $(AUTOGEN_SCRIPT) $< $(CONTRACT_OBJECTS_DIR)

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)/demo_contract
	
clean:
	-$(BOAT_RM) $(OBJECTS_DIR)/*
