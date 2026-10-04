TARGET := NECROROCK

CC := arm-none-eabi-gcc
OBJCOPY := arm-none-eabi-objcopy

CFLAGS := -mthumb -mthumb-interwork -mcpu=arm7tdmi \
          -O2 -ffreestanding -fno-common \
          -Wall -Wextra

LDFLAGS := -mthumb -mthumb-interwork -mcpu=arm7tdmi \
           -specs=nosys.specs \
           -nostartfiles

SRC := src/main.c

all: $(TARGET).gba

$(TARGET).elf: $(SRC)
	$(CC) $(CFLAGS) $(LDFLAGS) \
		-Ttext=0x08000000 \
		-o $@ $(SRC)

$(TARGET).gba: $(TARGET).elf
	$(OBJCOPY) -O binary $< $@

clean:
	rm -f $(TARGET).elf $(TARGET).gba
