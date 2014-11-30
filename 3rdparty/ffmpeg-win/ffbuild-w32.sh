

sudo apt-get install subversion yasm apt-file autoconf \
libtool zlib1g-dev libbz2-dev libfribidi-dev intltool libglib2.0-dev

sudo apt-get install mingw-w*

./configure --prefix="/home/ubu/work/ffmpeg" --arch=i386 --enable-cross-compile --cross-prefix=i686-w64-mingw32- --target-os=mingw32 --enable-dxva2 --enable-decoder=h264_dxva2  --enable-hwaccel=h264_dxva2 --disable-encoders --disable-static --enable-shared --enable-runtime-cpudetect --enable-memalign-hack --disable-avdevice --disable-postproc --disable-muxers --enable-w32threads  --pkg-config=pkg-config
