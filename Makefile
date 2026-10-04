# NECROROCK - Game Boy Advance
# Proper devkitARM GBA build

ifeq ($(strip $(DEVKITARM)),)
$(error "Please set DEVKITARM in your environment.")
endif

include $(DEVKITARM)/gba_rules

TARGET := NECROROCK

BUILD := build
SOURCES := src
INCLUDES := include

ARCH := -mthumb -mthumb-interwork

CFLAGS := -g -Wall -O2 \
          -mcpu=arm7tdmi -mtune=arm7tdmi \
          -ffast-math \
          $(ARCH)

CFLAGS += $(INCLUDE)

ASFLAGS := -g $(ARCH)

LDFLAGS := -g $(ARCH) -Wl,-Map,$(TARGET).map

export DEPSDIR := $(CURDIR)/$(BUILD)

CFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
SFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))

export OFILES := $(CFILES:.c=.o) $(SFILES:.s=.o)

export INCLUDE := \
    $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
    -I$(CURDIR)/$(BUILD)

.PHONY: all clean

all: $(TARGET).gba

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

clean:
	@echo "Cleaning..."
	@rm -rf $(BUILD) $(TARGET).elf $(TARGET).gba $(TARGET).map

else

DEPENDS := $(OFILES:.o=.d)

$(TARGET).gba: $(TARGET).elf

$(TARGET).elf: $(OFILES)

-include $(DEPENDS)

endif
