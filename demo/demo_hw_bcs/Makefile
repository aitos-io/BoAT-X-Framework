# Source and Objects

# Specify demo folder name
DEMO_FOLDER    = demo_hw_bcs

# Source and Objects
OBJECTS_DIR          = $(BOAT_BUILD_DIR)/demo/$(DEMO_FOLDER)
DEMO_SOURCES         = $(wildcard *.c) 

#dependence libs
DEPENDENCE_LIBS = $(BOAT_LIB_DIR)/libboatwallet.a \
				  $(BOAT_LIB_DIR)/libboatvendor.a \
				  $(LINK_LIBS)

all: $(OBJECTS_DIR)
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_hw_bcs.c \
	    -DUSE_ONETIME_WALLET \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_hw_bcs_onetime
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_hw_bcs.c \
	    -DUSE_CREATE_PERSIST_WALLET \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_hw_bcs_persist	
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) demo_hw_bcs.c \
	    -DUSE_LOAD_PERSIST_WALLET \
		$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/demo_hw_bcs_load		

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)
	
clean:
	-$(BOAT_RM) $(OBJECTS_DIR)/*
