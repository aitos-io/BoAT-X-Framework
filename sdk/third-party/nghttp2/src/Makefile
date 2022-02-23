# Source and Objects

SOURCES = $(wildcard *.c)
OBJECTS_DIR = $(BOAT_BUILD_DIR)/sdk/third-party/nghttp2
OBJECTS = $(patsubst %.c,$(OBJECTS_DIR)/%.o,$(SOURCES))

all:$(OBJECTS_DIR) $(OBJECTS)

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)

$(OBJECTS_DIR)/%.o:%.c
	$(CC) -c -DHAVE_ARPA_INET_H $(BOAT_CFLAGS) $< -o $@

clean:
	-$(BOAT_RM) $(OBJECTS)
