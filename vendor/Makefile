# Select one of the hardware


all:mbedTLS keccak

mbedTLS:
	make -C $(BOAT_BASE_DIR)/vendor/common/crypto/mbedTLS all
keccak:	
	make -C $(BOAT_BASE_DIR)/vendor/common/keccak all

clean:	
	make -C $(BOAT_BASE_DIR)/vendor/common/crypto/mbedTLS clean
	make -C $(BOAT_BASE_DIR)/vendor/common/keccak clean
