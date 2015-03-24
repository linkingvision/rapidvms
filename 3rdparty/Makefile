all:poco_build ffmpeg_build openssl_build leveldb_build onvifc_build

clean:distclean
distclean:poco_clean ffmpeg_clean openssl_clean leveldb_clean onvifc_clean

poco_build:
	cd poco; chmod +x ./configure; chmod +x ./build/script/*; ./configure --no-tests --no-samples --omit=Data/ODBC,Data/MySQL,Crypto,MongoDB,NetSSL_OpenSSL,CppUnit,PDF --prefix=$(VE_PATH)/linux;
	$(MAKE) -C poco && $(MAKE) -C poco install;

ffmpeg_build:
	cd ffmpeg; chmod +x ./configure; chmod +x version.sh; ./configure  --disable-vaapi --disable-programs  --disable-static --enable-shared --prefix=$(VE_PATH)/linux;
	$(MAKE) -C ffmpeg && $(MAKE) -C ffmpeg install;

openssl_build:
	cd openssl-linux; chmod +x ./config; chmod +x ./util/pod2mantest; chmod +x ./util/point.sh; ./config --prefix=$(VE_PATH)/linux -fPIC;
	$(MAKE) -C openssl-linux && $(MAKE) -C openssl-linux install;
leveldb_build:
	cd leveldb; chmod +x ./build_detect_platform;
	$(MAKE) -C leveldb; 
onvifc_build:
	cd onvifc/prj_linux/ ; qmake .;
	$(MAKE) -C onvifc/prj_linux; 

poco_clean:
	$(MAKE) -C poco clean
ffmpeg_clean:
	$(MAKE) -C ffmpeg clean
openssl_clean:
	$(MAKE) -C openssl-linux clean
leveldb_clean:
	$(MAKE) -C leveldb-linux clean
onvifc_clean:
	cd onvifc/prj_linux/ ; qmake .;
	$(MAKE) -C onvifc/prj_linux/ clean
	
	
