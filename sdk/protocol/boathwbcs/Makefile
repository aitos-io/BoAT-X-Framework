# Source and Objects

SOURCES = $(wildcard *.c)
OBJECTS_DIR = $(BOAT_BUILD_DIR)/sdk/protocol/boathwbcs
OBJECTS = $(patsubst %.c,$(OBJECTS_DIR)/%.o,$(SOURCES))

all: protos-objects $(OBJECTS_DIR) $(OBJECTS) 

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)

$(OBJECTS_DIR)/%.o:%.c
	$(CC) -c $(BOAT_CFLAGS) $< -o $@

protos-objects:
	make -C protos all

clean:
	-$(BOAT_RM) $(OBJECTS)
	make -C protos clean
