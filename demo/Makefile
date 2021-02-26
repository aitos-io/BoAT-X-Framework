# Source and Objects

SOURCES = $(wildcard *.c)
OBJECTS_DIR = $(BOAT_BUILD_DIR)/demo
OBJECTS = $(patsubst %.c,$(OBJECTS_DIR)/%.o,$(SOURCES))

DEPENDENCE_LIBS = $(BOAT_LIB_DIR)/libboatcontract.a \
				  $(BOAT_LIB_DIR)/libboatwallet.a \
				  $(BOAT_LIB_DIR)/libboathwdep.a \
				  $(LINK_LIBS)

all: $(OBJECTS_DIR) $(OBJECTS)
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) -o $(BOAT_BUILD_DIR)/demo/demo_ethereum_storeread \
		$(BOAT_BUILD_DIR)/demo/demo_ethereum_storeread.o $(DEPENDENCE_LIBS)
		
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) -o $(BOAT_BUILD_DIR)/demo/demo_ethereum_transfer \
		$(BOAT_BUILD_DIR)/demo/demo_ethereum_transfer.o $(DEPENDENCE_LIBS)
		
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) -o $(BOAT_BUILD_DIR)/demo/demo_fabric_invoke \
		$(BOAT_BUILD_DIR)/demo/demo_fabric_invoke.o $(DEPENDENCE_LIBS)
		
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) -o $(BOAT_BUILD_DIR)/demo/demo_platone_mycontract \
		$(BOAT_BUILD_DIR)/demo/demo_platone_mycontract.o $(DEPENDENCE_LIBS)
		
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) -o $(BOAT_BUILD_DIR)/demo/demo_fiscobcos_helloworld \
		$(BOAT_BUILD_DIR)/demo/demo_fiscobcos_helloworld.o $(DEPENDENCE_LIBS)
		

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)

$(OBJECTS_DIR)/%.o:%.c
	$(CC) -c $(BOAT_CFLAGS) $(HWDEP_INC) $< -o $@

clean:
	-$(BOAT_RM) $(OBJECTS)
	-$(BOAT_RM) $(BOAT_BUILD_DIR)/demo/*
