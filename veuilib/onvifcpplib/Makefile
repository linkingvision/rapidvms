-include ../../common.mk

DEBUG_BUILD=no
CPROG=libonvifcpplib

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

BUILD_DIR     = ./lib/linux/
CFLAGS	=  -DLINUX -fPIC 
CFLAGS	+= -DWITH_OPENSSL -DWITH_DOM -DWITH_PURE_VIRTUAL -DWITH_NONAMESPACES 
LIBS	= $(BUILD_DIR)/$(CPROG).a

# build tools
MKDIR = mkdir -p
RMF = rm -f
RMRF = rm -rf

HDRS   = $(wildcard ./include/*.hpp)
HDRS   += $(wildcard ./onvifgen/*.h)

LIB_SOURCES=\
               ./onvifgen/soapC.o\
               ./onvifgen/soapDeviceBindingProxy.o\
               ./onvifgen/soapDeviceBindingService.o\
               ./onvifgen/soapDeviceIOBindingProxy.o\
               ./onvifgen/soapDeviceIOBindingService.o\
               ./onvifgen/soapDisplayBindingProxy.o\
               ./onvifgen/soapDisplayBindingService.o\
               ./onvifgen/soapImagingBindingProxy.o\
               ./onvifgen/soapImagingBindingService.o\
               ./onvifgen/soapMediaBindingProxy.o\
               ./onvifgen/soapMediaBindingService.o\
               ./onvifgen/soapPTZBindingProxy.o\
               ./onvifgen/soapPTZBindingService.o\
               ./onvifgen/soapPullPointSubscriptionBindingProxy.o\
               ./onvifgen/soapPullPointSubscriptionBindingService.o\
               ./onvifgen/soapReceiverBindingProxy.o\
               ./onvifgen/soapReceiverBindingService.o\
               ./onvifgen/soapRecordingBindingProxy.o\
               ./onvifgen/soapRecordingBindingService.o\
               ./onvifgen/soapRemoteDiscoveryBindingProxy.o\
               ./onvifgen/soapRemoteDiscoveryBindingService.o\
               ./onvifgen/soapReplayBindingProxy.o\
               ./onvifgen/soapReplayBindingService.o\
               ./onvifgen/soapSearchBindingProxy.o\
               ./onvifgen/soapSearchBindingService.o\
               ./onvifgen/soapPullPointBindingService.o\
               ./onvifgen/soapSubscriptionManagerBindingProxy.o\
               ./onvifgen/soapNotificationProducerBindingProxy.o\
				./gsoap/gsoap/runtime/dom.o\
				./gsoap/gsoap/runtime/duration.o\
				./gsoap/gsoap/runtime/smdevp.o\
				./gsoap/gsoap/runtime/stdsoap2.o\
				./gsoap/gsoap/runtime/threads.o\
				./gsoap/gsoap/runtime/wsaapi.o\
				./gsoap/gsoap/runtime/wsseapi.o\
			   
			   

ifeq ($(DEBUG_BUILD), yes)
DEBUG	= -g -Wall
else
DEBUG   = -g -Wall
endif

IFLAGS 	= -I. -I./include/ -I./onvifgen/ 
IFLAGS += -I../../3rdparty/openssl/include
IFLAGS += -I./gsoap/gsoap/runtime


LDFLAGS +=  -g -L../../3rdparty/openssl-linux/  -lssl -lcrypto -lpthread -ldl -lstdc++ -lm -lcrypt 


all:  $(LIBS) example_

$(LIB_SOURCES): $(HDRS)

$(LIBS): $(LIB_SOURCES)
	$(MKDIR) $(BUILD_DIR) 
	$(AR) $(ARFLAGS) $(LIBS) $?

example_:
	#make -C example/server/onvifserverlinux
	#make -C example/client/onvifclientlinux		

clean:
	rm -f $(LIBS)  $(LIB_SOURCES)
	#make -C example/server/onvifserverlinux clean
	#make -C example/client/onvifclientlinux clean

distclean: clean



install:


.cpp.o:
	$(CPP) -c -o $@ $(DEBUG) $(CFLAGS) $(IFLAGS) $<
.c.o:
	$(CPP) -c -o $@ $(DEBUG) $(CFLAGS) $(IFLAGS) $<
