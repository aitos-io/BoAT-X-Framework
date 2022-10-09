# Source and Objects
SOURCES = $(wildcard *.c)
OBJECTS_DIR = $(BOAT_BUILD_DIR)/tests
OBJECTS = $(patsubst %.c,$(OBJECTS_DIR)/%.o,$(SOURCES))

.PHONY: tests_BoAT_chainmaker_linuxDefault tests_BoAT_ethereum_linuxDefault tests_BoAT_fabric_linuxDefault tests_BoAT_platon_linuxDefault cleanchaimakerlinux cleanethereumlinux cleanfabriclinux cleanplatonlinux cleanplatonelinux

all: tests_BoAT_chainmaker_linuxDefault tests_BoAT_ethereum_linuxDefault tests_BoAT_fabric_linuxDefault tests_BoAT_platon_linuxDefault tests_BoAT_platone_linuxDefault $(OBJECTS_DIR) $(OBJECTS)

tests_BoAT_chainmaker_linuxDefault:
ifeq ($(BOAT_TEST_USE_CHAINMAKER), 1)
	make -C BoAT_chainmaker_linuxDefault all
endif

tests_BoAT_ethereum_linuxDefault:
ifeq ($(BOAT_TEST_USE_ETHEREUM), 1)	
	make -C BoAT_ethereum_linuxDefault all
endif

tests_BoAT_fabric_linuxDefault:
ifeq ($(BOAT_TEST_USE_FABRIC), 1)	
	make -C  BoAT_fabric_linuxDefault all
endif	

tests_BoAT_platon_linuxDefault:
ifeq ($(BOAT_TEST_USE_PLATON), 1)	
	make -C  BoAT_platon_linuxDefault all
endif	

tests_BoAT_platone_linuxDefault:
ifeq ($(BOAT_TEST_USE_PLATONE), 1)	
	make -C  BoAT_platone_linuxDefault all
endif	
	

$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)

$(OBJECTS_DIR)/%.o:%.c
	$(CC)  -c  $(BOAT_CFLAGS)  $< -o $@

clean: cleanchaimakerlinux cleanethereumlinux cleanfabriclinux cleanplatonlinux cleanplatonelinux
	-$(BOAT_RM) $(OBJECTS)

cleanchaimakerlinux:
	make -C BoAT_chainmaker_linuxDefault clean

cleanethereumlinux:
	make -C BoAT_ethereum_linuxDefault clean
	
cleanfabriclinux:
	make -C BoAT_fabric_linuxDefault clean

cleanplatonlinux:
	make -C BoAT_platon_linuxDefault clean

cleanplatonelinux:
	make -C BoAT_platone_linuxDefault clean