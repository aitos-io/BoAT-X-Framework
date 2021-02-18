# Source and Objects

SOURCES = $(wildcard *.c)
OBJECTS_DIR = $(BOAT_BUILD_DIR)/demo
OBJECTS = $(patsubst %.c,$(OBJECTS_DIR)/%.o,$(SOURCES))


all: $(OBJECTS_DIR) $(OBJECTS)
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) -o $(BOAT_BUILD_DIR)/demo/boatdemo $(OBJECTS) \
		$(BOAT_LIB_DIR)/libboatcontract.a \
		$(BOAT_LIB_DIR)/libboatwallet.a \
		$(BOAT_LIB_DIR)/libboathwdep.a \
		$(LINK_LIBS)

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)

$(OBJECTS_DIR)/%.o:%.c
	$(CC) -c $(BOAT_CFLAGS) $(HWDEP_INC) $< -o $@


clean:
	-$(BOAT_RM) $(OBJECTS)
	-$(BOAT_RM) $(BOAT_BUILD_DIR)/demo/boatdemo
	-$(BOAT_RM) $(BOAT_BUILD_DIR)/demo/boatdemo.map