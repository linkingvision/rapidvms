$(eval version=$(shell grep "define VE_VERSION" include/config/confver.hpp | sed 's|.*VERSION "\(.*\)"|\1|g'))
REL=Rapidvms-$(version)-$(VE_OS)-$(VE_VER)-$(VE_ARCH)bit
RELC=RapidClient-$(version)-$(VE_OS)-$(VE_VER)-$(VE_ARCH)bit

ifeq ($(strip $(VE_CROSS_COMPILER)), )
subdirs=3rdparty xcmnlib velib veuilib vecvr client
appsubdirs=xcmnlib velib veuilib vecvr client
else
#embeded device only has the server
subdirs=3rdparty xcmnlib velib veuilib vecvr 
appsubdirs=xcmnlib velib veuilib vecvr
endif


all: prepare
	for d in $(subdirs); do (cd $$d; (if  test -e "Makefile"; then $(MAKE) $(MFLAGS); fi;) ); done

app: prepare
	for d in $(appsubdirs); do (cd $$d; (if  test -e "Makefile"; then $(MAKE) $(MFLAGS); fi;) ); done

prepare:
	mkdir -p ./output/$(VE_INSTALL_DIR)/lib
ifneq ($(strip $(VE_OS)), macos)
	chmod +x ./output/linux/*.sh
	cp -r ./output/linux/*.sh ./output/$(VE_INSTALL_DIR)/
	cp -r ./output/linux/service/ ./output/$(VE_INSTALL_DIR)/
	cp -r ./output/linux/fonts/ ./output/$(VE_INSTALL_DIR)/lib/
else
	chmod +x ./output/macos/*.sh
	cp -r ./output/macos/* ./output/$(VE_INSTALL_DIR)/
endif

clean:
	for d in $(subdirs); do (cd $$d; (if  test -e "Makefile"; then $(MAKE) clean; fi;) ); done
	#rm -rf ./linux/*.so ./linux/bin ./linux/lib/ ./linux/share ./linux/ssl ./linux/include
	rm -rf ./output/host/ 

install:
	for d in $(subdirs); do (cd $$d; (if  test -e "Makefile"; then $(MAKE) install; fi;) ); done

distclean: clean
	for d in $(subdirs); do (cd $$d; (if  test -e "Makefile"; then $(MAKE) distclean; fi;) ); done
disttest:
	cp ./output/$(VE_INSTALL_DIR)/lib/libve*.so .;
	cp ./output/$(VE_INSTALL_DIR)/lib/libcmnlib.so .;
	cp ./output/$(VE_INSTALL_DIR)/lib/libcover.so .;
	rm -rf ./output/$(VE_INSTALL_DIR)/;
	mkdir -p ./output/$(VE_INSTALL_DIR)/lib/;
	mv ./libve* libcmnlib.so libcover.so ./output/$(VE_INSTALL_DIR)/lib/
	strip ./output/$(VE_INSTALL_DIR)/lib/*.so

rel:
	echo $(REL)
	rm -rf ./$(REL)
	mkdir -p ./output/$(VE_INSTALL_DIR)/translations;
	cp -rf client/src/main/translations/*.qm ./output/$(VE_INSTALL_DIR)/translations;
	cp -rf veuilib/src/translations/*.qm ./output/$(VE_INSTALL_DIR)/translations;
ifneq ($(strip $(VE_OS)), macos)
	cp -r ./output/linux/*.sh ./output/$(VE_INSTALL_DIR)/
	cp -r ./output/linux/service/ ./output/$(VE_INSTALL_DIR)/
else
	cp -r ./output/macos/* ./output/$(VE_INSTALL_DIR)/
endif
	cp -rf ./output/$(VE_INSTALL_DIR) $(REL)
	rm -rf ./$(REL)/lib/x86_64-linux-gnu
	rm -rf ./$(REL)/examples
	rm -rf ./$(REL)/doc
	rm -rf ./$(REL)/mkspecs
	mv ./$(REL)/bin/openssl .
	rm -rf ./$(REL)/bin/*
ifeq ($(strip $(VE_CROSS_COMPILER)), )
	rm -rf ./RapidS* 
else
	rm -rf ./$(REL)/lib/libcover.so
	rm -rf ./$(REL)/*Client*
endif
	mv ./openssl ./$(REL)/bin/
	rm -rf ./$(REL)/include
	rm -rf ./$(REL)/share
	rm -rf ./$(REL)/ssl
	rm -rf ./$(REL)/.svn
	rm -rf ./$(REL)/lib/*.a
	rm -rf ./$(REL)/lib/pkgconfig
	rm -rf ./$(REL)/lib/*.dbg
	rm -rf ./$(REL)/lib/libSDL*
ifeq ($(strip $(VE_OS)), macos)
	$(VE_CROSS_COMPILER)strip ./$(REL)/RapidClient.app/Contents/MacOS/Rapid*
	$(VE_CROSS_COMPILER)strip ./$(REL)/RapidStor.app/Contents/MacOS/Rapid*
	install_name_tool -add_rpath @loader_path/../../../lib ./$(REL)/RapidClient.app/Contents/MacOS/RapidClient
	install_name_tool -add_rpath @loader_path/../../../lib ./$(REL)/RapidStor.app/Contents/MacOS/RapidStor
	#$(VE_CROSS_COMPILER)strip ./$(REL)/lib/libveuilib.dylib
	#$(VE_CROSS_COMPILER)strip ./$(REL)/lib/*ve*.so
	#$(VE_CROSS_COMPILER)strip ./$(REL)/lib/*cmn*.so
else
	$(VE_CROSS_COMPILER)strip ./$(REL)/Rapid*
	$(VE_CROSS_COMPILER)strip ./$(REL)/lib/*.so*
endif

#release client only for macos
relc:
ifeq ($(strip $(VE_OS)), macos)
	mkdir -p ./$(RELC)/RapidClient.app;
	cp -Rf ./output/$(VE_INSTALL_DIR)/RapidClient.app/ $(RELC)/RapidClient.app/;
	cp -rf ./output/$(VE_INSTALL_DIR)/lib/*.* ./$(RELC)/RapidClient.app/Contents/MacOS/
	$(VE_CROSS_COMPILER)strip ./$(RELC)/RapidClient.app/Contents/MacOS/Rapid*
	#$(VE_CROSS_COMPILER)strip ./$(RELC)/lib/libveuilib.dylib
	#$(VE_CROSS_COMPILER)strip ./$(RELC)/lib/*ve*.so
	#$(VE_CROSS_COMPILER)strip ./$(RELC)/lib/*cmn*.so
	rm -rf ./$(RELC)/RapidClient.app/Contents/MacOS/*.a
	rm -rf ./$(RELC)/RapidClient.app/Contents/MacOS/pkgconfig
	rm -rf ./$(RELC)/RapidClient.app/Contents/MacOS/*.dbg
	rm -rf ./$(RELC)/RapidClient.app/Contents/MacOS/libSDL*
endif
	
	
