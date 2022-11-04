all: testkeypair testsEthNetwork testsFiscobcosNetwork testsPlatonNetwork testsPlatoneNetwork testsQuorumNetwork testVenachainNetwork testFabricNetwork testHwbcsNetwork testCitaNetwork
	
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

testFabricNetwork:
ifeq ($(BOAT_PROTOCOL_USE_HLFABRIC),1)
	make -C FabricNetwork all
endif

testHwbcsNetwork:
ifeq ($(BOAT_PROTOCOL_USE_HWBCS),1)
	make -C HwbcsNetwork all
endif

testCitaNetwork:
ifeq ($(BOAT_PROTOCOL_USE_CITA),1)
	make -C CitaNetwork all
endif

clean:
	make -C keypair clean
	make -C EthNetwork clean
	make -C FiscobcosNetwork clean
	make -C PlatonNetwork clean
	make -C PlatoneNetwork clean
	make -C QuorumNetwork clean
	make -C VenachainNetwork clean
	make -C FabricNetwork clean
	make -C HwbcsNetwork clean
	make -C CitaNetwork clean