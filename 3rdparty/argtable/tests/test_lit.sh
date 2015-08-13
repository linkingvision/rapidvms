#!/bin/sh

echo $0 TESTS BEGIN

#these tests should all return zero (parse succeeded)
./test_lit --help || exit 1
./test_lit -cdD || exit 1
./test_lit -cdDd|| exit 1
./test_lit -CDd --delta --delta|| exit 1
./test_lit --delta -cD -b || exit 1
./test_lit -D -B --delta -C || exit 1
./test_lit -D -B --delta -C || exit 1
./test_lit -D -B --delta -C --hello || exit 1
./test_lit -D -B --delta -C --world || exit 1

#these tests should all return non-zero (parse failed)

./test_lit && exit 1
./test_lit -c && exit 1
./test_lit -D && exit 1
./test_lit -CD && exit 1
./test_lit -Dd && exit 1
./test_lit -cddddd && exit 1
./test_lit -ccddd && exit 1
./test_lit -C -d -D --delta -b -B && exit 1
./test_lit -C -d -D --delta --hello --world && exit 1
./test_lit -C -d -D --delta --hello X && exit 1

echo "$0 TESTS PASSED"
echo "----------------------------------"
