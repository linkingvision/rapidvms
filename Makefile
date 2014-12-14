
#Get OS version
OS=`lsb_release -si`
ARCH=`uname -m | sed 's/x86_//;s/i[3-6]86/32/'`
VER=`lsb_release -sr`
REL=vdceye-$(OS)-$(VER)-$(ARCH)bit

subdirs=3rdparty velib veuilib veapp

all:
	chmod +x ./linux/*.sh
	for d in $(subdirs); do (cd $$d; $(MAKE) $(MFLAGS) ); done

clean:
	for d in $(subdirs); do (cd $$d; $(MAKE) clean ); done
	rm -rf ./linux/bin ./linux/lib/ ./linux/share ./linux/ssl ./linux/include 

install:
	for d in $(subdirs); do (cd $$d; $(MAKE) install ); done

distclean: clean
	for d in $(subdirs); do (cd $$d; $(MAKE) distclean ); done

rel:
	echo $(REL)
	rm -rf ./$(REL)
	cp -r ./linux $(REL)
	rm -rf ./$(REL)/bin/c*
	rm -rf ./$(REL)/bin/f*
	rm -rf ./$(REL)/bin/o*
	rm -rf ./$(REL)/bin/s*
	rm -rf ./$(REL)/include
	rm -rf ./$(REL)/share
	rm -rf ./$(REL)/ssl
	rm -rf ./$(REL)/.svn
	rm -rf ./$(REL)/lib/*.a
	rm -rf ./$(REL)/lib/pkgconfig
	rm -rf ./$(REL)/lib/*.dbg
	rm -rf ./$(REL)/lib/libSDL*
	strip ./$(REL)/bin/vdcEyeManager
	strip ./$(REL)/lib/*.so
	strip ./$(REL)/lib/*.so.*
	
