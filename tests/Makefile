# Source and Objects

SOURCES = $(wildcard *.c)
OBJECTS_DIR = $(BOAT_BUILD_DIR)/tests
OBJECTS = $(patsubst %.c,$(OBJECTS_DIR)/%.o,$(SOURCES))

DEPENDENCE_LIBS = $(BOAT_LIB_DIR)/libboatcontract.a \
				  $(BOAT_LIB_DIR)/libboatwallet.a \
				  $(BOAT_LIB_DIR)/libboatvendor.a \
				  $(LINK_LIBS) \
				  -lcheck

all: $(OBJECTS_DIR) $(OBJECTS)	
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) -o $(BOAT_BUILD_DIR)/tests/boat_10_case \
		$(BOAT_BUILD_DIR)/tests/boat_10_case.o $(DEPENDENCE_LIBS)	

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)

$(OBJECTS_DIR)/%.o:%.c
	$(CC) -c $(BOAT_CFLAGS) $< -o $@

clean:
	-$(BOAT_RM) $(OBJECTS)
	-$(BOAT_RM) $(BOAT_BUILD_DIR)/tests/*
	
	
	
#UtilityBinToHexTEST = UtilityBinToHex-test
#$(UtilityBinToHexTEST) : UtilityBinToHex-test.c
#	gcc -o $(UtilityBinToHexTEST) UtilityBinToHex-test.c ../sdk/utilities/boatutility.c -I ../include  -I../sdk/include/ -I ../sdk/platform/
#	
#.PHONY: clean
#clean :
#	rm $(UtilityBinToHexTEST)

