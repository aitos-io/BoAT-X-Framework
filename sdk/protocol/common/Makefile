# Source and Objects


all:
ifeq ($(BOAT_PROTOCOL_USE_HLFABRIC), 1) 
	make -C http2intf all 
endif

ifeq ($(BOAT_PROTOCOL_USE_CHAINMAKER), 1) 
	make -C http2intf all 
endif
# If BOAT_PROTOCOL_USE_ETHEREUM, BOAT_PROTOCOL_USE_PLATON, BOAT_PROTOCOL_USE_PLATONE, BOAT_PROTOCOL_USE_FISCOBCOS,BOAT_PROTOCOL_USE_VENACHAIN has one setted to 1, 
# then build web3intf
ifeq ($(findstring $(BOAT_PROTOCOL_USE_ETHEREUM)$(BOAT_PROTOCOL_USE_PLATON)$(BOAT_PROTOCOL_USE_PLATONE)$(BOAT_PROTOCOL_USE_FISCOBCOS)$(BOAT_PROTOCOL_USE_VENACHAIN), 0000),)
	make -C web3intf  all
endif	
	
clean:
	make -C http2intf clean
	make -C web3intf  clean
