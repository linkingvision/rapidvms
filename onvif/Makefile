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

CPROG = onvifagentcli
#CXXPROG = h5s

# build tools
MKDIR = mkdir -p
RMF = rm -f
RMRF = rm -rf

BUILD_DIR = out.linux/build/


BUILD_DIRS = $(BUILD_DIR) $(BUILD_DIR)/src $(BUILD_DIR)/src/tinyprocess $(BUILD_DIR)/include\
			$(BUILD_DIR)/include/video $(BUILD_DIR)/example $(BUILD_DIR)/../3rdparty/civetweb/src/
LIB_SOURCES = ../3rdparty/civetweb/src/CivetServer.cpp\
		src/onvifagentlib.cpp\
		src/tinyprocess/process_unix.cpp\
		src/tinyprocess/process.cpp\
		include/video/h264_bit_reader.cpp\
		include/video/h264_parser.cpp\
		include/video/h5s_h264_sink.cpp\
		include/video/h5slib.cpp
		
LIB_SOURCES_CC = \
		include/onvifclidevice.pb.cc\
		include/onvifclimedia.pb.cc\
		include/onvifcli.pb.cc\
		include/onvifcliptz.pb.cc

		
		
LIB_SOURCES_C = ../3rdparty/civetweb/src/civetweb.c
	
LIB_INLINE  = 
APP_SOURCES = example/example.cpp

SOURCE_DIRS =

#OBJECTS = $(LIB_SOURCES:.c=.o) $(APP_SOURCES:.c=.o)
OBJECTS =  $(LIB_SOURCES:.cpp=.o) $(LIB_SOURCES_CC:.cc=.o) $(LIB_SOURCES_C:.c=.o) $(APP_SOURCES:.cpp=.o)

BUILD_RESOURCES =

LIVE555=3rdparty/live555/

CFLAGS +=  -Wall -Wextra -Wshadow -Wformat-security -Winit-self -Wmissing-prototypes -O2 -D$(TARGET_OS) -Iinclude 
CFLAGS += -DUSE_WEBSOCKET
CFLAGS+= -I$(VE_PATH)/output/$(VE_INSTALL_DIR)/include\
			-I./include\
			-I./include/video\
			-I./src/\
			-I../$(LIVE555)/BasicUsageEnvironment/include  -I../$(LIVE555)/groupsock/include\
			-I../$(LIVE555)/liveMedia/include -I../$(LIVE555)/UsageEnvironment/include\
			-I../$(LIVE555)/live555/include/\
			-I../3rdparty/civetweb/include/
			
CXXFLAGS += -std=c++11
		
LDFLAGS+= -L$(VE_PATH)/output/$(VE_INSTALL_DIR)/lib
LDFLAGS+= -llive555 -lprotobuf  -lssl -lcrypto -lpthread -ldl -lstdc++ -lm

ifneq ($(strip $(VE_OS)), macos)
LDFLAGS += -Wl,-Bdynamic -lcrypt 
endif


LIBS = -lpthread -lm



ifdef WITH_DEBUG
  CFLAGS += -g -DDEBUG_ENABLED
else
  CFLAGS += -DNDEBUG -Os
endif

BUILD_DIRS += $(addprefix $(BUILD_DIR)/, $(SOURCE_DIRS))
BUILD_OBJECTS = $(addprefix $(BUILD_DIR)/, $(OBJECTS))
MAIN_OBJECTS = $(addprefix $(BUILD_DIR)/, $(APP_SOURCES:.c=.o))
LIB_OBJECTS = $(filter-out $(MAIN_OBJECTS), $(BUILD_OBJECTS))

ifeq ($(TARGET_OS),LINUX)
  LIBS += -lrt -ldl
  CAN_INSTALL = 1
endif

SHARED_LIB = so

all: build
	cp $(CPROG) $(VE_PATH)/output/$(VE_INSTALL_DIR)/bin

build: $(CPROG)

lib: lib$(CPROG).a

slib: lib$(CPROG).$(SHARED_LIB)

clean:
	$(RMRF) $(BUILD_DIR)
	$(RMRF) out.linux
	$(RMRF) lib$(CPROG).a
	$(RMRF) lib$(CPROG).so
	$(RMRF) $(CPROG)


distclean: clean
	$(RMF) $(CPROG) lib$(CPROG).so lib$(CPROG).a 

lib$(CPROG).a: CFLAGS += -fPIC
lib$(CPROG).a: $(LIB_OBJECTS)
	@$(RMF) $@
	ar cq $@ $(LIB_OBJECTS)

lib$(CPROG).so: CFLAGS += -fPIC
lib$(CPROG).so: $(LIB_OBJECTS)
	$(CPP) -shared -o $@ $(CFLAGS) $(LDFLAGS) $(LIB_OBJECTS)


$(CPROG): $(BUILD_OBJECTS) $(BUILD_RESOURCES)
	$(CPP) -o $@ $(CFLAGS)  $(BUILD_OBJECTS) $(BUILD_RESOURCES) $(LIBS) $(LDFLAGS)

$(CXXPROG): $(BUILD_OBJECTS)
	$(CPP) -o $@ $(CFLAGS) $(LDFLAGS) $(BUILD_OBJECTS) $(LIBS)

$(BUILD_OBJECTS): $(BUILD_DIRS)

$(BUILD_DIRS):
	-@$(MKDIR) "$@"

$(BUILD_DIR)/%.o : %.cpp
	$(CPP) -c $(CFLAGS) $(CXXFLAGS) $< -o $@
	

$(BUILD_DIR)/%.o : %.cc
	$(CPP) -c $(CFLAGS) $(CXXFLAGS) $< -o $@

$(BUILD_DIR)/%.o : %.c
	$(CC) -c $(CFLAGS) $< -o $@

.PHONY: all help build install clean lib so
