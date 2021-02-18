# Source and Objects

SOURCES = $(wildcard *.c)
OBJECTS_DIR = $(BOAT_BUILD_DIR)/sdk/protocol
OBJECTS = $(patsubst %.c,$(OBJECTS_DIR)/%.o,$(SOURCES))


all: protocol_common $(OBJECTS_DIR) $(OBJECTS)


protocol_common:
	make -C common all

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)

$(OBJECTS_DIR)/%.o:%.c
	$(CC) -c $(BOAT_CFLAGS) $< -o $@


clean:
	-$(BOAT_RM) $(OBJECTS)
	make -C common clean
