# Source and Objects  

SOURCES = $(wildcard *.c)
OBJECTS_DIR = $(BOAT_BUILD_DIR)/vendor/platform/$(PLATFORM_TARGET)/src/port_crypto_default
OBJECTS = $(patsubst %.c,$(OBJECTS_DIR)/%.o,$(SOURCES))


all: $(OBJECTS_DIR) $(OBJECTS)


$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)

$(OBJECTS_DIR)/%.o:%.c
	$(CC) -c $(BOAT_CFLAGS) $< -o $@


clean:
	-$(BOAT_RM) $(OBJECTS)
