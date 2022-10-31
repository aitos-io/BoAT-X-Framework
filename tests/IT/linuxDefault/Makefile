all: testkeypair testEthNetwork testFiscobcosNetwork testPlatonNetwork testPlatoneNetwork testQuorumNetwork testVenachainNetwork
	
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

testPlatoneNetwork:
ifeq ($(BOAT_PROTOCOL_USE_PLATONE),1)
	make -C PlatoneNetwork all
endif

testQuorumNetwork:
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
