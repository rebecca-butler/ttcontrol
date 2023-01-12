COMPILER=viper
AREA=appl
DEBUGINFO=true
TARGET=TTC94

# directories
SRC_DIR = src
IODRIVER_DIR = ../lib/tttech
IODRIVER_LDIR = $(IODRIVER_DIR)/lib
BSP_DIR = $(IODRIVER_DIR)/bsp

# includes
include $(IODRIVER_DIR)/build/settings.mk
INCDIRS += -I"."
INCDIRS += -I $(SRC_DIR)
INCDIRS += -I"C:\Eigen"

# list of source and object files
C_FILES = $(notdir $(basename $(wildcard $(SRC_DIR)/*.c)))
CPP_FILES = $(notdir $(basename $(wildcard $(SRC_DIR)/*.cpp)))
OBJ_FILES := $(addprefix build/, $(addsuffix .obj, $(notdir $(C_FILES))))
SO_FILES := $(addprefix build/, $(addsuffix .so, $(notdir $(CPP_FILES))))

# list of bsp files
ifeq ($(TARGET), TTC94)
  	BSP_OBJ_FILES = $(BSP_DIR)/bin/bsp_cstart_ttc94.obj
	DOWNLOADER_HW_TYPE = 0x00200801
endif

# modified compiler arguments for cpp (use g++ instead of gcc)
TSK_VIPER_COMP_FLAGS_CPP = -t -Wa-gAHLs --emit-locals=-equ,-symbols -Wa-OgsaJ --section-info=-console -Wa--error-limit=42
TSK_VIPER_COMP_FLAGS_CPP += -Mh --g++,-strings -O3 --mac --tradeoff=0 --source
TSK_VIPER_COMP_FLAGS_CPP += -Wc--no-tasking-sfr
TSK_VIPER_COMP_FLAGS_CPP += -DTASKING_VIPER

# list of lsl files
LSL_FILE=application.lsl

###############################################################################
#
# Rules for building
#
###############################################################################
all : build/main.elf postbuild

# call linker
build/main.elf : $(IODRIVER_LDIR)/$(LIB_NAME) $(BSP_OBJ_FILES) $(SO_FILES) $(OBJ_FILES)
	@echo linking $@
	@"$(TSK_VIPER_LNK)" -o"$@" $(BSP_OBJ_FILES) $(OBJ_FILES) $(SO_FILES) $(IODRIVER_LDIR)/$(LIB_NAME) -Wl-o"build/main.hex":IHEX -Wl-o"build/main.s19":SREC $(TSK_VIPER_LINK_FLAGS) -Wl--map-file="build/main.mapxml":XML $(BSP_DIR)/lsl/$(TARGET)/$(LSL_FILE)
	@echo  -------------
	@echo  done linking.
	@echo  -------------

# build object files
build/%.obj : $(SRC_DIR)/%.c build/%.so
	@echo compiling: $<
	@"$(TSK_VIPER_CC)" -c -o $@ $(TSK_VIPER_COMP_FLAGS) $(INCDIRS) $<

# build shared libraries
build/%.so : $(SRC_DIR)/%.cpp
	@echo compiling: $<
	@"$(TSK_VIPER_CC)" -c -shared -o $@ $(TSK_VIPER_COMP_FLAGS_CPP) $(INCDIRS) $<

clean:
	@echo cleaning up test module files
	-@del /F /Q build\*.*
	@echo done.

postbuild:
	@echo $(BSP_DIR)
	@echo  ---------------------------------
	@echo   Creating APDB...
	@echo  ---------------------------------
	@if exist build\main.hex.addAPDB del build\main.hex.addAPDB
	@if exist build\main.s19.addAPDB del build\main.s19.addAPDB	
	@"$(IODRIVER_DIR)"\\build\\AddAPDB.exe -t $(DOWNLOADER_HW_TYPE) -k "$(IODRIVER_DIR)"\\build\\public.key -v 2.6 -w 32 -f I "$(IODRIVER_DIR)"\\build\\targets.xml build\main.hex build\main.hex.addAPDB
	@"$(IODRIVER_DIR)"\\build\\AddAPDB.exe -t $(DOWNLOADER_HW_TYPE) -k "$(IODRIVER_DIR)"\\build\\public.key -v 2.6 -w 32 -f M "$(IODRIVER_DIR)"\\build\\targets.xml build\main.s19 build\main.s19.addAPDB	
	@copy /Y build\main.hex.addAPDB build\main.hex 
	@copy /Y build\main.s19.addAPDB build\main.s19 	
	@if exist build\main.hex.addAPDB del build\main.hex.addAPDB
	@if exist build\main.s19.addAPDB del build\main.s19.addAPDB

