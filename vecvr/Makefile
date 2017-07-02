all:mediaserver_build vepro_build 


clean:vepro_clean mediaserver_clean

vepro_build:
	cd ./prj_linux/ ; qmake .;
	$(MAKE) -C ./prj_linux;
	cp -r ./prj_linux/RapidStor* ../output/$(VE_INSTALL_DIR)/ 
ifeq ($(strip $(VE_OS)), macos)
	cp -r ../output/macos/RapidStor* ../output/$(VE_INSTALL_DIR)/
endif

mediaserver_build:
	cd src/ZLToolKit; mkdir -p build; cd build;cmake .. -DOPENSSL_ROOT_DIR=$(VE_PATH)/output/$(VE_INSTALL_DIR)/ -DCMAKE_INSTALL_PREFIX=$(VE_PATH)/output/$(VE_INSTALL_DIR)/; make; make install;
	#delete shared library
ifeq ($(strip $(VE_OS)), macos)
	rm -rf $(VE_PATH)/output/$(VE_INSTALL_DIR)/lib/libZLToolKit.dylib
else
	rm -rf $(VE_PATH)/output/$(VE_INSTALL_DIR)/lib/libZLToolKit.so
endif
	cd src/ZLMediaKit; mkdir -p build; cd build;cmake .. -DOPENSSL_ROOT_DIR=$(VE_PATH)/output/$(VE_INSTALL_DIR)/ -DCMAKE_INSTALL_PREFIX=$(VE_PATH)/output/$(VE_INSTALL_DIR)/; make; make install;
	#delete shared library
ifeq ($(strip $(VE_OS)), macos)
	rm -rf $(VE_PATH)/output/$(VE_INSTALL_DIR)/lib/libZLMediaKit.dylib
else
	rm -rf $(VE_PATH)/output/$(VE_INSTALL_DIR)/lib/libZLMediaKit.so
endif

vepro_clean:
	cd ./prj_linux/ ; qmake .;
	$(MAKE) -C ./prj_linux/ clean;
	rm -rf ./prj_linux/RapidStor*

mediaserver_clean:
	rm -rf src/ZLToolKit/build
	rm -rf src/ZLMediaKit/build

	
	
