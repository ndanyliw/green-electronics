CC=arm-none-eabi-gcc
AR=arm-none-eabi-ar

# Location of the Libraries folder from the STM32F0xx Standard Peripheral Library
STD_PERIPH_LIB=../project-template/Libraries

###########################################

vpath %.c src $(STD_PERIPH_LIB)/STM32F30x_StdPeriph_Driver/src $(STD_PERIPH_LIB)/STM32_USB-FS-Device_Driver/src

CFLAGS  = -g -O2 -Wall -std=c99 -DGE_LIB -L$(STD_PERIPH_LIB) -lstm32f3
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -march=armv7e-m
CFLAGS += -mfpu=fpv4-sp-d16 -mfloat-abi=hard
CFLAGS += -ffreestanding -nostdlib
CFLAGS += -includestm32f30x_conf.h 
CFLAGS += -I$(STD_PERIPH_LIB)/CMSIS/Include 
CFLAGS += -I$(STD_PERIPH_LIB)/CMSIS/Device/ST/STM32F30x/Include 
CFLAGS += -I$(STD_PERIPH_LIB)/STM32F30x_StdPeriph_Driver/inc
CFLAGS += -I$(STD_PERIPH_LIB)/STM32_USB-FS-Device_Driver/inc
CFLAGS += -I$(STD_PERIPH_LIB)
CFLAGS += -I$(STD_PERIPH_LIB)/../inc
CFLAGS += -Iinc

SRCS = $(wildcard src/*.c)

#include USB files
SRCS += $(wildcard $(STD_PERIPH_LIB)/STM32_USB-FS-Device_Driver/src/*.c)

OBJS = $(addprefix objs/,$(notdir $(SRCS:.c=.o)))
DEPS = $(addprefix deps/,$(notdir $(SRCS:.c=.d)))

.PHONY: all clean

all: lib libge.a

-include $(DEPS)

lib:
	$(MAKE) -C $(STD_PERIPH_LIB)

dirs:
	mkdir -p deps objs
	touch dirs

objs/%.o : %.c dirs
	$(CC) $(CFLAGS) -c -o $@ $< -MMD -MF deps/$(*F).d

libge.a: $(OBJS)
	$(AR) -r $@ $(OBJS)

clean:
	rm -f dirs $(OBJS) $(DEPS) libge.a

