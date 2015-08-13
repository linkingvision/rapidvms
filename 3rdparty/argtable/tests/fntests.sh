#!/bin/sh

fail ()
    { 
    echo "$0 TESTS FAILED";
    exit 1;
    }

echo $0 TESTS BEGIN
./fntests || fail

echo "$0 TESTS PASSED"
echo "----------------------------------"

