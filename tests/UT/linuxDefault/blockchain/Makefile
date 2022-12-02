all: testfabric testvenachain testplatone
	
testfabric:
	make -C FabricBlockchain all

testvenachain:
	make -C venachain all

testplatone:
	make -C platone all

clean:
	make -C FabricBlockchain clean
	make -C venachain clean
	make -C platone