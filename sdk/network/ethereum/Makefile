# Source and Objects

SOURCES = $(wildcard *.c)
# SOURCES = boatwallet.c 


OBJECTS_DIR = $(BOAT_BUILD_DIR)/sdk/network/ethereum
OBJECTS = $(patsubst %.c,$(OBJECTS_DIR)/%.o,$(SOURCES))


all: $(OBJECTS_DIR) $(OBJECTS)

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)

$(OBJECTS_DIR)/%.o:%.c
	$(CC) -c $(BOAT_CFLAGS) $< -o $@
	$(BOAT_COPY) $(SOURCES) $(OBJECTS_DIR)


clean:
	-$(BOAT_RM) $(OBJECTS)

