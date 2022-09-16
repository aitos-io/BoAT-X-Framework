# Select one of the crypto-dep and platform

.PHONY:all mbedTLS crypto_default keccak storage platform

ifeq ($(SOFT_CRYPTO), CRYPTO_DEFAULT)
    BUILD_DEP :=  keccak storage crypto_default platform
else ifeq ($(SOFT_CRYPTO), CRYPTO_MBEDTLS)
    BUILD_DEP :=  keccak  storage  platform
else
    BUILD_DEP :=
endif

all: $(BUILD_DEP) 


crypto_default:
	make -C $(BOAT_BASE_DIR)/vendor/common/crypto/crypto_default all

keccak:	
	make -C $(BOAT_BASE_DIR)/vendor/common/crypto/keccak all

storage:
	make -C $(BOAT_BASE_DIR)/vendor/common/storage all

platform:
	make -C $(BOAT_BASE_DIR)/vendor/platform all
	
clean:
	make -C $(BOAT_BASE_DIR)/vendor/common/crypto/keccak clean
	make -C $(BOAT_BASE_DIR)/vendor/common/storage clean
	make -C $(BOAT_BASE_DIR)/vendor/platform clean 
