# Source and Objects

SOURCES = $(wildcard *.c)
OBJECTS_DIR = $(BOAT_BUILD_DIR)/contract
OBJECTS = $(patsubst %.c,$(OBJECTS_DIR)/%.o,$(SOURCES))

LIBNAME = $(BOAT_LIB_DIR)/libboatcontract.a

all: $(LIBNAME)

$(LIBNAME): $(OBJECTS_DIR) $(OBJECTS)
	$(AR) r $(LIBNAME) $(OBJECTS)
	
$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)

$(OBJECTS_DIR)/%.o:%.c
	$(CC) -c $(BOAT_CFLAGS) $< -o $@


clean:
	-$(BOAT_RM) $(OBJECTS)
	-$(BOAT_RM) $(LIBNAME)
