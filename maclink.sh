#!/bin/sh
export ve_libavcodec=libavcodec.57.dylib
export ve_libavformat=libavformat.57.dylib 
export ve_libavutil=libavutil.55.dylib
export ve_libavfilter=libavfilter.6.dylib
export ve_libswscale=libswscale.4.dylib
export ve_libswresample=libswresample.2.dylib

export ve_libPocoFoundation=libPocoFoundation.24.dylib
export ve_libPocoDataSQLite=libPocoDataSQLite.24.dylib
export ve_libPocoData=libPocoData.24.dylib
export ve_libPocoUtil=libPocoUtil.24.dylib
export ve_libPocoXML=libPocoXML.24.dylib
export ve_libPocoJSON=libPocoJSON.24.dylib

export ve_libcurl=libcurl.4.dylib

#Change RapidClient
cd $VE_PATH/output/$VE_INSTALL_DIR/RapidClient.app/Contents/MacOS

install_name_tool -add_rpath @loader_path/../../../lib RapidClient
install_name_tool -change libcmnlib.so @rpath/libcmnlib.so RapidClient

install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libavcodec @rpath/$ve_libavcodec RapidClient
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libavformat @rpath/$ve_libavformat RapidClient
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libavutil @rpath/$ve_libavutil RapidClient
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libavfilter @rpath/$ve_libavfilter RapidClient
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libswscale @rpath/$ve_libswscale RapidClient
install_name_tool -change libvelib.so @rpath/libvelib.so RapidClient
install_name_tool -change libveuilib.dylib @rpath/libveuilib.dylib RapidClient


install_name_tool -change $VE_PATH/3rdparty/poco/lib/Darwin/x86_64/$ve_libPocoFoundation @rpath/$ve_libPocoFoundation RapidClient
install_name_tool -change $VE_PATH/3rdparty/poco/lib/Darwin/x86_64/$ve_libPocoDataSQLite @rpath/$ve_libPocoDataSQLite RapidClient
install_name_tool -change $VE_PATH/3rdparty/poco/lib/Darwin/x86_64/$ve_libPocoData @rpath/$ve_libPocoData RapidClient
install_name_tool -change $VE_PATH/3rdparty/poco/lib/Darwin/x86_64/$ve_libPocoUtil @rpath/$ve_libPocoUtil RapidClient

#Change RapidStor
cd $VE_PATH/output/$VE_INSTALL_DIR/RapidStor.app/Contents/MacOS
install_name_tool -add_rpath @loader_path/../../../lib RapidStor
install_name_tool -change libcmnlib.so @rpath/libcmnlib.so RapidStor
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libavcodec @rpath/$ve_libavcodec RapidStor
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libavformat @rpath/$ve_libavformat RapidStor
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libavutil @rpath/$ve_libavutil RapidStor
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libavfilter @rpath/$ve_libavfilter RapidStor
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libswscale @rpath/$ve_libswscale RapidStor
install_name_tool -change libvelib.so @rpath/libvelib.so RapidStor
install_name_tool -change libveuilib.dylib @rpath/libveuilib.dylib RapidStor


install_name_tool -change $VE_PATH/3rdparty/poco/lib/Darwin/x86_64/$ve_libPocoFoundation @rpath/$ve_libPocoFoundation RapidStor
install_name_tool -change $VE_PATH/3rdparty/poco/lib/Darwin/x86_64/$ve_libPocoDataSQLite @rpath/$ve_libPocoDataSQLite RapidStor
install_name_tool -change $VE_PATH/3rdparty/poco/lib/Darwin/x86_64/$ve_libPocoData @rpath/$ve_libPocoData RapidStor
install_name_tool -change $VE_PATH/3rdparty/poco/lib/Darwin/x86_64/$ve_libPocoUtil @rpath/$ve_libPocoUtil RapidStor

#Change Lib 
cd $VE_PATH/output/$VE_INSTALL_DIR/lib

#libcmnlib.so
install_name_tool -change liblive555.so @rpath/liblive555.so libcmnlib.so
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libavcodec @rpath/$ve_libavcodec libcmnlib.so
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libavformat @rpath/$ve_libavformat libcmnlib.so
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libavutil @rpath/$ve_libavutil libcmnlib.so
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libavfilter @rpath/$ve_libavfilter libcmnlib.so
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libswscale @rpath/$ve_libswscale libcmnlib.so
install_name_tool -change $VE_PATH/3rdparty/poco/lib/Darwin/x86_64/$ve_libPocoFoundation @rpath/$ve_libPocoFoundation libcmnlib.so
install_name_tool -change $VE_PATH/3rdparty/poco/lib/Darwin/x86_64/$ve_libPocoDataSQLite @rpath/$ve_libPocoDataSQLite libcmnlib.so
install_name_tool -change $VE_PATH/3rdparty/poco/lib/Darwin/x86_64/$ve_libPocoData @rpath/$ve_libPocoData libcmnlib.so
install_name_tool -change $VE_PATH/3rdparty/poco/lib/Darwin/x86_64/$ve_libPocoUtil @rpath/$ve_libPocoUtil libcmnlib.so
install_name_tool -change $VE_PATH/3rdparty/poco/lib/Darwin/x86_64/libPocoNet.24.dylib @rpath/libPocoNet.24.dylib libcmnlib.so


#libvelib.so 
install_name_tool -change libcmnlib.so @rpath/libcmnlib.so libvelib.so 
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libavcodec @rpath/$ve_libavcodec libvelib.so
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libavformat @rpath/$ve_libavformat libvelib.so
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libavutil @rpath/$ve_libavutil libvelib.so
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libavfilter @rpath/$ve_libavfilter libvelib.so
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libswscale @rpath/$ve_libswscale libvelib.so
install_name_tool -change $VE_PATH/3rdparty/poco/lib/Darwin/x86_64/$ve_libPocoFoundation @rpath/$ve_libPocoFoundation libvelib.so
install_name_tool -change $VE_PATH/3rdparty/poco/lib/Darwin/x86_64/$ve_libPocoDataSQLite @rpath/$ve_libPocoDataSQLite libvelib.so
install_name_tool -change $VE_PATH/3rdparty/poco/lib/Darwin/x86_64/$ve_libPocoData @rpath/$ve_libPocoData libvelib.so
install_name_tool -change $VE_PATH/3rdparty/poco/lib/Darwin/x86_64/$ve_libPocoUtil @rpath/$ve_libPocoUtil libvelib.so
install_name_tool -change $VE_PATH/3rdparty/poco/lib/Darwin/x86_64/libPocoNet.24.dylib @rpath/libPocoNet.24.dylib libvelib.so

#$ve_libcurl
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR/lib/$ve_libcurl @rpath/$ve_libcurl libvelib.so

#libveuilib.dylib 
install_name_tool -change libcmnlib.so @rpath/libcmnlib.so libveuilib.dylib
install_name_tool -change libvelib.so @rpath/libvelib.so libveuilib.dylib 
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libavcodec @rpath/$ve_libavcodec libveuilib.dylib 
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libavformat @rpath/$ve_libavformat libveuilib.dylib 
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libavutil @rpath/$ve_libavutil libveuilib.dylib 
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libavfilter @rpath/$ve_libavfilter libveuilib.dylib 
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libswscale @rpath/$ve_libswscale libveuilib.dylib 

install_name_tool -change $VE_PATH/3rdparty/poco/lib/Darwin/x86_64/$ve_libPocoDataSQLite @rpath/$ve_libPocoDataSQLite libveuilib.dylib 
install_name_tool -change $VE_PATH/3rdparty/poco/lib/Darwin/x86_64/$ve_libPocoData @rpath/$ve_libPocoData libveuilib.dylib 
install_name_tool -change $VE_PATH/3rdparty/poco/lib/Darwin/x86_64/$ve_libPocoFoundation @rpath/$ve_libPocoFoundation libveuilib.dylib 
install_name_tool -change $VE_PATH/3rdparty/poco/lib/Darwin/x86_64/$ve_libPocoUtil @rpath/$ve_libPocoUtil libveuilib.dylib 
install_name_tool -change $VE_PATH/3rdparty/poco/lib/Darwin/x86_64/libPocoNet.24.dylib @rpath/libPocoNet.24.dylib libveuilib.dylib 


#$ve_libPocoDataSQLite 
install_name_tool -change $VE_PATH/3rdparty/poco/lib/Darwin/x86_64/$ve_libPocoData @rpath/$ve_libPocoData $ve_libPocoDataSQLite 
install_name_tool -change $VE_PATH/3rdparty/poco/lib/Darwin/x86_64/$ve_libPocoFoundation @rpath/$ve_libPocoFoundation $ve_libPocoDataSQLite 

#$ve_libPocoData
install_name_tool -change $VE_PATH/3rdparty/poco/lib/Darwin/x86_64/$ve_libPocoFoundation @rpath/$ve_libPocoFoundation $ve_libPocoData 

#libPocoNet.24.dylib
install_name_tool -change $VE_PATH/3rdparty/poco/lib/Darwin/x86_64/$ve_libPocoFoundation @rpath/$ve_libPocoFoundation libPocoNet.24.dylib 

#$ve_libPocoUtil
install_name_tool -change $VE_PATH/3rdparty/poco/lib/Darwin/x86_64/$ve_libPocoFoundation @rpath/$ve_libPocoFoundation $ve_libPocoUtil
install_name_tool -change $VE_PATH/3rdparty/poco/lib/Darwin/x86_64/$ve_libPocoXML @rpath/$ve_libPocoXML $ve_libPocoUtil
install_name_tool -change $VE_PATH/3rdparty/poco/lib/Darwin/x86_64/$ve_libPocoJSON @rpath/$ve_libPocoJSON $ve_libPocoUtil

#$ve_libPocoXML
install_name_tool -change $VE_PATH/3rdparty/poco/lib/Darwin/x86_64/$ve_libPocoFoundation @rpath/$ve_libPocoFoundation $ve_libPocoXML

#$ve_libPocoJSON
install_name_tool -change $VE_PATH/3rdparty/poco/lib/Darwin/x86_64/$ve_libPocoFoundation @rpath/$ve_libPocoFoundation $ve_libPocoJSON


#$ve_libavfilter 
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libavcodec @rpath/$ve_libavcodec $ve_libavfilter  
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libswscale @rpath/$ve_libswscale $ve_libavfilter  
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libavformat @rpath/$ve_libavformat $ve_libavfilter 
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libavutil @rpath/$ve_libavutil $ve_libavfilter
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libswresample @rpath/$ve_libswresample $ve_libavfilter

#$ve_libswscale
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libavutil @rpath/$ve_libavutil $ve_libswscale

#$ve_libavformat
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libavcodec @rpath/$ve_libavcodec $ve_libavformat
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libswresample @rpath/$ve_libswresample $ve_libavformat
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libavutil @rpath/$ve_libavutil $ve_libavformat

#$ve_libavcodec
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libswresample @rpath/$ve_libswresample $ve_libavcodec
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libavutil @rpath/$ve_libavutil $ve_libavcodec

#$ve_libavutil
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libswresample @rpath/$ve_libswresample $ve_libavutil

#$ve_libswresample
install_name_tool -change $VE_PATH/output/$VE_INSTALL_DIR//lib/$ve_libavutil @rpath/$ve_libavutil $ve_libswresample

