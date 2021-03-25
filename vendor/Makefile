# Select one of the crypto-dep and platform

.PHONY:all mbedTLS keccak storage platform

all: mbedTLS keccak storage platform 

mbedTLS:
	make -C $(BOAT_BASE_DIR)/vendor/common/crypto/mbedTLS all

keccak:	
	make -C $(BOAT_BASE_DIR)/vendor/common/crypto/keccak all

storage:
	make -C $(BOAT_BASE_DIR)/vendor/common/storage all

platform:
	make -C $(BOAT_BASE_DIR)/vendor/platform all
	
clean:	
	make -C $(BOAT_BASE_DIR)/vendor/common/crypto/mbedTLS clean
	make -C $(BOAT_BASE_DIR)/vendor/common/crypto/keccak clean
	make -C $(BOAT_BASE_DIR)/vendor/platform clean 
