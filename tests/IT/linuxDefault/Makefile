all: testkeypair testEthNetwork
	
testkeypair:
	make -C keypair all

testEthNetwork:
ifeq ($(BOAT_PROTOCOL_USE_ETHEREUM),1)
	make -C EthNetwork all
endif

clean:
	make -C keypair clean
	make -C EthNetwork clean
