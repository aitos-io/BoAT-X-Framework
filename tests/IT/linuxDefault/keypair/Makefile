# Source and Objects

SOURCES = $(wildcard *.c)
OBJECTS_DIR = $(BOAT_BUILD_DIR)/tests/IT/linuxDefault/keypair/lib
EXECUTABLE_DIR  = $(BOAT_BUILD_DIR)/tests/IT/linuxDefault/keypair
OBJECTS = $(patsubst %.c,$(OBJECTS_DIR)/%.o,$(SOURCES))

DEPENDENCE_LIBS = $(BOAT_LIB_DIR)/libboatwallet.a \
				  $(BOAT_LIB_DIR)/libboatvendor.a \
				  $(LINK_LIBS) \
				  -lcheck\
				  -lpthread\
				  -lm\
				  -lrt\
				  -lsubunit

BOAT_CFLAGS +=  -DTEST_FABRIC_NODE_URL=\"$(FABRIC_NODE_URL)\"
all: $(OBJECTS_DIR) $(OBJECTS)	
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(OBJECTS) $(DEPENDENCE_LIBS) -o $(EXECUTABLE_DIR)/keypairtest

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)

$(OBJECTS_DIR)/%.o:%.c
	$(CC) -c $(BOAT_CFLAGS) $< -o $@

clean:
	-$(BOAT_RM) $(OBJECTS)
	-$(BOAT_RM) $(OBJECTS_DIR)
