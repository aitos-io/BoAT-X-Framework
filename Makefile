# Directories
BOAT_BASE_DIR  := $(CURDIR)
BOAT_SDK_DIR   := $(BOAT_BASE_DIR)/sdk
BOAT_LIB_DIR   := $(BOAT_BASE_DIR)/lib
BOAT_BUILD_DIR := $(BOAT_BASE_DIR)/build

# Set chain support
# Set to 1 to enable releated chain, to 0 to disable
BOAT_PROTOCOL_USE_ETHEREUM   ?= 1
BOAT_PROTOCOL_USE_PLATON     ?= 1
BOAT_PROTOCOL_USE_PLATONE    ?= 1
BOAT_PROTOCOL_USE_FISCOBCOS  ?= 1
BOAT_PROTOCOL_USE_HLFABRIC   ?= 1
BOAT_PROTOCOL_USE_HWBCS      ?= 1
BOAT_PROTOCOL_USE_CHAINMAKER ?= 1
BOAT_DISCOVERY_PEER_QUERY    ?= 1
BOAT_PROTOCOL_USE_VENACHAIN  ?= 1
BOAT_PROTOCOL_USE_QUORUM     ?= 1
BOAT_PROTOCOL_USE_CITA       ?= 1

# Chain config check
ifeq ($(BOAT_PROTOCOL_USE_ETHEREUM)_$(BOAT_PROTOCOL_USE_PLATON)_$(BOAT_PROTOCOL_USE_PLATONE)_$(BOAT_PROTOCOL_USE_FISCOBCOS)_$(BOAT_PROTOCOL_USE_HLFABRIC)_$(BOAT_PROTOCOL_USE_HWBCS)_$(BOAT_PROTOCOL_USE_CHAINMAKER)_$(BOAT_PROTOCOL_USE_VENACHAIN)_$(BOAT_PROTOCOL_USE_QUORUM)_$(BOAT_PROTOCOL_USE_CITA), 0_0_0_0_0_0_0_0_0)
    $(error Select at least one chain)
endif


# Set parameter to scripts
SCRIPTS_PARAM += "BOAT_PROTOCOL_USE_ETHEREUM=$(BOAT_PROTOCOL_USE_ETHEREUM)" \
                 "BOAT_PROTOCOL_USE_PLATON=$(BOAT_PROTOCOL_USE_PLATON)" \
                 "BOAT_PROTOCOL_USE_PLATONE=$(BOAT_PROTOCOL_USE_PLATONE)" \
                 "BOAT_PROTOCOL_USE_FISCOBCOS=$(BOAT_PROTOCOL_USE_FISCOBCOS)" \
                 "BOAT_PROTOCOL_USE_HLFABRIC=$(BOAT_PROTOCOL_USE_HLFABRIC)" \
                 "BOAT_PROTOCOL_USE_HWBCS=$(BOAT_PROTOCOL_USE_HWBCS)" \
		         "BOAT_PROTOCOL_USE_CHAINMAKER=$(BOAT_PROTOCOL_USE_CHAINMAKER)" \
                 "BOAT_DISCOVERY_PEER_QUERY=$(BOAT_DISCOVERY_PEER_QUERY)" \
                 "BOAT_PROTOCOL_USE_VENACHAIN=$(BOAT_PROTOCOL_USE_VENACHAIN)" \
                 "BOAT_PROTOCOL_USE_QUORUM=$(BOAT_PROTOCOL_USE_QUORUM)" \
                 "BOAT_PROTOCOL_USE_CITA=$(BOAT_PROTOCOL_USE_CITA)" 


# Platform target
# The valid option value of PLATFORM_TARGET list as below:
# - linux-default             : Default linux platform
# - Fibocom-L610              : Fibocom's LTE Cat.1 module
# - Quectel-BG95              : Quectel's NB-IoT/GSM module
# - Neoway-N58                : Neoway's LTE Cat.1 module
# - YanFei-CUIot-MZ-6         : China Unicom's LTE Cat.1 module
# - ChinaMobile-ML302         : China Mobile's LTE Cat.1 module
# - ChinaMobile-ML302v2       : China Mobile's LTE Cat.1 module API version 2
# - MTK-MT3620                : MTK MT3620
# - XinYi-XY1100              : XY1100
# - Fibocom-MA510             : MA510
# - Simcom-A7670C             : A7670C
PLATFORM_TARGET ?= Simcom-A7670C

# Environment-specific Settings
include $(BOAT_BASE_DIR)/vendor/platform/$(PLATFORM_TARGET)/external.env


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

# Compiling Flags

# Target-independent Flags
BOAT_INCLUDE :=   -I$(BOAT_BASE_DIR)/include \
                  -I$(BOAT_SDK_DIR) \
                  -I$(BOAT_SDK_DIR)/include \
				  -I$(BOAT_SDK_DIR)/platform \
                  -I$(BOAT_SDK_DIR)/protocol \
                  -I$(BOAT_SDK_DIR)/protocol/common/web3intf \
                  -I$(BOAT_BASE_DIR)/vendor/platform/include \
                  -I$(BOAT_BASE_DIR)/vendor/platform/$(PLATFORM_TARGET)/src/log \
                  -I$(BOAT_BASE_DIR)/contract/generated \
                  -I$(BOAT_BASE_DIR)/demo \
				  -I$(BOAT_BASE_DIR)/tests \
                  $(EXTERNAL_INC)

ifeq ($(BOAT_PROTOCOL_USE_ETHEREUM),1)         
BOAT_INCLUDE +=  -I$(BOAT_SDK_DIR)/network/ethereum \
                -I$(BOAT_SDK_DIR)/protocol/boatethereum 
endif

ifeq ($(BOAT_PROTOCOL_USE_HLFABRIC),1)         
BOAT_INCLUDE +=  -I$(BOAT_SDK_DIR)/network/hlfabric \
                -I$(BOAT_SDK_DIR)/protocol/boathlfabric  \
                -I$(BOAT_SDK_DIR)/protocol/common/http2intf \
                -I$(BOAT_SDK_DIR)/third-party/protos \
                -I$(BOAT_SDK_DIR)/third-party/nghttp2/include \
                -I$(BOAT_SDK_DIR)/third-party/protobuf-c/include

endif

ifeq ($(BOAT_PROTOCOL_USE_PLATONE),1)         
BOAT_INCLUDE +=  -I$(BOAT_SDK_DIR)/network/platone \
                -I$(BOAT_SDK_DIR)/protocol/boatplatone
endif

ifeq ($(BOAT_PROTOCOL_USE_VENACHAIN),1)         
BOAT_INCLUDE +=  -I$(BOAT_SDK_DIR)/network/venachain \
                -I$(BOAT_SDK_DIR)/protocol/boatvenachain
endif

ifeq ($(BOAT_PROTOCOL_USE_PLATON),1)         
BOAT_INCLUDE +=  -I$(BOAT_SDK_DIR)/network/platon \
                -I$(BOAT_SDK_DIR)/protocol/boatplaton
endif

ifeq ($(BOAT_PROTOCOL_USE_FISCOBCOS),1)         
BOAT_INCLUDE +=  -I$(BOAT_SDK_DIR)/network/fiscobcos \
                -I$(BOAT_SDK_DIR)/protocol/boatfiscobcos
endif

ifeq ($(BOAT_PROTOCOL_USE_QUORUM),1)         
BOAT_INCLUDE +=  -I$(BOAT_SDK_DIR)/network/quorum \
                -I$(BOAT_SDK_DIR)/protocol/boatquorum
endif

ifeq ($(BOAT_PROTOCOL_USE_HWBCS),1)         
BOAT_INCLUDE +=  -I$(BOAT_SDK_DIR)/network/hwbcs \
                -I$(BOAT_SDK_DIR)/protocol/boathwbcs  \
                -I$(BOAT_SDK_DIR)/protocol/boathwbcs/protos  \
                -I$(BOAT_SDK_DIR)/protocol/common/http2intf \
                -I$(BOAT_SDK_DIR)/third-party/protos \
                -I$(BOAT_SDK_DIR)/third-party/nghttp2/include \
                -I$(BOAT_SDK_DIR)/third-party/protobuf-c/include
endif

ifeq ($(BOAT_PROTOCOL_USE_CHAINMAKER),1)         
BOAT_INCLUDE += -I$(BOAT_SDK_DIR)/network/chainmaker \
                -I$(BOAT_SDK_DIR)/protocol/boatchainmaker  \
                -I$(BOAT_SDK_DIR)/protocol/boatchainmaker/protos  \
                -I$(BOAT_SDK_DIR)/protocol/common/http2intf \
                -I$(BOAT_SDK_DIR)/third-party/protos \
                -I$(BOAT_SDK_DIR)/third-party/nghttp2/include \
                -I$(BOAT_SDK_DIR)/third-party/protobuf-c/include
endif

ifeq ($(BOAT_PROTOCOL_USE_CITA),1)         
BOAT_INCLUDE +=  -I$(BOAT_SDK_DIR)/network/cita \
                 -I$(BOAT_SDK_DIR)/protocol/boatcita \
                 -I$(BOAT_SDK_DIR)/protocol/boatcita/protos  \
                 -I$(BOAT_SDK_DIR)/third-party/protobuf-c/include
endif

BOAT_CSTD_FLAGS := -std=c99
#BOAT_OPTIMIZATION_FLAGS := -g #-Os 
BOAT_OPTIMIZATION_FLAGS := -Os
BOAT_WARNING_FLAGS := -Wall
BOAT_DEFINED_MACROS := #-DDEBUG_LOG

# BOAT_COMMON_LINK_FLAGS := -Wl,-Map,$(BOAT_BUILD_DIR)/boat.map


# Target-specific Flags
ifeq ($(COMPILER_TYPE), "ARM")
    TARGET_SPEC_CFLAGS := -mthumb -ffunction-sections -fdata-sections
    TARGET_SPEC_LIBS := 
    TARGET_SPEC_LINK_FLAGS :=
else ifeq ($(COMPILER_TYPE), "LINUX")
    TARGET_SPEC_CFLAGS := -ffunction-sections -fdata-sections 
    TARGET_SPEC_LIBS := 
    TARGET_SPEC_LINK_FLAGS := -Wl,-gc-sections
else ifeq ($(COMPILER_TYPE), "CYGWIN")
    TARGET_SPEC_CFLAGS :=
    TARGET_SPEC_LIBS := 
    TARGET_SPEC_LINK_FLAGS :=
else
    TARGET_SPEC_CFLAGS :=
    TARGET_SPEC_LIBS :=
    TARGET_SPEC_LINK_FLAGS :=
endif

# Soft-crypto Dependencies
# The valid option value of SOFT_CRYPTO list as below:
# - CRYPTO_DEFAULT      : default soft crypto algorithm
# - CRYPTO_MBEDTLS      : mbedtls crypto algorithm
# SOFT_CRYPTO ?= CRYPTO_MBEDTLS

ifeq ($(PLATFORM_TARGET), linux-default)
    SOFT_CRYPTO ?= CRYPTO_MBEDTLS
else ifeq ($(PLATFORM_TARGET), Fibocom-L610) 
    SOFT_CRYPTO ?= CRYPTO_DEFAULT
else ifeq ($(PLATFORM_TARGET), Quectel-BG95) 
    SOFT_CRYPTO ?= CRYPTO_MBEDTLS
else ifeq ($(PLATFORM_TARGET), Neoway-N58) 
    SOFT_CRYPTO ?= CRYPTO_DEFAULT
else ifeq ($(PLATFORM_TARGET), YanFei-CUIot-MZ-6) 
    SOFT_CRYPTO ?= CRYPTO_DEFAULT
else ifeq ($(PLATFORM_TARGET), ChinaMobile-ML302) 
    SOFT_CRYPTO ?= CRYPTO_DEFAULT
else ifeq ($(PLATFORM_TARGET), ChinaMobile-ML302v2) 
    SOFT_CRYPTO ?= CRYPTO_DEFAULT
else ifeq ($(PLATFORM_TARGET), XinYi-XY1100) 
    SOFT_CRYPTO ?= CRYPTO_MBEDTLS
else ifeq ($(PLATFORM_TARGET), MTK-MT3620) 
    SOFT_CRYPTO ?= CRYPTO_DEFAULT
else ifeq ($(PLATFORM_TARGET), Fibocom-MA510) 
    SOFT_CRYPTO ?= CRYPTO_DEFAULT
else ifeq ($(PLATFORM_TARGET), Simcom-A7670C) 
    SOFT_CRYPTO ?= CRYPTO_DEFAULT
else
    $(error not support this platform : $(PLATFORM_TARGET))
endif


ifeq ($(SOFT_CRYPTO), CRYPTO_DEFAULT)
    ifeq ($(BOAT_PROTOCOL_USE_HLFABRIC), 1)
    $(error $(PLATFORM_TARGET) not support fabric)
    endif
    ifeq ($(BOAT_PROTOCOL_USE_HWBCS), 1)
    $(error $(PLATFORM_TARGET) not support HWBCS)
    endif
endif

ifeq ($(SOFT_CRYPTO), CRYPTO_DEFAULT)
    BOAT_INCLUDE += -I$(BOAT_BASE_DIR)/vendor/common/crypto/crypto_default \
	                -I$(BOAT_BASE_DIR)/vendor/common/crypto/crypto_default/aes \
                    -I$(BOAT_BASE_DIR)/vendor/common/storage
else ifeq ($(SOFT_CRYPTO), CRYPTO_MBEDTLS)
    BOAT_INCLUDE += -I$(BOAT_BASE_DIR)/vendor/common/crypto/mbedTLS/include \
                    -I$(BOAT_BASE_DIR)/vendor/common/crypto/keccak \
                    -I$(BOAT_BASE_DIR)/vendor/common/storage
else
    BOAT_INCLUDE +=
endif

# cJSON Dependencies
#
# - CJSON_DEFAULT : default cJSON library
# - CJSON_OUTTER  : externally provided by users
CJSON_LIBRARY ?= CJSON_DEFAULT

ifeq ($(CJSON_LIBRARY), CJSON_DEFAULT)
    BOAT_INCLUDE += -I$(BOAT_SDK_DIR)/third-party/cJSON
endif

 
ifeq ($(BOAT_TEST), TEST_MODE)
BOAT_TEST_FLAG = -fprofile-arcs\
                 -ftest-coverage

ifeq ($(BOAT_NODES_DISCOVER), OPEN)
BOAT_DISCOVERY_PEER_QUERY    = 1
else
BOAT_DISCOVERY_PEER_QUERY    = 0
endif
endif


# Combine FLAGS
BOAT_CFLAGS := $(TARGET_SPEC_CFLAGS) \
               $(BOAT_INCLUDE) \
               $(BOAT_CSTD_FLAGS) \
               $(BOAT_OPTIMIZATION_FLAGS) \
               $(BOAT_WARNING_FLAGS) \
               $(BOAT_DEFINED_MACROS) \
               $(EXTERNAL_CFLAGS) \
               $(BOAT_TEST_FLAG)

BOAT_LFLAGS := $(BOAT_COMMON_LINK_FLAGS) $(TARGET_SPEC_LINK_FLAGS) $(EXTERNAL_LFLAGS)
LINK_LIBS := $(EXTERNAL_LIBS) $(TARGET_SPEC_LIBS)

export BOAT_PROTOCOL_USE_ETHEREUM
export BOAT_PROTOCOL_USE_PLATON
export BOAT_PROTOCOL_USE_PLATONE
export BOAT_PROTOCOL_USE_FISCOBCOS
export BOAT_PROTOCOL_USE_HLFABRIC
export BOAT_PROTOCOL_USE_HWBCS
export BOAT_PROTOCOL_USE_CHAINMAKER
export BOAT_PROTOCOL_USE_VENACHAIN
export BOAT_PROTOCOL_USE_QUORUM
export BOAT_PROTOCOL_USE_CITA
export BOAT_DISCOVERY_PEER_QUERY
export BOAT_USE_DEFAULT_CJSON


export SOFT_CRYPTO
export CJSON_LIBRARY
export PLATFORM_TARGET
export BOAT_BASE_DIR
export BOAT_LIB_DIR
export BOAT_BUILD_DIR
export BOAT_CFLAGS
export BOAT_LFLAGS
export LINK_LIBS

.PHONY: all boatlibs createdir boatwalletlib vendorlib demo tests clean cleanboatwallet cleanvendor cleantests

#all: boatlibs demo tests
all: boatlibs
	
boatlibs: createdir boatwalletlib vendorlib

createdir:
	@echo generate header file boatconfig.h...
	python ./vendor/platform/$(PLATFORM_TARGET)/scripts/gen.py $(PLATFORM_TARGET) $(SCRIPTS_PARAM)
	@echo generate done.

	$(BOAT_MKDIR) -p $(BOAT_LIB_DIR)
	$(BOAT_MKDIR) -p $(BOAT_BUILD_DIR)

boatwalletlib:
	make -C $(BOAT_BASE_DIR)/sdk all

vendorlib:
	make -C $(BOAT_BASE_DIR)/vendor all

demo: boatlibs
	make -C $(BOAT_BASE_DIR)/demo all

rulecheck: 
	cppcheck  --enable=all -i$(BOAT_BASE_DIR)/sdk/third-party/ -i$(BOAT_BASE_DIR)/vendor/common/crypto/  -i$(BOAT_BASE_DIR)/sdk/protocol/boathlfabric/protos  --force $(BOAT_BASE_DIR) 

tests: boatlibs
	make -C $(BOAT_BASE_DIR)/tests all

clean: cleanboatwallet cleanvendor cleandemo cleantests
	-$(BOAT_RM) $(BOAT_BUILD_DIR)

cleanboatwallet:
	make -C $(BOAT_BASE_DIR)/sdk clean

cleanvendor:
	make -C $(BOAT_BASE_DIR)/vendor clean

cleandemo:
	make -C $(BOAT_BASE_DIR)/demo clean

cleantests:
	make -C $(BOAT_BASE_DIR)/tests clean

