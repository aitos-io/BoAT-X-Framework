-include $(CFG)/config.mk

LDFLAGS += -lumdp -lumdpcommon
LDFLAGS += -L../BoAT-X-Framework/lib -lboatwallet
LDFLAGS += -L../BoAT-X-Framework/lib -lboatvendor
LDFLAGS += -lpthread  -lrt 
LDFLAGS += ./libcurl.so.4.5.0
   
OBJDIR = Object
SRCS_DIR := $(wildcard ./*.c)

OBJS := $(addsuffix .o,$(addprefix $(OBJDIR)/,$(basename $(notdir $(SRCS_DIR)))))
DEPENDS := $(addsuffix .d,$(OBJS))
  
TARGET = call_boat_test
TARGET_EXE = $(OBJDIR)/$(TARGET)
all: distclean $(TARGET_EXE)
 
$(TARGET_EXE) : $(OBJS)
	@echo "linking $@"
	$(CC) -o $@ $(OBJS) -Wl,-start-group $(LDFLAGS) -Wl,-end-group $(CFLAGS)
	$(STRIP) -g $(TARGET_EXE)
	$(INSTALL) -c -m 777 $(TARGET_EXE) $(APP_BIN)
define make-cmd-cc
$2 : $1
	@mkdir -p $(OBJDIR)
	$$(CC) $$(CFLAGS) -MMD -MT $$@ -MF $$@.d -c -o $$@ $$<   
endef
 
$(foreach afile,$(SRCS_DIR),\
	$(eval $(call make-cmd-cc,$(afile),\
        $(addsuffix .o,$(addprefix $(OBJDIR)/,$(basename $(notdir $(afile))))))))

clean:
	$(RM) -rf $(OBJS) $(TARGET_EXE) $(OBJDIR)

distclean: clean
	$(RM) $(APP_BIN)/$(TARGET) -f
 
-include $(DEPENDS)
