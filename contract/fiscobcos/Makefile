# Auto generate C/H source files from contract ABI JSON files

# Specify generator script
AUTOGEN_SCRIPT = $(BOAT_BASE_DIR)/tools/fiscobcos2c.py

# Search all files with .json extension
ABIJSONS = $(wildcard *.json)

# Output directory for generated C/H source files
OUTPUT_SOURCES_DIR = $(BOAT_BASE_DIR)/contract/generated

OUTPUT_H_SOURCES = $(patsubst %.json, $(OUTPUT_SOURCES_DIR)/%.h, $(ABIJSONS))
OUTPUT_C_SOURCES = $(patsubst %.json, $(OUTPUT_SOURCES_DIR)/%.c, $(ABIJSONS))

.PHONY: all

all: $(OUTPUT_C_SOURCES) $(OUTPUT_H_SOURCES)


$(OBJECTS_DIR):
	$(BOAT_MKDIR) -p $(OBJECTS_DIR)

$(OUTPUT_H_SOURCES): $(OUTPUT_SOURCES_DIR)/%.h : %.json
ifeq ($(BOAT_PROTOCOL_USE_FISCOBCOS), 1)
	$(PY) $(AUTOGEN_SCRIPT) $< $(OUTPUT_SOURCES_DIR)
endif

$(OUTPUT_C_SOURCES): $(OUTPUT_SOURCES_DIR)/%.c : %.json
ifeq ($(BOAT_PROTOCOL_USE_FISCOBCOS), 1)
	$(PY) $(AUTOGEN_SCRIPT) $< $(OUTPUT_SOURCES_DIR)
endif
	


clean:
	-$(BOAT_RM) $(OUTPUT_H_SOURCES)
	-$(BOAT_RM) $(OUTPUT_C_SOURCES)




