# Source and Objects
SOURCES = $(wildcard *.c)
OBJECTS_DIR = $(BOAT_BUILD_DIR)/tests
OBJECTS = $(patsubst %.c,$(OBJECTS_DIR)/%.o,$(SOURCES))

.PHONY: tests_BoAT_chainmaker_linuxDefault

all: tests_BoAT_chainmaker_linuxDefault $(OBJECTS_DIR) $(OBJECTS)

tests_BoAT_chainmaker_linuxDefault:
ifeq ($(BOAT_PROTOCOL_USE_CHAINMAKER), 1)
	make -C  BoAT_chainmaker_linuxDefault all
endif

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)

$(OBJECTS_DIR)/%.o:%.c
	$(CC)  -c  $(BOAT_CFLAGS)  $< -o $@

clean:
	-$(BOAT_RM) $(OBJECTS)
	make -C BoAT_chainmaker_linuxDefault clean