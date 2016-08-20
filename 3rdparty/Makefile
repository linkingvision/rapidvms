-include ../common.mk

ifeq ($(strip $(PRJ_CROSS)), )
all:poco_build ffmpeg_build openssl_build leveldb_build qt_build  qtxmlpatterns_build  onvifc_build protobuf_build curl_build
endif

ifeq ($(strip $(VE_OS)), OSX)
all:poco_build ffmpeg_build openssl_build_osx leveldb_build qt_build  qtxmlpatterns_build  onvifc_build protobuf_build curl_build
endif

ifneq ($(strip $(PRJ_CROSS)), )
all:poco_build_cross ffmpeg_build_cross openssl_build_cross leveldb_build qt_build_cross  qtxmlpatterns_build  onvifc_build protobuf_build_cross curl_build_cross
endif


clean:distclean
distclean:poco_clean ffmpeg_clean openssl_clean leveldb_clean qt_clean qtxmlpatterns_clean  onvifc_clean protobuf_clean curl_clean

poco_build:
	cd poco; chmod +x ./configure; chmod +x ./build/script/*; ./configure --no-tests --no-samples --omit=Data/ODBC,Data/MySQL,Crypto,MongoDB,NetSSL_OpenSSL,CppUnit,PDF --prefix=$(VE_PATH)/output/$(VE_INSTALL_DIR)/;
	$(MAKE) -C poco && $(MAKE) -C poco install;

poco_build_cross:
	cd poco; chmod +x ./configure; chmod +x ./build/script/*; ./configure --config=ARM-Linux --no-tests --no-samples --omit=Data/ODBC,Data/MySQL,Crypto,MongoDB,NetSSL_OpenSSL,CppUnit,PDF --prefix=$(VE_PATH)/output/$(VE_INSTALL_DIR)/;
	$(MAKE) -C poco && $(MAKE) -C poco install;

ffmpeg_build:
	cd ffmpeg; chmod +x ./configure; chmod +x version.sh; ./configure  --disable-vaapi --disable-programs  --disable-static --enable-shared --prefix=$(VE_PATH)/output/$(VE_INSTALL_DIR)/;
	$(MAKE) -C ffmpeg && $(MAKE) -C ffmpeg install;

ffmpeg_build_cross:
	cd ffmpeg; chmod +x ./configure; chmod +x version.sh; ./configure --enable-cross-compile --cross-prefix=$(VE_CROSS_COMPILER) --arch=arm --target-os=linux --disable-vaapi --disable-programs  --disable-static --enable-shared --prefix=$(VE_PATH)/output/$(VE_INSTALL_DIR)/;
	$(MAKE) -C ffmpeg && $(MAKE) -C ffmpeg install;

openssl_build:
	cd openssl; chmod +x ./config; chmod +x ./util/pod2mantest; chmod +x ./util/point.sh; ./config --prefix=$(VE_PATH)/output/$(VE_INSTALL_DIR)/ -fPIC;
	$(MAKE) -C openssl && $(MAKE) -C openssl install;

openssl_build_osx:
	cd openssl; chmod +x ./config; chmod +x ./util/pod2mantest; chmod +x ./util/point.sh; ./Configure darwin64-x86_64-cc --prefix=$(VE_PATH)/output/$(VE_INSTALL_DIR)/ -fPIC;
	$(MAKE) -C openssl && $(MAKE) -C openssl install_sw;

openssl_build_cross:
	cd openssl; chmod +x ./config; chmod +x ./util/pod2mantest; chmod +x ./util/point.sh; ./config --prefix=$(VE_PATH)/output/$(VE_INSTALL_DIR)/ os/compiler:$(VE_CROSS_COMPILER)gcc  -fPIC;
	$(MAKE) -C openssl && $(MAKE) -C openssl install;

leveldb_build:
	cd leveldb-posix; chmod +x ./build_detect_platform ;
	$(MAKE) -C leveldb-posix CC=$(CC) CXX=$(CPP);
	cp leveldb-posix/out-static/*.a $(VE_PATH)/output/$(VE_INSTALL_DIR)/lib/ 

onvifc_build:
	cd onvifc/prj_linux/ ; qmake .;
	$(MAKE) -C onvifc/prj_linux; 

protobuf_build:
	mkdir -p protobuf/cmake/linux;
	cd protobuf/cmake/linux && cmake -DCMAKE_CXX_FLAGS=-fPIC -DCMAKE_C_FLAGS=-fPIC -Dprotobuf_MSVC_STATIC_RUNTIME=ON -DCMAKE_BUILD_TYPE=Release -Dprotobuf_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=$(VE_PATH)/output/$(VE_INSTALL_DIR)/  ../;
	$(MAKE) -C protobuf/cmake/linux;
	$(MAKE) -C protobuf/cmake/linux install;

protobuf_build_cross:
	cd protobuf; chmod +x ./configure ; make distclean; ./configure --prefix=$(VE_PATH)/output/host/
	$(MAKE) -C protobuf; 
	$(MAKE) -C protobuf install; 
	cd protobuf; chmod +x ./configure ; make distclean; ./configure --host=$(VE_CROSS_TOOL) CC=$(CC) CXX=$(CPP)  --prefix=$(VE_PATH)/output/$(VE_INSTALL_DIR)/ --with-protoc=$(VE_PATH)/output/host/bin/protoc
	$(MAKE) -C protobuf; 
	$(MAKE) -C protobuf install; 

qt_build:
	cd qt && tar zxvf qtbase-opensource-src-5.5.1.tar.gz ; 
	cd qt/qtbase-opensource-src-5.5.1; chmod +x ./configure ;./configure  -confirm-license -release -make libs -opensource --prefix=$(VE_PATH)/output/$(VE_INSTALL_DIR)/
	$(MAKE) -C qt/qtbase-opensource-src-5.5.1; 
	$(MAKE) -C qt/qtbase-opensource-src-5.5.1 install; 

qt_build_cross:
	cd qt && rm -rf pcre-8.39
	cd qt && unzip pcre-8.39.zip;
	cd qt/pcre-8.39; chmod +x ./configure ; ./configure --enable-pcre16  CC=$(VE_CROSS_COMPILER)gcc --host=$(VE_CROSS_TOOL)  --prefix=$(VE_PATH)/output/$(VE_INSTALL_DIR)/
	$(MAKE) -C qt/pcre-8.39;
	$(MAKE) -C qt/pcre-8.39 install; 
	cd qt && tar zxvf qtbase-opensource-src-5.5.1.tar.gz ; 
	cp -r qt/linux-cross-g++ qt/qtbase-opensource-src-5.5.1/mkspecs/devices
	cd qt/qtbase-opensource-src-5.5.1; chmod +x ./configure ;./configure  -confirm-license -opensource --prefix=$(VE_PATH)/output/$(VE_INSTALL_DIR)/ -device linux-cross-g++ -device-option CROSS_COMPILE=$(VE_CROSS_COMPILER) -hostprefix $(VE_PATH)/output/$(VE_INSTALL_DIR)/  -release -make libs -no-qpa-platform-guard -L $(VE_PATH)/output/$(VE_INSTALL_DIR)/lib/ -I $(VE_PATH)/output/$(VE_INSTALL_DIR)/include/ 
	#-system-pcre
	$(MAKE) -C qt/qtbase-opensource-src-5.5.1; 
	$(MAKE) -C qt/qtbase-opensource-src-5.5.1 install; 

qtscript_build:
	cd qt && tar zxvf qtscript-opensource-src-5.5.1.tar.gz ;
	cd qt/qtscript-opensource-src-5.5.1; qmake PREFIX=$(VE_PATH)/output/$(VE_INSTALL_DIR)/
	$(MAKE) -C qt/qtscript-opensource-src-5.5.1; 
	$(MAKE) -C qt/qtscript-opensource-src-5.5.1 install; 
qtxmlpatterns_build:
	cd qt && tar zxvf qtxmlpatterns-opensource-src-5.5.1.tar.gz ;
	cd qt/qtxmlpatterns-opensource-src-5.5.1;qmake PREFIX=$(VE_PATH)/output/$(VE_INSTALL_DIR)/ 
	$(MAKE) -C qt/qtxmlpatterns-opensource-src-5.5.1; 
	$(MAKE) -C qt/qtxmlpatterns-opensource-src-5.5.1 install;
curl_build:
	cd curl; chmod +x ./configure; ./configure --prefix=$(VE_PATH)/output/$(VE_INSTALL_DIR)/;
	$(MAKE) -C curl && $(MAKE) -C curl install;

curl_build_cross:
	cd curl; chmod +x ./configure; ./configure --host=$(VE_CROSS_TOOL) --prefix=$(VE_PATH)/output/$(VE_INSTALL_DIR)/;
	$(MAKE) -C curl && $(MAKE) -C curl install;
	
poco_clean:
	$(MAKE) -C poco clean
ffmpeg_clean:
	$(MAKE) -C ffmpeg clean
openssl_clean:
	$(MAKE) -C openssl clean
leveldb_clean:
	$(MAKE) -C leveldb-posix clean
onvifc_clean:
	cd onvifc/prj_linux/ ; qmake .;
	$(MAKE) -C onvifc/prj_linux/ clean
protobuf_clean:
	rm -rf protobuf/cmake/linux/*.*
qt_clean:
	rm -rf qt/qtbase-opensource-src-5.5.1	
qtxmlpatterns_clean:
	rm -rf qt/qtxmlpatterns-opensource-src-5.5.1;

qtscript_clean:
	rm -rf qt/qtscript-opensource-src-5.5.1;
curl_clean:
	$(MAKE) -C curl clean
