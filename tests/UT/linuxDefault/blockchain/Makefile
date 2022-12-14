all: testethereum testfabric testvenachain testplatone testplaton testchainmaker
	 
testfabric:
ifeq ($(BOAT_PROTOCOL_USE_HLFABRIC),1)
	make -C fabric all
endif

testvenachain:
ifeq ($(BOAT_PROTOCOL_USE_VENACHAIN),1)
	make -C venachain all
endif

testplatone:
ifeq ($(BOAT_PROTOCOL_USE_PLATONE),1)
	make -C platone all
endif

testplaton:
ifeq ($(BOAT_PROTOCOL_USE_PLATON),1) 
	make -C platon all
endif

testethereum:
ifeq ($(BOAT_PROTOCOL_USE_ETHEREUM),1) 
	make -C ethereum all
endif

testchainmaker:
ifeq ($(BOAT_PROTOCOL_USE_CHAINMAKER_V1),1) 
	make -C chainmaker all
endif

ifeq ($(BOAT_PROTOCOL_USE_CHAINMAKER_V2),1) 
	make -C chainmaker all
endif


clean:
	make -C fabric clean
	make -C venachain clean
	make -C platone clean
	make -C platon clean
	make -C chainmaker clean