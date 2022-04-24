# Source and Objects
SOURCES = $(wildcard *.c)
OBJECTS_DIR = $(BOAT_BUILD_DIR)/tests
OBJECTS = $(patsubst %.c,$(OBJECTS_DIR)/%.o,$(SOURCES))

.PHONY: tests_BoAT_chainmaker_linuxDefault tests_BoAT_ethereum_linuxDefault tests_BoAT_fabric_linuxDefault cleanchaimakerlinux cleanethereumlinux

all: tests_BoAT_chainmaker_linuxDefault tests_BoAT_ethereum_linuxDefault tests_BoAT_fabric_linuxDefault $(OBJECTS_DIR) $(OBJECTS)

tests_BoAT_chainmaker_linuxDefault:
ifeq ($(BOAT_TEST_USE_CHAINMAKER), 1)
	make -C BoAT_chainmaker_linuxDefault all
endif

tests_BoAT_ethereum_linuxDefault:
	make -C BoAT_ethereum_linuxDefault all

tests_BoAT_fabric_linuxDefault:
	make -C  BoAT_fabric_linuxDefault all

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)

$(OBJECTS_DIR)/%.o:%.c
	$(CC)  -c  $(BOAT_CFLAGS)  $< -o $@

clean: cleanchaimakerlinux cleanethereumlinux
	-$(BOAT_RM) $(OBJECTS)

cleanchaimakerlinux:
	make -C BoAT_chainmaker_linuxDefault clean

cleanethereumlinux:
	make -C BoAT_ethereum_linuxDefault clean
	
cleanfabriclinux:
	make -C BoAT_fabric_linuxDefault clean

