
#Get OS version
OS=`lsb_release -si`
ARCH=`uname -m | sed 's/x86_//;s/i[3-6]86/32/'`
VER=`lsb_release -sr`
REL=OpenCVR-$(OS)-$(VER)-$(ARCH)bit

subdirs=3rdparty xcmnlib velib veuilib vecvr client

all:
	chmod +x ./output/linux/*.sh
	for d in $(subdirs); do (cd $$d; (if  test -e "Makefile"; then $(MAKE) $(MFLAGS); fi;) ); done
	strip ./output/$(VE_INSTALL_DIR)/lib/*.so
	cp -r ./output/linux/*.sh ./output/$(VE_INSTALL_DIR)/
clean:
	for d in $(subdirs); do (cd $$d; (if  test -e "Makefile"; then $(MAKE) clean; fi;) ); done
	#rm -rf ./linux/*.so ./linux/bin ./linux/lib/ ./linux/share ./linux/ssl ./linux/include 

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
	cp -r ./output/$(VE_INSTALL_DIR) $(REL)
	rm -rf ./$(REL)/lib/x86_64-linux-gnu
	rm -rf ./$(REL)/examples
	rm -rf ./$(REL)/doc
	rm -rf ./$(REL)/mkspecs
	mv ./$(REL)/bin/OpenCVR* .
	rm -rf ./$(REL)/bin/*
	mv ./OpenCVRC* ./$(REL)/bin/
	mv ./OpenCVRS* ./$(REL)/bin/
	rm -rf ./$(REL)/include
	rm -rf ./$(REL)/share
	rm -rf ./$(REL)/ssl
	rm -rf ./$(REL)/.svn
	rm -rf ./$(REL)/lib/*.a
	rm -rf ./$(REL)/lib/pkgconfig
	rm -rf ./$(REL)/lib/*.dbg
	rm -rf ./$(REL)/lib/libSDL*
	strip ./$(REL)/bin/OpenCVR*
	strip ./$(REL)/lib/*.so
	strip ./$(REL)/lib/*.so.*
	
