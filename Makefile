COMPILER=viper
AREA=appl
DEBUGINFO=true
TARGET=TTC94

# directories
SRC_DIR = src
IODRIVER_DIR = lib/tttech
IODRIVER_LDIR = $(IODRIVER_DIR)/lib
BSP_DIR = $(IODRIVER_DIR)/bsp

# includes
include $(IODRIVER_DIR)/build/settings.mk
INCDIRS += -I"."
INCDIRS += -I $(SRC_DIR)
INCDIRS += -I"C:\Eigen"

# list of source and object files
FILES = $(notdir $(basename $(wildcard $(SRC_DIR)/*.c)))
OBJ_FILES := $(addprefix build/, $(addsuffix .obj, $(notdir $(FILES))))

# list of bsp files
ifeq ($(TARGET), TTC94)
  	BSP_OBJ_FILES = $(BSP_DIR)/bin/bsp_cstart_ttc94.obj
	DOWNLOADER_HW_TYPE = 0x00200801
endif

# list of lsl files
LSL_FILE=application.lsl

###############################################################################
#
# Rules for building
#
###############################################################################
all : build/main.elf postbuild

# call linker
build/main.elf : $(IODRIVER_LDIR)/$(LIB_NAME) $(BSP_OBJ_FILES) $(OBJ_FILES)
	@echo linking $@
	@"$(TSK_VIPER_LNK)" -o"$@" $(BSP_OBJ_FILES) $(OBJ_FILES) $(IODRIVER_LDIR)/$(LIB_NAME) -Wl-o"build/main.hex":IHEX -Wl-o"build/main.s19":SREC $(TSK_VIPER_LINK_FLAGS) -Wl--map-file="build/main.mapxml":XML $(BSP_DIR)/lsl/$(TARGET)/$(LSL_FILE)
	@echo  -------------
	@echo  done linking.
	@echo  -------------

# build object files
build/%.obj : $(SRC_DIR)/%.c
	@echo compiling: $<
	@"$(TSK_VIPER_CC)" -c -o $@ $(TSK_VIPER_COMP_FLAGS) $(INCDIRS) $<

# build shared libraries
# build/%.so : $(SRC_DIR)/%.cpp
# 	@echo compiling: $<
# 	@"$(TSK_VIPER_++)" -c -o $@ $(TSK_VIPER_COMP_FLAGS) $(INCDIRS) $<

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

