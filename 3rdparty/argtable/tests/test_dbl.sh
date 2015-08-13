#!/bin/sh

fail ()
    { 
    echo "$0 TESTS FAILED";
    exit 1;
    }

echo $0 TESTS BEGIN

#these tests should all return zero (parse succeeded)
./test_dbl --help || fail
./test_dbl 0 || fail
./test_dbl 1.234 -- -1234e-3 || fail
./test_dbl 1.8 2.3 -- -4.1 || fail
./test_dbl 5 7 9 -d -21 || fail
./test_dbl -d 13e-1 -D 17e-1 --delta 36e-1 -- -66e-1 || fail
./test_dbl 1.2 2.3 4.5 --eps -8.0 || fail
./test_dbl 1.2 2.3 4.5 --eqn -8.0 || fail
./test_dbl 1.2 2.3 4.5 -D0.2 --eps -8.2 || fail

#these tests should all return non-zero (parse failed)
./test_dbl && fail
./test_dbl 1 2 3 4 && fail
./test_dbl 1 2 3 -d1 -d2 -d3 -d4 && fail
./test_dbl 1 2 3 --eps && fail
./test_dbl 1 2 3 --eps 3 --eqn 6 && fail
./test_dbl hello && fail
./test_dbl 4 hello && fail


echo "$0 TESTS PASSED"
echo "----------------------------------"

