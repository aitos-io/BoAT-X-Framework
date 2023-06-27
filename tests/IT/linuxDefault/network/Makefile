all:  testEthNetwork testFiscobcosNetwork testPlatonNetwork testPlatoneNetwork testQuorumNetwork testVenachainNetwork testFabricNetwork testHwbcsNetwork testCitaNetwork testChainmakerNetwork
	

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

testChainmakerNetwork:
ifneq ($(BOAT_PROTOCOL_USE_CHAINMAKER_V1)_$(BOAT_PROTOCOL_USE_CHAINMAKER_V2),0_0)
	make -C ChainmakerNetwork all
endif

clean:
	make -C EthNetwork clean
	make -C FiscobcosNetwork clean
	make -C PlatonNetwork clean
	make -C PlatoneNetwork clean
	make -C QuorumNetwork clean
	make -C VenachainNetwork clean
	make -C FabricNetwork clean
	make -C HwbcsNetwork clean
	make -C CitaNetwork clean
	make -C ChainmakerNetwork clean
