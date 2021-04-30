# Source and Objects


all:
	make -C cJSON all
ifeq ($(BOAT_PROTOCOL_USE_HLFABRIC), 1)
	make -C nghttp2/src all
	make -C protobuf-c/src all
endif
	
clean:
	make -C cJSON clean
ifeq ($(BOAT_PROTOCOL_USE_HLFABRIC), 1)
	make -C nghttp2/src clean
	make -C protobuf-c/src clean
endif
