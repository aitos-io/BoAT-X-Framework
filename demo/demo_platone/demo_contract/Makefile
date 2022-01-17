# Auto generate C/H source files from contract ABI JSON files

all:
	make -C ./Solidity all
	make -C ./WASM all

clean:
	-$(BOAT_RM) $(OUTPUT_H_SOURCES)
	-$(BOAT_RM) $(OUTPUT_C_SOURCES)




