export LD_LIBRARY_PATH=`pwd`/lib/:$LD_LIBRARY_PATH
export QT_QPA_FONTDIR=`pwd`/lib/fonts/
export QT_QPA_PLATFORM_PLUGIN_PATH=`pwd`/plugins/platforms/

./bin/OpenCVRStor -platform offscreen &
