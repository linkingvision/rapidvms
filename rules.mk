export VE_PATH=/home/user/opencvr/

export VE_OS=`lsb_release -si`
export VE_ARCH=`uname -m | sed 's/x86_//;s/i[3-6]86/32/'`
export VE_VER=`lsb_release -sr`
export VE_CROSS_COMPILER=
export VE_INSTALL_DIR=$VE_OS-$VE_VER-$VE_ARCH"bit"
export LD_LIBRARY_PATH=$VE_PATH/output/$VE_INSTALL_DIR/lib/:$LD_LIBRARY_PATH
export PATH=$VE_PATH/output/$VE_INSTALL_DIR"/bin:"$PATH 


