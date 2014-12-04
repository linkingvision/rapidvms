all:vepro_build 


clean:vepro_clean 

vepro_build:
	cd ./prj_linux/ ; qmake .;
	$(MAKE) -C ./prj_linux;
	cp ./prj_linux/vdcEyeManager ../linux/bin/ 

vepro_clean:
	cd ./prj_linux/ ; qmake .;
	$(MAKE) -C ./prj_linux/ clean;
	rm -rf ./prj_linux/vdcEyeManager
	
	
