all: testwallet testEthAccount
	
testwallet:
	make -C wallet all

testEthAccount:
ifeq ($(BOAT_PROTOCOL_USE_ETHEREUM),1)
	make -C EthAccount all
endif

clean:
	make -C wallet clean
	make -C EthAccount clean
