# Select one of the crypto-dep and platform

.PHONY:all crypto_default storage keystore platform 

ifeq ($(SOFT_CRYPTO), CRYPTO_DEFAULT)
    BUILD_DEP :=  storage crypto_default keystore platform 
else
    BUILD_DEP :=
endif

all: $(BUILD_DEP) 


crypto_default:
	make -C $(BOAT_BASE_DIR)/vendor/common/crypto/crypto_default all

storage:
	make -C $(BOAT_BASE_DIR)/vendor/common/storage all

platform:
	make -C $(BOAT_BASE_DIR)/vendor/platform all

keystore:
	make -C $(BOAT_BASE_DIR)/vendor/keystore all
	
clean:
	make -C $(BOAT_BASE_DIR)/vendor/common/crypto/crypto_default clean
	make -C $(BOAT_BASE_DIR)/vendor/common/storage clean
	make -C $(BOAT_BASE_DIR)/vendor/platform clean 
