all: testsEthNetwork testsFiscobcosNetwork testsPlatonNetwork testsPlatoneNetwork testsQuorumNetwork testVenachainNetwork testFabricNetwork testHwbcsNetwork testCitaNetwork testChainmakerNetwork testNetworkCross


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

testChainmakerNetwork:
ifneq ($(BOAT_PROTOCOL_USE_CHAINMAKER_V1)_$(BOAT_PROTOCOL_USE_CHAINMAKER_V2),0_0)
	make -C ChainmakerNetwork all
endif

testNetworkCross:
ifeq ($(BOAT_PROTOCOL_USE_ETHEREUM)_$(BOAT_PROTOCOL_USE_PLATON)_$(BOAT_PROTOCOL_USE_PLATONE)_$(BOAT_PROTOCOL_USE_FISCOBCOS)_$(BOAT_PROTOCOL_USE_HLFABRIC)_$(BOAT_PROTOCOL_USE_HWBCS)_$(BOAT_PROTOCOL_USE_CHAINMAKER)_$(BOAT_PROTOCOL_USE_VENACHAIN)_$(BOAT_PROTOCOL_USE_QUORUM)_$(BOAT_PROTOCOL_USE_CITA), 1_1_1_1_1_1_1_1_1_1)
	make -C NetworkCrossTest all
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
	make -C NetworkCrossTest clean