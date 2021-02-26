# Environment-specific Settings
include external.env

# Commands
BOAT_RM := rm -rf
BOAT_MKDIR := mkdir
BOAT_FIND := find

export BOAT_RM BOAT_MKDIR BOAT_FIND


# Check gcc version

ifneq (,$(CC))
    GCCVERSION := $(shell $(CC) -v 2>&1)
else
    GCCVERSION := "NONE"
endif

ifneq (,$(findstring arm,$(GCCVERSION)))         # Target: arm-oe-linux-gnueabi
    COMPILER_TYPE = "ARM"
else ifneq (,$(findstring linux,$(GCCVERSION)))  # Target: x86_64-redhat-linux
    COMPILER_TYPE = "LINUX"
else ifneq (,$(findstring cygwin,$(GCCVERSION))) # Target: x86_64-pc-cygwin
    COMPILER_TYPE = "CYGWIN"
else
    COMPILER_TYPE = "NOTSUPPORT"                 # Not supported
endif


# Environment Language
LANG := en_US  # zh_CN.UTF-8


# Directories
BOAT_BASE_DIR := $(CURDIR)
BOAT_SDK_DIR := $(BOAT_BASE_DIR)/sdk
BOAT_LIB_DIR := $(BOAT_BASE_DIR)/lib
BOAT_BUILD_DIR := $(BOAT_BASE_DIR)/build
HWDEP_BASE_DIR := $(BOAT_BASE_DIR)/hwdep/$(HW_TARGET)


# Compiling Flags

# Target-independent Flags
BOAT_INCLUDE :=   -I$(BOAT_BASE_DIR)/include \
                  -I$(BOAT_SDK_DIR) \
                  -I$(BOAT_SDK_DIR)/include \
				  -I$(BOAT_SDK_DIR)/platform \
                  -I$(BOAT_SDK_DIR)/protocol \
                  -I$(BOAT_SDK_DIR)/protocol/common \
				  -I$(BOAT_SDK_DIR)/protocol/boathlfabric \
                  -I$(BOAT_SDK_DIR)/protocol/boathlfabric/protos \
				  -I$(BOAT_SDK_DIR)/protocol/boatethereum \
				  -I$(BOAT_SDK_DIR)/protocol/boatplaton \
				  -I$(BOAT_SDK_DIR)/protocol/boatplatone \
				  -I$(BOAT_SDK_DIR)/protocol/boatfiscobcos \
                  -I$(BOAT_SDK_DIR)/rpc \
				  -I$(BOAT_SDK_DIR)/third-party/cJSON \
                  -I$(BOAT_SDK_DIR)/third-party/nghttp2/include \
                  -I$(BOAT_SDK_DIR)/third-party/protobuf-c/include \
                  -I$(BOAT_BASE_DIR)/include \
                  -I$(BOAT_BASE_DIR)/contract/generated \
                  -I$(BOAT_BASE_DIR)/demo \
				  -I$(BOAT_BASE_DIR)/tests \
                  $(EXTERNAL_INC)

BOAT_CSTD_FLAGS := -std=c99
#BOAT_OPTIMIZATION_FLAGS := -g #-Os 
BOAT_OPTIMIZATION_FLAGS := -Os
BOAT_WARNING_FLAGS := -Wall
BOAT_DEFINED_MACROS := #-DDEBUG_LOG

BOAT_COMMON_LINK_FLAGS := -Wl,-Map,$(BOAT_BUILD_DIR)/boat.map


# Target-specific Flags
ifeq ($(COMPILER_TYPE), "ARM")
    TARGET_SPEC_CFLAGS := -mthumb
    TARGET_SPEC_LIBS := -lcurl
    TARGET_SPEC_LINK_FLAGS :=
else ifeq ($(COMPILER_TYPE), "LINUX")
    TARGET_SPEC_CFLAGS := -ffunction-sections -fdata-sections
    TARGET_SPEC_LIBS := -lcurl
    TARGET_SPEC_LINK_FLAGS := -Wl,-gc-sections
else ifeq ($(COMPILER_TYPE), "CYGWIN")
    TARGET_SPEC_CFLAGS :=
    TARGET_SPEC_LIBS := -lcurl
    TARGET_SPEC_LINK_FLAGS :=
else
    TARGET_SPEC_CFLAGS :=
    TARGET_SPEC_LIBS :=
    TARGET_SPEC_LINK_FLAGS :=
endif


# Hardware-specific Flags
BOAT_INCLUDE += -I$(BOAT_BASE_DIR)/hwdep/$(HW_TARGET)/crypto/mbedTLS/include \
                -I$(BOAT_BASE_DIR)/hwdep/$(HW_TARGET)/keccak \
                -I$(BOAT_BASE_DIR)/hwdep/$(HW_TARGET)/storage


# Combine FLAGS
BOAT_CFLAGS := $(TARGET_SPEC_CFLAGS) \
               $(BOAT_INCLUDE) \
               $(BOAT_CSTD_FLAGS) \
               $(BOAT_OPTIMIZATION_FLAGS) \
               $(BOAT_WARNING_FLAGS) \
               $(BOAT_DEFINED_MACROS)

BOAT_LFLAGS := $(BOAT_COMMON_LINK_FLAGS) $(TARGET_SPEC_LINK_FLAGS)
LINK_LIBS := $(EXTERNAL_LIBS) $(TARGET_SPEC_LIBS)


export HW_TARGET
export BOAT_BASE_DIR
export BOAT_LIB_DIR
export BOAT_BUILD_DIR
export HWDEP_BASE_DIR
export BOAT_CFLAGS
export BOAT_LFLAGS
export LINK_LIBS

.PHONY: all boatlibs createdir boatwalletlib hwdeplib contractlib demo tests clean cleanboatwallet cleanhwdep cleancontract cleantests

all: boatlibs demo tests

boatlibs: createdir boatwalletlib hwdeplib

createdir:
	$(BOAT_MKDIR) -p $(BOAT_LIB_DIR)
	$(BOAT_MKDIR) -p $(BOAT_BUILD_DIR)

boatwalletlib:
	if [ -d "$(BOAT_BASE_DIR)/sdk" ]; then \
	make -C $(BOAT_BASE_DIR)/sdk all; \
	fi

hwdeplib:
	if [ -d "$(BOAT_BASE_DIR)/hwdep" ]; then \
	make -C $(BOAT_BASE_DIR)/hwdep all; \
	fi

contractlib:
	make -C $(BOAT_BASE_DIR)/contract all 

demo: boatlibs contractlib
	make -C $(BOAT_BASE_DIR)/demo all

tests: boatlibs contractlib
	if [ -d "$(BOAT_BASE_DIR)/tests" ]; then \
	make -C $(BOAT_BASE_DIR)/tests all; \
	fi

clean: cleanboatwallet cleanhwdep cleancontract cleandemo cleantests
	-$(BOAT_RM) $(BOAT_BUILD_DIR)

cleanboatwallet:
	if [ -d "$(BOAT_BASE_DIR)/sdk" ]; then \
		make -k -C $(BOAT_BASE_DIR)/sdk clean; \
	fi

cleanhwdep:
	if [ -d "$(BOAT_BASE_DIR)/hwdep" ]; then \
	make -C $(BOAT_BASE_DIR)/hwdep clean; \
	fi

cleancontract:
	make -C $(BOAT_BASE_DIR)/contract clean

cleandemo:
	make -C $(BOAT_BASE_DIR)/demo clean

cleantests:
	if [ -d "$(BOAT_BASE_DIR)/tests" ]; then \
	make -C $(BOAT_BASE_DIR)/tests clean; \
	fi
