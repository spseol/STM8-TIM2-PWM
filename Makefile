#######
# makefile for STM8S_StdPeriph_Lib and SDCC compiler
#
# Customized by MightyPork 1/2017
#
# usage:
#   1. if SDCC not in PATH set path -> CC_ROOT
#   2. set correct STM8 device -> DEVICE
#   3. set project paths -> PRJ_ROOT, PRJ_SRC_DIR, PRJ_INC_DIR
#   4. set SPL root path -> SPL_ROOT
#   5. include required SPL modules -> SPL_SOURCE
#
#######

# STM8 device (default is STM8 discovery board)
DEVICE=STM8S208
DEVICE_FLASH=stm8s208rb
F_CPU=16000000

# trap handling requires SDCC >=v3.4.3
SKIP_TRAPS = 0

MKDIR = mkdir
CP = cp

# set compiler path & parameters 
CC_ROOT =
CC      = sdcc
CFLAGS  = -mstm8 --out-fmt-elf --opt-code-size
CFLAGS += --std-sdcc99
CFLAGS += --asm=gas --function-sections --data-sections
CFLAGS += --nogcse --all-callee-saves --stack-auto --fverbose-asm --float-reent --no-peep

CFLAGS += -D F_CPU=$(F_CPU)
CFLAGS += --debug

LD = stm8-ld
LD_FLAGS = -T./elf32stm8s003f3.x --print-memory-usage --gc-sections -Map $(OUTPUT_DIR)/map_$(DEVICE).map
LIB_DIRS = $(addprefix -L, /usr/local/share/sdcc/lib/stm8)

AS = stm8-as
AS_FLAGS =

# set output folder and target name
OUTPUT_DIR = ./build-$(DEVICE)
TARGET     = $(OUTPUT_DIR)/out-$(DEVICE)

# set project folder and files (all *.c)
PRJ_ROOT    = .
PRJ_SRC_DIR = $(PRJ_ROOT)/src
PRJ_INC_DIR = $(PRJ_ROOT)/inc
# all project sources included by default
PRJ_SOURCE  = $(notdir $(wildcard $(PRJ_SRC_DIR)/*.c))
PRJ_OBJECTS := $(addprefix $(OUTPUT_DIR)/, $(PRJ_SOURCE:.c=.o))

# set SPL paths
#SPL_SRC_DIR = /usr/local/lib/stm8/src/
#SPL_INC_DIR = /usr/local/lib/stm8/inc/
#SPL_SRC_DIR = ../SPL/src/
#SPL_INC_DIR = ../SPL/inc/
SPL_SRC_DIR = ../SPL-$(DEVICE)/src/
SPL_INC_DIR = ../SPL-$(DEVICE)/inc/
# add all library sources used here singly ... or all .c files in SPL src dir
#SPL_SOURCE  = stm8s_gpio.c stm8s_clk.c stm8s_tim4.c stm8s_itc.c 
#SPL_SOURCE += stm8s_uart1.c
SPL_SOURCE  = $(notdir $(wildcard $(SPL_SRC_DIR)/*.c))
#SPL_SOURCE  += $(notdir $(wildcard /usr/local/stow/sdcc-gas/share/sdcc/lib/src/_*.c))
SPL_OBJECTS := $(addprefix $(OUTPUT_DIR)/, $(SPL_SOURCE:.c=.o))

SDCC_LIB_SRC_DIR = /usr/local/stow/sdcc-gas/share/sdcc/lib/src/
SDCC_LIB_SOURCE = _mullong.c _divulong.c _mulint.c _modsint.c 
SDCC_LIB_SOURCE += printf_large.c _strlen.c sprintf.c vprintf.c

SDCC_LIB_OBJECTS := $(addprefix $(OUTPUT_DIR)/, $(SDCC_LIB_SOURCE:.c=.o))

# collect all include folders
INCLUDE = -I$(PRJ_INC_DIR) -I$(SPL_INC_DIR) 

# collect all source directories
VPATH=$(PRJ_SRC_DIR):$(SPL_SRC_DIR):$(SDCC_LIB_SRC_DIR)

SDCC_SRC_DIR = /usr/local/stow/sdcc-gas/share/sdcc/lib/src

# Source dependencies:
#DEPS = $(OBJECTS:.o=.d)
#ASM_DEPS = $(OBJECTS:.o=.asm)

#-include $(DEPS)


$(TARGET).elf: $(PRJ_OBJECTS) $(SPL_OBJECTS) $(SDCC_LIB_OBJECTS)
	$(LD) $^ -o $@ $(LD_FLAGS) $(LIBS)


clean:
	@echo $(SPL_OBJECTS)
	rm -rf $(OUTPUT_DIR)

$(OUTPUT_DIR)/%.d: %.c
	@$(MKDIR) -p $(OUTPUT_DIR)
	$(CC) -c $< $(CFLAGS) -D$(DEVICE) $(INCLUDE) -MM > $@

$(OUTPUT_DIR)/%.o: %.c $(OUTPUT_DIR)/%.d
	@$(MKDIR) -p $(OUTPUT_DIR)
	$(CC) -c $< $(CFLAGS) -D$(DEVICE) $(INCLUDE)  -o $@

#$(OUTPUT_DIR)/%.o: %.asm
#    @$(MKDIR) -p $(OUTPUT_DIR)
#    $(AS) $< $(AS_FLAGS) -o $@


flash: $(TARGET).elf
	stm8-objcopy -O ihex $(TARGET).elf $(TARGET).hex
	stm8flash -c stlinkv21 -p $(DEVICE_FLASH) -s flash -w $(TARGET).hex

# ----------------------------------------
# Phony targets
# ----------------------------------------
.PHONY: clean debug

# vim:ft=make
