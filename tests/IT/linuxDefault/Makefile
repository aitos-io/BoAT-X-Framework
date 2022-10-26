all: testkeypair testEthAccount
	
testkeypair:
	make -C keypair all

testEthAccount:
ifeq ($(BOAT_PROTOCOL_USE_ETHEREUM),1)
	make -C EthAccount all
endif

clean:
	make -C keypair clean
	make -C EthAccount clean
