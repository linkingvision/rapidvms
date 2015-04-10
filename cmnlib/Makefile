ifeq ($(strip $(CROSS_COMPILE)), )
#CROSS_COMPILE=
endif

DEBUG_ENABLE=yes

LIB_NAME=libcmnlib

AS              = $(CROSS_COMPILE)as
LD              = $(CROSS_COMPILE)ld
CC              = $(CROSS_COMPILE)gcc
CPP             = $(CROSS_COMPILE)g++
AR              = $(CROSS_COMPILE)ar
NM              = $(CROSS_COMPILE)nm
STRIP           = $(CROSS_COMPILE)strip
OBJCOPY         = $(CROSS_COMPILE)objcopy
OBJDUMP         = $(CROSS_COMPILE)objdump
RANLIB          = $(CROSS_COMPILE)ranlib

BIN     =./lib/linux/
CFLAGS	= -std=c++11 -DLINUX -DLINUX_OS -D__STDC_CONSTANT_MACROS 

LIBS	= $(BIN)/$(LIB_NAME).a

HDRS   = $(wildcard ./*.h) 

MACHINE := $(shell uname -m)

IFLAGS 	= -I. -I./include/vplay\
		-I./include/ffkit\
		-I./include/cppkit\
		-I./include/hyperhttp\
		-I./include/\
		-I../3rdparty/poco/Data/include\
		-I../3rdparty/poco/Foundation/include\
		-I../3rdparty/poco/Data/SQLite/include\
		-I$(VE_PATH)/linux/include\

DEPEND_FILES =  ./src/ffkit/argb24_to_yuv420p.o\
				./src/ffkit/av_demuxer.o\
				./src/ffkit/av_muxer.o\
				./src/ffkit/av_packet.o\
				./src/ffkit/av_packet_factory.o\
				./src/ffkit/h264_decoder.o\
				./src/ffkit/h264_encoder.o\
				./src/ffkit/h264mp4_to_annexb.o\
				./src/ffkit/h264_transcoder.o\
				./src/ffkit/jpeg_decoder.o\
				./src/ffkit/jpeg_encoder.o\
				./src/ffkit/fflocky.o\
				./src/ffkit/ffoptions.o\
				./src/ffkit/ffutils.o\
				./src/ffkit/yuv420p_to_argb24.o\
				./src/cppkit/ck_actor.o\
				./src/cppkit/ck_byte_ptr.o\
				./src/cppkit/ck_command_queue.o\
				./src/cppkit/ck_dynamic_library.o\
				./src/cppkit/ck_error_msg.o\
				./src/cppkit/ck_exception.o\
				./src/cppkit/ck_json.o\
				./src/cppkit/ck_large_files.o\
				./src/cppkit/ck_library_id.o\
				./src/cppkit/ck_logger.o\
				./src/cppkit/ck_memory.o\
				./src/cppkit/ck_memory_map.o\
				./src/cppkit/ck_nullable.o\
				./src/cppkit/ck_path.o\
				./src/cppkit/ck_pool.o\
				./src/cppkit/ck_socket_address.o\
				./src/cppkit/ck_socket.o\
				./src/cppkit/ck_sprintf.o\
				./src/cppkit/ck_stack_trace.o\
				./src/cppkit/ck_string.o\
				./src/cppkit/ck_string_slice.o\
				./src/cppkit/ck_timer.o\
				./src/cppkit/ck_time_utils.o\
				./src/cppkit/ck_udp_receiver.o\
				./src/cppkit/ck_udp_sender.o\
				./src/cppkit/ck_uuid_utils.o\
				./src/cppkit/ck_variant.o\
				./src/hyperhttp/client_request.o\
				./src/hyperhttp/client_response.o\
				./src/hyperhttp/hyperhttp_exception.o\
				./src/hyperhttp/hyperhttp_utils.o\
				./src/hyperhttp/methods.o\
				./src/hyperhttp/server_request.o\
				./src/hyperhttp/server_response.o\
				./src/hyperhttp/hyperuri.o\
				./src/hyperhttp/hyperurl.o\
				./src/XSDK/LargeFiles.o\
				./src/XSDK/OS.o\
				./src/XSDK/Rand.o\
				./src/XSDK/StackTrace.o\
				./src/XSDK/TimeUtils.o\
				./src/XSDK/UuidUtils.o\
				./src/XSDK/XBaseObject.o\
				./src/XSDK/XBlockingQueue.o\
				./src/XSDK/XCache.o\
				./src/XSDK/XCommandQueue.o\
				./src/XSDK/XDateTuple.o\
				./src/XSDK/XDomParser.o\
				./src/XSDK/XDomParserNode.o\
				./src/XSDK/XDuration.o\
				./src/XSDK/XDynamicLibrary.o\
				./src/XSDK/XException.o\
				./src/XSDK/XFullVariant.o\
				./src/XSDK/XGuard.o\
				./src/XSDK/XHash.o\
				./src/XSDK/XInternalList.o\
				./src/XSDK/XJSON.o\
				./src/XSDK/XListBox.o\
				./src/XSDK/XLogger.o\
				./src/XSDK/XMD5.o\
				./src/XSDK/XMemory.o\
				./src/XSDK/XMemoryMap.o\
				./src/XSDK/XMonoClock.o\
				./src/XSDK/XObjCount.o\
				./src/XSDK/XObjectProxy.o\
				./src/XSDK/XPath.o\
				./src/XSDK/XSocketAddress.o\
				./src/XSDK/XSocket.o\
				./src/XSDK/XSSLSocket.o\
				./src/XSDK/XStackTrace.o\
				./src/XSDK/XStopWatch.o\
				./src/XSDK/XString.o\
				./src/XSDK/XStringSlice.o\
				./src/XSDK/XSymbol.o\
				./src/XSDK/XSystemStats.o\
				./src/XSDK/XTaskBase.o\
				./src/XSDK/XTime.o\
				./src/XSDK/XTimedCommandQueue.o\
				./src/XSDK/XTimer.o\
				./src/XSDK/XTimeZone.o\
				./src/XSDK/XUDPReceiver.o\
				./src/XSDK/XUDPSender.o\
				./src/XSDK/XVariant.o\
				./src/XSDK/XUuid.o\
				./src/XSDK/POSIX/Sprintf.o\
				./src/XSDK/POSIX/XCondition.o\
				./src/XSDK/POSIX/XMutex.o\
				./src/AVKit/ARGB24ToYUV420P.o\
				./src/AVKit/AVDeMuxer.o\
				./src/AVKit/AVMuxer.o\
				./src/AVKit/H264Decoder.o\
				./src/AVKit/H264Encoder.o\
				./src/AVKit/H264MP4ToAnnexB.o\
				./src/AVKit/H264Transcoder.o\
				./src/AVKit/JPEGEncoder.o\
				./src/AVKit/Locky.o\
				./src/AVKit/Options.o\
				./src/AVKit/Utils.o\
				./src/AVKit/YUV420PToARGB24.o\
				./src/Webby/ClientSideRequest.o\
				./src/Webby/ClientSideResponse.o\
				./src/Webby/RequestBody.o\
				./src/Webby/ServerSideRequest.o\
				./src/Webby/ServerSideResponse.o\
				./src/Webby/SoapArgs.o\
				./src/Webby/URI.o\
				./src/Webby/URL.o\
				./src/Webby/WebbyException.o\
				./src/Webby/WebbyUtils.o\
				
				

ifeq ($(DEBUG_ENABLE), yes)
DEBUG	= -g -Wall
else
DEBUG   = -Wall
endif
		

all:  $(LIBS)

$(DEPEND_FILES): $(HDRS)


$(LIBS): $(DEPEND_FILES)
	$(AR) $(ARFLAGS) $(LIBS) $?

clean: 
	rm -f $(LIBS) $(DEPEND_FILES) 

distclean: distclean

install:

.cpp.o:
	$(CPP) -c -o $@ $(DEBUG) $(CFLAGS) $(IFLAGS) $<
.c.o:
	$(CC) -c -o $@ $(DEBUG) $(CFLAGS) $(IFLAGS) $<
