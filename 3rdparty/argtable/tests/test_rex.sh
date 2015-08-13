#!/bin/sh

fail ()
    { 
    echo "$0 TESTS FAILED";
    exit 1;
    }

echo $0 TESTS BEGIN

#these tests should all return zero (parse succeeded)
./test_rex --help || fail
./test_rex --beta world goodbye || fail
./test_rex GoodBye --beta World || fail
./test_rex --beta world GOODBYE GoodBye Goodbye gOoDbyE Anything || fail
./test_rex --beta world GOODBYE AnyHow || fail
./test_rex -a hello --beta world GOODBYE AnyHow || fail

#these tests should all return non-zero (parse failed)
./test_rex && fail
./test_rex --beta WORLD goodbye && fail
./test_rex --beta World goodby  && fail
./test_rex --beta world GOODBYE GoodBye Goodbye gOoDbyE Anything goodbye && fail
./test_rex --beta world GOODBYE GoodBye Goodbye gOoDbyE Anything Anytime && fail
./test_rex --beta world GOODBYE GoodBye Goodbye gOoDbyE Anything -a Hello && fail

echo "$0 TESTS PASSED"
echo "----------------------------------"

