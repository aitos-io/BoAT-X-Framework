all: testkeypair testsEthNetwork testsFiscobcosNetwork testsPlatonNetwork testsPlatoneNetwork testsQuorumNetwork testVenachainNetwork
	
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

testsPlatoneNetwork:
ifeq ($(BOAT_PROTOCOL_USE_PLATONE),1)
	make -C PlatoneNetwork all
endif

testsQuorumNetwork:
ifeq ($(BOAT_PROTOCOL_USE_QUORUM),1)
	make -C QuorumNetwork all
endif

testVenachainNetwork:
ifeq ($(BOAT_PROTOCOL_USE_VENACHAIN),1)
	make -C VenachainNetwork all
endif

clean:
	make -C keypair clean
	make -C EthNetwork clean
	make -C FiscobcosNetwork clean
	make -C PlatonNetwork clean
	make -C PlatoneNetwork clean
	make -C QuorumNetwork clean
	make -C VenachainNetwork clean