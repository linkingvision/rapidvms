-include ../common.mk
AS              = $(PRJ_CROSS)as
LD              = $(PRJ_CROSS)ld
CC              = $(PRJ_CROSS)gcc
CPP             = $(PRJ_CROSS)g++
AR              = $(PRJ_CROSS)ar
NM              = $(PRJ_CROSS)nm
STRIP           = $(PRJ_CROSS)strip
OBJCOPY         = $(PRJ_CROSS)objcopy
OBJDUMP         = $(PRJ_CROSS)objdump
RANLIB          = $(PRJ_CROSS)ranlib

ifeq ($(TARGET_OS),)
  ifeq ($(OS),Windows_NT)
    TARGET_OS = WIN32
  else
    UNAME_S := $(shell uname -s)
    ifeq ($(UNAME_S),Linux)
        TARGET_OS = LINUX
    else
        ifeq ($(UNAME_S),Darwin)
            TARGET_OS = OSX
        else
            TARGET_OS = BSD
        endif
    endif
  endif
endif

all:veuilib_build 


clean:veuilib_clean 

LIBS=libveuilib

veuilib_build:
	$(MAKE) -C ./onvifcpplib;
ifneq ($(strip $(VE_OS)), macos)#macos do not build the server
ifeq ($(strip $(PRJ_CROSS)), )
	$(MAKE) -C ./rtspserver;
endif
endif
ifeq ($(strip $(PRJ_CROSS)), )
	cd ./prj_linux/ ; qmake .;
else
	cd ./prj_linux/ ; qmake . CONFIG+=ARM_LINUX;
endif
	$(MAKE) -C ./prj_linux;
	rm -rf ../linux/lib/libveuilib*
	cp -f ./prj_linux/libveuilib* ./lib/linux/
ifneq ($(strip $(VE_OS)), macos)
	cd ./lib/linux && \
	$(OBJCOPY) --only-keep-debug $(LIBS).so $(LIBS).so.dbg && $(STRIP) -g $(LIBS).so && $(OBJCOPY) --add-gnu-debuglink=$(LIBS).so.dbg $(LIBS).so
endif
	cp ./lib/linux/libveuilib* ../output/$(VE_INSTALL_DIR)/lib/

veuilib_clean:
	$(MAKE) -C ./onvifcpplib clean;
	cd ./prj_linux/ ; qmake .;
	$(MAKE) -C ./prj_linux/ clean;
	rm -rf ./prj_linux/libveuilib.*
	
	
