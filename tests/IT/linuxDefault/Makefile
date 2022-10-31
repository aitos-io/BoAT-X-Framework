all: testkeypair testEthNetwork testFiscobcosNetwork testPlatonNetwork
	
testkeypair:
	make -C keypair all

testEthNetwork:
ifeq ($(BOAT_PROTOCOL_USE_ETHEREUM),1)
	make -C EthNetwork all
endif

testFiscobcosNetwork:
ifeq ($(BOAT_PROTOCOL_USE_FISCOBCOS),1)
	make -C FiscobcosNetwork all
endif

testPlatonNetwork:
ifeq ($(BOAT_PROTOCOL_USE_PLATON),1)
	make -C PlatonNetwork all
endif

clean:
	make -C keypair clean
	make -C EthNetwork clean
	make -C FiscobcosNetwork clean
	make -C PlatonNetwork clean
