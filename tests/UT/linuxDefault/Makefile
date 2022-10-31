all: testkeypair testsEthNetwork testsFiscobcosNetwork testsPlatonNetwork
	
testkeypair:
	make -C keypair all
testsEthNetwork:
ifeq ($(BOAT_PROTOCOL_USE_ETHEREUM),1)
	make -C EthNetwork all
endif

testsFiscobcosNetwork:
ifeq ($(BOAT_PROTOCOL_USE_FISCOBCOS),1)
	make -C FiscobcosNetwork all
endif

testsPlatonNetwork:
ifeq ($(BOAT_PROTOCOL_USE_PLATON),1)
	make -C PlatonNetwork all
endif

clean:
	make -C keypair clean
	make -C EthNetwork clean
	make -C FiscobcosNetwork clean
	make -C PlatonNetwork clean