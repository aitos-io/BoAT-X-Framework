SOURCES = $(wildcard *.c)
OBJECTS_DIR = $(BOAT_BUILD_DIR)/sdk/protocol/boatcita
OBJECTS = $(patsubst %.c,$(OBJECTS_DIR)/%.o,$(SOURCES))

all: $(OBJECTS_DIR) $(OBJECTS) protos-objects

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)

$(OBJECTS_DIR)/%.o:%.c
	$(CC) -c $(BOAT_CFLAGS) $< -o $@
	$(BOAT_COPY) $(SOURCES) $(OBJECTS_DIR)

protos-objects:
	make -C protos all

clean:
	-$(BOAT_RM) $(OBJECTS)
	make -C protos clean
