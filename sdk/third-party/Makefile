# Source and Objects


all:
# If BOAT_PROTOCOL_USE_ETHEREUM, BOAT_PROTOCOL_USE_PLATON, BOAT_PROTOCOL_USE_PLATONE, BOAT_PROTOCOL_USE_FISCOBCOS  BOAT_PROTOCOL_USE_VENACHAIN has one setted to 1, 
# then build cJSON
ifeq ($(findstring $(BOAT_PROTOCOL_USE_ETHEREUM)$(BOAT_PROTOCOL_USE_PLATON)$(BOAT_PROTOCOL_USE_PLATONE)$(BOAT_PROTOCOL_USE_FISCOBCOS)$(BOAT_PROTOCOL_USE_VENACHAIN)$(BOAT_PROTOCOL_USE_QUORUM)$(BOAT_PROTOCOL_USE_CITA), 000000),)
ifeq ($(CJSON_LIBRARY),CJSON_DEFAULT)
	make -C cJSON all
endif
endif

ifneq ($(BOAT_PROTOCOL_USE_HLFABRIC)_$(BOAT_PROTOCOL_USE_CHAINMAKER_V1)_$(BOAT_PROTOCOL_USE_CHAINMAKER_V2)_$(BOAT_PROTOCOL_USE_HWBCS),0_0_0_0)
	make -C nghttp2/src all
	make -C protobuf-c/src all
	make -C protos all
endif

ifneq ($(BOAT_PROTOCOL_USE_CITA), 0)
	make -C protobuf-c/src all
endif

	
clean:
	make -C cJSON clean
	make -C nghttp2/src clean
	make -C protobuf-c/src clean
	make -C protos clean
