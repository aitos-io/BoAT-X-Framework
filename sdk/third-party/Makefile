# Source and Objects


all:
# If BOAT_PROTOCOL_USE_ETHEREUM, BOAT_PROTOCOL_USE_PLATONE, BOAT_PROTOCOL_USE_PLATONE, BOAT_PROTOCOL_USE_FISCOBCOS  has one setted to 1, 
# then build cJSON
ifeq ($(findstring $(BOAT_PROTOCOL_USE_ETHEREUM)$(BOAT_PROTOCOL_USE_PLATON)$(BOAT_PROTOCOL_USE_PLATONE)$(BOAT_PROTOCOL_USE_FISCOBCOS), 0000),)
ifeq ($(CJSON_LIBRARY),CJSON_DEFAULT)
	make -C cJSON all
endif
endif

ifeq ($(BOAT_PROTOCOL_USE_HLFABRIC), 1)
	make -C nghttp2/src all
	make -C protobuf-c/src all
endif

ifeq ($(BOAT_PROTOCOL_USE_CHAINMAKER), 1)
	make -C nghttp2/src all
	make -C protobuf-c/src all
endif
	
clean:
	make -C cJSON clean
	make -C nghttp2/src clean
	make -C protobuf-c/src clean
