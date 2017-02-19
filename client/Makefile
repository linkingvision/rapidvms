all:vepro_build 


clean:vepro_clean 

vepro_build:
	cd ./prj_linux/ ; qmake .;
	$(MAKE) -C ./prj_linux;
	cp -r ./prj_linux/RapidClient* ../output/$(VE_INSTALL_DIR)/ 
ifeq ($(strip $(VE_OS)), macos)
	cp -r ../output/macos/RapidClient* ../output/$(VE_INSTALL_DIR)/
endif

vepro_clean:
	cd ./prj_linux/ ; qmake .;
	$(MAKE) -C ./prj_linux/ clean;
	rm -rf ./prj_linux/RapidClient*
	
	
