-include ../common.mk
all:poco_build ffmpeg_build openssl_build leveldb_build qt_build  qtxmlpatterns_build  onvifc_build protobuf_build curl_build

clean:distclean
distclean:poco_clean ffmpeg_clean openssl_clean leveldb_clean qt_clean qtxmlpatterns_clean  onvifc_clean protobuf_clean curl_clean

poco_build:
	cd poco; chmod +x ./configure; chmod +x ./build/script/*; ./configure --no-tests --no-samples --omit=Data/ODBC,Data/MySQL,Crypto,MongoDB,NetSSL_OpenSSL,CppUnit,PDF --prefix=$(VE_PATH)/output/$(VE_INSTALL_DIR)/;
	$(MAKE) -C poco && $(MAKE) -C poco install;

ffmpeg_build:
	cd ffmpeg; chmod +x ./configure; chmod +x version.sh; ./configure  --disable-vaapi --disable-programs  --disable-static --enable-shared --prefix=$(VE_PATH)/output/$(VE_INSTALL_DIR)/;
	$(MAKE) -C ffmpeg && $(MAKE) -C ffmpeg install;

openssl_build:
	cd openssl; chmod +x ./config; chmod +x ./util/pod2mantest; chmod +x ./util/point.sh; ./config --prefix=$(VE_PATH)/output/$(VE_INSTALL_DIR)/ -fPIC;
	$(MAKE) -C openssl && $(MAKE) -C openssl install;
leveldb_build:
	cd leveldb; chmod +x ./build_detect_platform;
	$(MAKE) -C leveldb;
	rm -rf ./leveldb/libleveldb.so* 
onvifc_build:
	cd onvifc/prj_linux/ ; qmake .;
	$(MAKE) -C onvifc/prj_linux; 
protobuf_build:
	mkdir -p protobuf/cmake/linux;
	cd protobuf/cmake/linux && cmake -DCMAKE_CXX_FLAGS=-fPIC -DCMAKE_C_FLAGS=-fPIC -Dprotobuf_MSVC_STATIC_RUNTIME=ON -DCMAKE_BUILD_TYPE=Release -Dprotobuf_BUILD_TESTS=OFF -DCMAKE_INSTALL_PREFIX=$(VE_PATH)/output/$(VE_INSTALL_DIR)/  ../;
	$(MAKE) -C protobuf/cmake/linux;
	$(MAKE) -C protobuf/cmake/linux install;
qt_build:
	cd qt && tar zxvf qtbase-opensource-src-5.5.1.tar.gz ; 
	cd qt/qtbase-opensource-src-5.5.1; chmod +x ./configure ;./configure  -confirm-license -opensource --prefix=$(VE_PATH)/output/$(VE_INSTALL_DIR)/
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
	
poco_clean:
	$(MAKE) -C poco clean
ffmpeg_clean:
	$(MAKE) -C ffmpeg clean
openssl_clean:
	$(MAKE) -C openssl clean
leveldb_clean:
	$(MAKE) -C leveldb clean
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
