.SUFFIXES:

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
	-ffunction-sections -fdata-sections \
	$(ARCH)

CFLAGS += $(INCLUDE)

ASFLAGS := -g $(ARCH)

LDFLAGS = -g $(ARCH) -Wl,-Map,$(notdir $*.map)

LIBS := -lgba
LIBDIRS := $(LIBGBA)

ifneq ($(BUILD),$(notdir $(CURDIR)))

export OUTPUT := $(CURDIR)/$(TARGET)

export VPATH := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir))

export DEPSDIR := $(CURDIR)/$(BUILD)

export PATH := $(DEVKITARM)/bin:$(PATH)

CFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))
SFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.s)))

export OFILES := $(CFILES:.c=.o) $(SFILES:.s=.o)

export INCLUDE := $(foreach dir,$(INCLUDES),-I$(CURDIR)/$(dir)) \
	$(foreach dir,$(LIBDIRS),-I$(dir)/include) \
	-I$(CURDIR)/$(BUILD)

export LIBPATHS := $(foreach dir,$(LIBDIRS),-L$(dir)/lib)

# IMPORTANT: use the GCC driver for linking, not raw ld.
export LD := $(CC)

.PHONY: $(BUILD) clean

$(BUILD):
	@[ -d $@ ] || mkdir -p $@
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

clean:
	@echo clean ...
	@rm -fr $(BUILD) $(TARGET).elf $(TARGET).gba

else

DEPENDS := $(OFILES:.o=.d)

$(OUTPUT).gba: $(OUTPUT).elf

$(OUTPUT).elf: $(OFILES) $(LIBGBA)/lib/libgba.a

-include $(DEPENDS)

endif
