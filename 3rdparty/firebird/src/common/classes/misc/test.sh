rm -f core

# Test for library integrity
# this should be compiled with optimization turned off
ulimit -s unlimited
ulimit -c unlimited

DEBUGGING="-ggdb -DDEV_BUILD"

g++ $DEBUGGING -pthread -Wno-invalid-offsetof -Wno-inline-new-delete class_test.cpp ../alloc.cpp -I ../../../include/gen -I ../../../include -I.. -DLINUX -DAMD64 -fmax-errors=1 \
../../fb_exception.cpp ../locks.cpp ../../DynamicStrings.cpp ../../utils.cpp ../fb_string.cpp \
../init.cpp ../../StatusArg.cpp \
../../os/posix/os_utils.cpp ../../os/posix/path_utils.cpp ../../os/posix/guid.cpp ../../os/posix/fbsyslog.cpp \
-lfbclient
