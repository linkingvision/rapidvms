#!/bin/sh

fail ()
    { 
    echo "$0 TESTS FAILED";
    exit 1;
    }

echo $0 TESTS BEGIN

#these tests should all return zero (parse succeeded)
./test_date --help || fail
./test_date 23:59 --date 12/31/04 || fail
./test_date --date 12/31/04 20:15 || fail
./test_date --date 12/31/04 20:15 --date 06/07/84 || fail
./test_date --date 12/31/04 20:15 -b 1982-11-28 --date 06/07/84 || fail

#these tests should all return non-zero (parse failed)
./test_date && fail
./test_date 25:59 --date 12/31/04 && fail
./test_date 23:59 --date 12/32/04 && fail
./test_date 23:59 --date 12/31/04 22:58 && fail
./test_date --date 12/31/04 20:15 --date 26/07/84 && fail
./test_date --date 12/31/04 20:15 -b 1982-11-28 -b 1982-11-28 --date 06/07/84 && fail

echo "$0 TESTS PASSED"
echo "----------------------------------"

