# Source and Objects

OBJECTS_DIR  = $(BOAT_BUILD_DIR)/sdk/third-party/cJSON \
               $(BOAT_BUILD_DIR)/sdk/third-party/nghttp2 \
               $(BOAT_BUILD_DIR)/sdk/third-party/protobuf-c

all:
	make -C cJSON all
	make -C nghttp2/src all
	make -C protobuf-c/src all
	
clean:
	make -C cJSON clean
	make -C nghttp2/src clean
	make -C protobuf-c/src clean
