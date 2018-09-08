#Makefile from TI, modified by Derek Molloy
#PRU_CGT environment variable must point to the TI PRU compiler directory. E.g.:
#(Linux) export PRU_CGT=/usr/lib/ti/pru-software-support-package

PRU_CGT     :=/usr/lib/ti/pru-software-support-package
MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST)))
CURRENT_DIR := $(notdir $(patsubst %/,%,$(dir $(MKFILE_PATH))))
PROJ_NAME   =$(CURRENT_DIR)
LINKER_COMMAND_FILE=./AM335x_PRU.cmd
LIBS        =--library=$(PRU_CGT)/lib/rpmsg_lib.lib
INCLUDE     =--include_path=$(PRU_CGT)/include --include_path=$(PRU_CGT)/include/am335x
STACK_SIZE  =0x100
HEAP_SIZE   =0x100
GEN_DIR     =gen
PRU0        :=/sys/class/remoteproc/remoteproc1
PRU1        :=/sys/class/remoteproc/remoteproc2

#Common compiler and linker flags (Defined in 'PRU Optimizing C/C++ Compiler User's Guide)
CFLAGS=-v3 -O2 --display_error_number --endian=little --hardware_mac=on --obj_directory=$(GEN_DIR) --pp_directory=$(GEN_DIR) -ppd -ppa
#Linker flags (Defined in 'PRU Optimizing C/C++ Compiler User's Guide)
LFLAGS=--reread_libs --warn_sections --stack_size=$(STACK_SIZE) --heap_size=$(HEAP_SIZE)

TARGET      =$(GEN_DIR)/$(PROJ_NAME).out
MAP         =$(GEN_DIR)/$(PROJ_NAME).map
SOURCES     =$(wildcard *.c)
#Using .object instead of .obj in order to not conflict with the CCS build process
OBJECTS     =$(patsubst %,$(GEN_DIR)/%,$(SOURCES:.c=.object))

all: printStart $(TARGET) printEnd

printStart:
	@echo ''
	@echo '************************************************************'
	@echo 'Building project: $(PROJ_NAME)'

printEnd:
	@echo ''
	@echo 'Finished building project: $(PROJ_NAME)'
	@echo '************************************************************'
	@echo ''

install_PRU0:
	@echo ''
	@echo 'Stopping current PRU0 application ($(PRU0))'
	@echo stop | sudo tee $(PRU0)/state
	@echo 'Installing firmware'
	@sudo cp $(TARGET) /lib/firmware/am335x-pru0-fw
	@echo 'Deploying firmware'
	@echo am335x-pru0-fw | sudo tee $(PRU0)/firmware
	@echo 'Starting new PRU0 application'
	@echo start | sudo tee $(PRU0)/state

install_PRU1:
	@echo ''
	@echo 'Stopping current PRU1 application ($(PRU1))'
	@echo stop | sudo tee $(PRU1)/state
	@echo 'Installing firmware'
	@sudo cp $(TARGET) /lib/firmware/am335x-pru1-fw
	@echo 'Deploying firmware'
	@echo am335x-pru1-fw | sudo tee $(PRU1)/firmware
	@echo 'Starting new PRU1 application'
	@echo start | sudo tee $(PRU1)/state

$(GEN_DIR)/perfTestASM.out: $(GEN_DIR)/perfTestContainer.object $(GEN_DIR)/perfTestASM.object
	@echo 'LD	$^'
	@/usr/bin/lnkpru -i$(PRU_CGT)/lib -i$(PRU_CGT)/include $(LFLAGS) -o $@ $^  $(LINKER_COMMAND_FILE) --library=libc.a $(LIBS) $^

# Invokes the linker (-z flag) to make the .out file
#$(TARGET): $(OBJECTS) $(LINKER_COMMAND_FILE)
#	@echo ''
#	@echo 'Building target: $@'
#	@echo 'Invoking: PRU Linker'
#	/usr/bin/clpru $(CFLAGS) -z -i$(PRU_CGT)/lib -i$(PRU_CGT)/include $(LFLAGS) -o $(TARGET) $(OBJECTS) -m$(MAP) $(LINKER_COMMAND_FILE) --library=libc.a $(LIBS)
#	@echo 'Finished building target: $@'
#	@echo ''
#	@echo 'Output files can be found in the "$(GEN_DIR)" directory'

# Invokes the compiler on all c files in the directory to create the object files
$(GEN_DIR)/perfTestContainer.object: perfTestContainer.c
	@mkdir -p $(GEN_DIR)
	@echo ''
	@echo 'Building file: $<'
	@echo 'Invoking: PRU Compiler'
	/usr/bin/clpru --include_path=$(PRU_CGT)/include $(INCLUDE) $(CFLAGS) -fe $@ $<

# Invokes the compiler on all c files in the directory to create the object files
$(GEN_DIR)/perfTestASM.object: perfTestASM.asm
	@mkdir -p $(GEN_DIR)
	@echo ''
	@echo 'Building file: $<'
	@echo 'Invoking: PRU Compiler'
	/usr/bin/clpru --include_path=$(PRU_CGT)/include $(INCLUDE) $(CFLAGS) -fe $@ $<

.PHONY: all clean

# Remove the $(GEN_DIR) directory
clean:
	@echo ''
	@echo '************************************************************'
	@echo 'Cleaning project: $(PROJ_NAME)'
	@echo ''
	@echo 'Removing files in the "$(GEN_DIR)" directory'
	@rm   -rf $(GEN_DIR)
	@echo ''
	@echo 'Finished cleaning project: $(PROJ_NAME)'
	@echo '************************************************************'
	@echo ''

# Includes the dependencies that the compiler creates (-ppd and -ppa flags)
-include $(OBJECTS:%.object=%.pp)
