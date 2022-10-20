# Auto generate C/H source files from contract ABI JSON files

# Specify generator script
# Specify generator script
PY             = python
AUTOGEN_SCRIPT = $(BOAT_BASE_DIR)/tools/fiscobcos2c.py

# Search all files with .json extension
ABIJSONS = $(wildcard *.json)

# Output directory for generated C/H source files
OUTPUT_SOURCES_DIR = $(BOAT_BUILD_DIR)/demo/demo_fiscobcos/demo_contract

OUTPUT_H_SOURCES = $(patsubst %.json, $(OUTPUT_SOURCES_DIR)/%.h, $(ABIJSONS))
OUTPUT_C_SOURCES = $(patsubst %.json, $(OUTPUT_SOURCES_DIR)/%.c, $(ABIJSONS))

.PHONY: all

all: $(OUTPUT_C_SOURCES) $(OUTPUT_H_SOURCES)


$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)

$(OUTPUT_H_SOURCES): $(OUTPUT_SOURCES_DIR)/%.h : %.json
	$(PY) $(AUTOGEN_SCRIPT) $? $(OUTPUT_SOURCES_DIR)


$(OUTPUT_C_SOURCES): $(OUTPUT_SOURCES_DIR)/%.c : %.json
	$(PY) $(AUTOGEN_SCRIPT) $? $(OUTPUT_SOURCES_DIR)

	


clean:
	-$(BOAT_RM) $(OUTPUT_H_SOURCES)
	-$(BOAT_RM) $(OUTPUT_C_SOURCES)




