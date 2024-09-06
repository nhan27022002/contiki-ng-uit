CONTIKI_PROJECT = hello-world
all: $(CONTIKI_PROJECT)

CONTIKI = ../..
MODULES += os/services/shell
include $(CONTIKI)/Makefile.include
  MAKE_MAC = MAKE_MAC_TSCH
  MODULES += $(CONTIKI_NG_MAC_DIR)/tsch
  CFLAGS += -DMAC_CONF_WITH_TSCH=1