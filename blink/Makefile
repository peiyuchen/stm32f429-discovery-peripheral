# The root path of STM32F429 firmware packge
STDP ?= ../../STM32F429I-Discovery_FW_V1.0.1
STD_PERIPH = $(STDP)/Libraries/STM32F4xx_StdPeriph_Driver
CMSIS = $(STDP)/Libraries/CMSIS

# Toolchain configurations
CROSS_COMPILE ?= arm-none-eabi-
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump
SIZE = $(CROSS_COMPILE)size

# Compile flag
## for STM32F429 Discovery
CPU = cortex-m4
CFLAGS = -mcpu=$(CPU) -march=armv7e-m -mtune=$(CPU)
CFLAGS += -mlittle-endian -mthumb
CFLAGS += -DSTM32F429_439xx
## FPU
CFLAGS += -mfloat-abi=hard
## Basic configurations
CFLAGS += -g -std=c99 -Wall
## Optimizations
# CFLAGS += -O3
CFLAGS += -ffunction-sections -fdata-sections -fno-common
CFLAGS += --param max-inline-insns-single=1000
# to run from FLASH
CFLAGS += -DVECT_TAB_FLASH
LDFLAGS = -T stm32f429zi_flash.ld

#####################
# Projet starts here
PROJECT = blink

EXECUTABLE = $(PROJECT).elf
BIN_IMAGE = $(PROJECT).bin
HEX_IMAGE = $(PROJECT).hex

OBJS = \
	   main.o \
	   startup_stm32f429_439xx.o \
	   system_stm32f4xx.o

# Include libraries
CFLAGS += -I.
CFLAGS += -I$(CMSIS)/Device/ST/STM32F4xx/Include
CFLAGS += -I$(CMSIS)/Include
## STM32F4xx_StdPeriph_Driver
CFLAGS += -DUSE_STDPERIPH_DRIVER
CFLAGS += -I$(STD_PERIPH)/inc
CFLAGS += -D"assert_param(expr)=((void)0)"
OBJS += \
		$(STD_PERIPH)/src/stm32f4xx_gpio.o \
		$(STD_PERIPH)/src/stm32f4xx_rcc.o

# Link Library image file
LDFLAGS +=
define get_library_path
    $(shell dirname $(shell $(CC) $(CFLAGS) -print-file-name=$(1)))
endef
LDFLAGS += -L $(call get_library_path,libc.a)
LDFLAGS += -L $(call get_library_path,libgcc.a)

all: $(BIN_IMAGE)

$(BIN_IMAGE): $(EXECUTABLE)
	$(OBJCOPY) -O binary $^ $@
	$(OBJCOPY) -O ihex $^ $(HEX_IMAGE)
	$(OBJDUMP) -h -S -D $(EXECUTABLE) > $(PROJECT).lst
	$(SIZE) $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(LD) -o $@ $(OBJS) \
		$(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(EXECUTABLE)
	rm -rf $(BIN_IMAGE)
	rm -rf $(HEX_IMAGE)
	rm -f $(OBJS)
	rm -f $(PROJECT).lst

flash:
	openocd -f interface/stlink-v2.cfg \
		-f target/stm32f4x_stlink.cfg \
		-c "init" \
		-c "reset init" \
		-c "halt" \
		-c "flash write_image erase $(EXECUTABLE)" \
		-c "verify_image $(EXECUTABLE)" \
		-c "reset run" -c shutdown || \
	st-flash write $(BIN_IMAGE) 0x8000000

.PHONY: clean
