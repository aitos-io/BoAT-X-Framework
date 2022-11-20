all: testkeypair testnetwork testblockchain
	
testkeypair:
	make -C keypair all

testnetwork:
	make -C network all

testblockchain:
	make -C blockchain

clean:
	make -C keypair clean
	make -C network clean
	make -C blockchain clean