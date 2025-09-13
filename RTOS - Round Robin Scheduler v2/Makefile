#CMD Commands
CC=arm-none-eabi-gcc
OBJ_DUMP=arm-none-eabi-objdump.exe
OBJ_COPY=arm-none-eabi-objcopy
NM=arm-none-eabi-nm.exe
GDB=arm-none-eabi-gdb.exe
FLASH=st-flash --reset write
PROBE=st-info --probe

#Toolchain Paths
OPENOCD_DIR=D:\Software\xpack-openocd-0.12.0-3\openocd

#Openocd
#Configuration File
#Interface
OPENOCD_INTF_CFG_FILE=$(OPENOCD_DIR)\scripts\interface\stlink-v2.cfg
#target
OPENOCD_TGT_CFG_FILE=$(OPENOCD_DIR)\scripts\target\stm32f1x.cfg

#MCU Params
MACH=cortex-m3
FLASH_ADDR=0x08000000

#Directories
WORK_DIR=D:/Workspaces/Custom RTOS Workspace
OBJ_DIR=Objects
INC_DIR=../Drivers/Inc
SRC_DIR=../Drivers/Src

#Files
MAIN=main
ELF=final.elf
STARTUP=bp_startup
SYSCALLS=syscalls
LINK=bp_linker.ld
RTOS=rtos_kernel.o rtos_task.o rtos_info.o
DRIVERS=stm32f103xx_gpio.o stm32f103xx_afio.o stm32f103xx_exti.o stm32f103xx_nvic.o \
        stm32f103xx_usart.o stm32f103xx_rcc.o stm32f103xx_systick.o stm32f103xx_timer.o \
		stm32f103xx_faults.o
OTHERS=stm32f103xx_init.o stm32f103xx_serial.o stm32f103xx_utilities.o
#MIDDLEWARE=

#Flags
CFLAGS= -c -mcpu=$(MACH) -mthumb -mfloat-abi=soft -std=gnu11 -Wall -Wno-unused -O0 -I$(INC_DIR)
DEBUG_FLAG= -g3
LDFLAGS= --specs=nano.specs -mcpu=$(MACH) -mthumb -T $(LINK) -Wl,-Map=logs/final.map



all: compile load
compile: clean final.elf clean-obj
final: final.elf
rtos_kernel.o:rtos_kernel.c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
rtos_task.o:rtos_task.c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
rtos_info.o:rtos_info.c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
stm32f103xx_gpio.o:$(SRC_DIR)/stm32f103xx_gpio.c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
stm32f103xx_afio.o:$(SRC_DIR)/stm32f103xx_afio.c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
stm32f103xx_exti.o:$(SRC_DIR)/stm32f103xx_exti.c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
stm32f103xx_nvic.o:$(SRC_DIR)/stm32f103xx_nvic.c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
stm32f103xx_usart.o:$(SRC_DIR)/stm32f103xx_usart.c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
stm32f103xx_rcc.o:$(SRC_DIR)/stm32f103xx_rcc.c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
stm32f103xx_timer.o:$(SRC_DIR)/stm32f103xx_timer.c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
stm32f103xx_faults.o:$(SRC_DIR)/stm32f103xx_faults.c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
stm32f103xx_init.o:$(SRC_DIR)/stm32f103xx_init.c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
stm32f103xx_serial.o:$(SRC_DIR)/stm32f103xx_serial.c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
stm32f103xx_systick.o:$(SRC_DIR)/stm32f103xx_systick.c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
stm32f103xx_utilities.o:$(SRC_DIR)/stm32f103xx_utilities.c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
$(MAIN).o:$(MAIN).c 
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
$(STARTUP).o:$(STARTUP).c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
$(SYSCALLS).o:$(SYSCALLS).c
	$(CC) $(CFLAGS) $(DEBUG_FLAG) $^ -o $@
final.elf: $(MAIN).o $(RTOS) $(DRIVERS) $(OTHERS) $(STARTUP).o $(SYSCALLS).o
	$(CC) $(LDFLAGS) $^ -o $@

final.bin:final.elf
	arm-none-eabi-objcopy -O binary $^ $@
load:final.bin
	$(FLASH) $^ $(FLASH_ADDR)

examine-sections:
	$(OBJ_DUMP) -h final.elf
examine-symbols:
	$(NM) final.elf
dump:final.elf
	$(OBJ_DUMP) -d $^ > logs/assembly_dump.log
	$(OBJ_DUMP) -h $^ > logs/section_contents.log
	$(OBJ_DUMP) -s $^ > logs/section_contents_detailed.log
	$(NM) final.elf > logs/symbols.log
probe:
	$(PROBE)

debug: openocd gdb
openocd:
	openocd -f $(OPENOCD_INTF_CFG_FILE) -f $(OPENOCD_TGT_CFG_FILE)
gdb:
	$(GDB) final.elf
gdb_tui:
	$(GDB) -tui final.elf

.PHONY: clean clean-obj
clean:
	del /s *.o *.elf *.map *.log *.bin *.exe
clean-obj:
	del /s *.o