all:vepro_build 


clean:vepro_clean 

vepro_build:
	cd ./prj_linux/ ; qmake .;
	$(MAKE) -C ./prj_linux;
	cp ./prj_linux/vdceyeManager ../linux/bin/ 

vepro_clean:
	cd ./prj_linux/ ; qmake .;
	$(MAKE) -C ./prj_linux/ clean;
	rm -rf ./prj_linux/vdceyeManager
	
	
