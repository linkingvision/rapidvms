#!/bin/sh

fail ()
    { 
    echo "$0 TESTS FAILED";
    exit 1;
    }

echo $0 TESTS BEGIN

#these tests should all return zero (parse succeeded)
./test_int --help || fail
./test_int 0 || fail
./test_int 1 -- -1 || fail
./test_int 1 2 -- -3 || fail
./test_int 5 7 9 -d -21 || fail
./test_int -d 1 -D 1 --delta 1 -- -3 || fail
./test_int 1 2 4 --eps -7 || fail
./test_int 1 2 4 --eqn -7 || fail
./test_int 1 2 3 -D4 --eps -10 || fail
./test_int 1 -f || fail
./test_int -f 1 || fail
./test_int -f 2 --filler || fail
./test_int -f 1 --filler=1 -f || fail

./test_int 0x0 || fail
./test_int 0x10 -- -0x10 || fail
./test_int 0X10 0x32 -- -0x42 || fail
./test_int 0x5 0xA 0xF -d -0x1e || fail
./test_int -d 0xab -D0x09 --delta 0x02e -- -226 || fail

./test_int 0o0 || fail
./test_int 0o10 -- -0o10 || fail
./test_int 0o67 0O23 -- -0o112 || fail
./test_int 0o5 0o0 0x1 -d -0o6 || fail
./test_int -d 0o012 -D0o0777 --delta 0o56 -- -0o1067 || fail

./test_int 0B0 || fail
./test_int 0b10 -- -0b10 || fail
./test_int 0B10110 0b1110001 -- -0b10000111 || fail
./test_int 0b101001 0b101 0b00101010101 -d -0B110000011 || fail
./test_int -d 0b101 -D0B11 --delta 0b11011 -- -35 || fail

./test_int 11 0x11 0o11 -D0b11 --eps -0o50 || fail

./test_int 1024 --eps -1KB || fail
./test_int 1048576 --eps -1MB || fail
./test_int 1073741824 --eps -1GB || fail
./test_int 0x5KB 0xAMB 0x1GB -d -0x40A01400 || fail

#these tests should all return non-zero (parse failed)
./test_int && fail
./test_int 1 2 3 4 && fail
./test_int 1 2 3 -d1 -d2 -d3 -d4 && fail
./test_int 1 2 3 -d1 -d2 -d3 -d && fail
./test_int 1 2 3 -d1 -d2 -d && fail
./test_int 1 2 3 -d1 -d && fail
./test_int 1 2 3 -d && fail
./test_int 1 2 3 --eps && fail
./test_int 1 2 3 --eps 3 --eqn 6 && fail
./test_int hello && fail
./test_int 1.234 && fail
./test_int 4 hello && fail
./test_int 5 1.234 && fail
./test_int -f 2 --filler= && fail
./test_int 0x0g && fail
./test_int 0o08 && fail
./test_int 0b02 && fail
./test_int 1000GB && fail
./test_int 1GBH && fail


echo "$0 TESTS PASSED"
echo "----------------------------------"

