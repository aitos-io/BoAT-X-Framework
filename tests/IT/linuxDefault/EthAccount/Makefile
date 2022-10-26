
DEMO_FOLDER    = wallet_test
# Source and Objects
SOURCES = $(wildcard *.c)
OBJECTS_DIR = $(BOAT_BUILD_DIR)/tests/IT/EthAccount
OBJECTS = $(patsubst %.c,$(OBJECTS_DIR)/%.o,$(SOURCES))

#dependence libs
DEPENDENCE_LIBS = $(BOAT_LIB_DIR)/libboatwallet.a \
				  $(BOAT_LIB_DIR)/libboatvendor.a \
				  $(LINK_LIBS)

.PHONY: tests_EthAccount_linuxDefault   cleanplatonlinux

all:  $(OBJECTS_DIR) $(OBJECTS) tests_EthAccount_linuxDefault
	
tests_EthAccount_linuxDefault:
	$(CC) $(BOAT_CFLAGS) $(BOAT_LFLAGS) $(CONTRACT_INCLUDE) $(CONTRACT_SOURCES) $(SOURCES)\
	 -DUSE_PRIKEY_FORMAT_EXTERNAL_INJECTION_NATIVE \
	$(DEPENDENCE_LIBS) -o $(OBJECTS_DIR)/eth_account_test



$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)

$(OBJECTS_DIR)/%.o:%.c
	$(CC)  -c  $(BOAT_CFLAGS)  $< -o $@

clean:  
	-$(BOAT_RM) $(OBJECTS)

