# Source and Objects


all:
ifeq ($(BOAT_PROTOCOL_USE_HLFABRIC), 1) 
	make -C http2intf all 
endif
	make -C web3intf  all
	
clean:
ifeq ($(BOAT_PROTOCOL_USE_HLFABRIC), 1)
	make -C http2intf clean
endif
	make -C web3intf  clean
