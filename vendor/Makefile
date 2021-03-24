# Select one of the hardware

.PHONY:all mbedTLS keccak platform

all: mbedTLS keccak platform

mbedTLS:
	make -C $(BOAT_BASE_DIR)/vendor/common/crypto/mbedTLS all

keccak:	
	make -C $(BOAT_BASE_DIR)/vendor/common/crypto/keccak all

platform:
	@echo ++++++++++++++++++++++++++++++++++---
	make -C $(BOAT_BASE_DIR)/vendor/platform/$(PLATFORM_TARGET) all
	
clean:	
	make -C $(BOAT_BASE_DIR)/vendor/common/crypto/mbedTLS clean
	make -C $(BOAT_BASE_DIR)/vendor/common/crypto/keccak clean
	make -C $(BOAT_BASE_DIR)/vendor/platform/$(PLATFORM_TARGET) clean 
