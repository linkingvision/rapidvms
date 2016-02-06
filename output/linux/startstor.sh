export LD_LIBRARY_PATH=$`pwd`/lib/:$LD_LIBRARY_PATH

export QT_QPA_FONTDIR=.
./bin/OpenCVRStor -platform offscreen &
