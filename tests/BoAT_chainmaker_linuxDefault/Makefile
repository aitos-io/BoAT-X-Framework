# Source and Objects

SOURCES = $(wildcard *.c)
OBJECTS_DIR = $(BOAT_BUILD_DIR)/tests/BoAT_chainmaker_linuxDefault/lib
EXECUTABLE_DIR  = $(BOAT_BUILD_DIR)/tests/BoAT_chainmaker_linuxDefault
OBJECTS = $(patsubst %.c,$(OBJECTS_DIR)/%.o,$(SOURCES))

DEPENDENCE_LIBS = $(BOAT_LIB_DIR)/libboatwallet.a \
				  $(BOAT_LIB_DIR)/libboatvendor.a \
				  $(LINK_LIBS) \
				  -lcheck\
				  -lpthread\
				  -lm\
				  -lrt\
				  -lsubunit
                                
BOAT_CFLAGS +=  -DTEST_CHAINMAKER_NODE_URL=\"$(CHAINMKER_NODE_URL)\"\
				-DTEST_CHAINMAKER_CHAIN_ID=\"$(CHAINMKER_CHAIN_ID)\"\
				-DTEST_CHAINMAKER_ORG_ID=\"$(CHAINMKER_ORG_ID)\"\
				-DTEST_CHAINMAKER_HOST_NAME=\"$(CHAINMKER_HOST_NAME)\"
all: $(OBJECTS_DIR) $(OBJECTS)	
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(OBJECTS) $(DEPENDENCE_LIBS) -o $(EXECUTABLE_DIR)/boattest

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)

$(OBJECTS_DIR)/%.o:%.c
	$(CC) -c $(BOAT_CFLAGS) $< -o $@

clean:
	-$(BOAT_RM) $(OBJECTS)
	-$(BOAT_RM) $(OBJECTS_DIR)
