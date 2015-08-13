#!/bin/sh

echo $0 TESTS BEGIN

#these tests should all return zero (parse succeeded)
./test_file --help || exit 1

./test_file       foo.bar       foo.bar   foo.bar   .bar   || exit 1
./test_file      /foo.bar      /foo.bar   foo.bar   .bar   || exit 1
./test_file     ./foo.bar     ./foo.bar   foo.bar   .bar   || exit 1
./test_file   ././foo.bar   ././foo.bar   foo.bar   .bar   || exit 1
./test_file ./././foo.bar ./././foo.bar   foo.bar   .bar   || exit 1
./test_file    ../foo.bar    ../foo.bar   foo.bar   .bar   || exit 1
./test_file ../../foo.bar ../../foo.bar   foo.bar   .bar   || exit 1

./test_file       foo           foo       foo       ''     || exit 1
./test_file      /foo          /foo       foo       ''     || exit 1
./test_file     ./foo         ./foo       foo       ''     || exit 1
./test_file   ././foo       ././foo       foo       ''     || exit 1
./test_file ./././foo     ./././foo       foo       ''     || exit 1
./test_file    ../foo        ../foo       foo       ''     || exit 1
./test_file ../../foo     ../../foo       foo       ''     || exit 1

./test_file       .foo          .foo      .foo      ''     || exit 1
./test_file      /.foo         /.foo      .foo      ''     || exit 1
./test_file     ./.foo        ./.foo      .foo      ''     || exit 1
./test_file    ../.foo       ../.foo      .foo      ''     || exit 1

./test_file       foo.          foo.      foo.      ''     || exit 1
./test_file      /foo.         /foo.      foo.      ''     || exit 1
./test_file     ./foo.        ./foo.      foo.      ''     || exit 1
./test_file    ../foo.       ../foo.      foo.      ''     || exit 1

./test_file     /.foo.        /.foo.     .foo.      ''     || exit 1
./test_file    /.foo.c       /.foo.c    .foo.c      .c     || exit 1
./test_file /.foo..b.c    /.foo..b.c .foo..b.c      .c     || exit 1

./test_file          /             /        ''      ''     || exit 1
./test_file          .             .        ''      ''     || exit 1
./test_file         ..            ..        ''      ''     || exit 1
./test_file         /.            /.        ''      ''     || exit 1
./test_file        /..           /..        ''      ''     || exit 1
./test_file         ./            ./        ''      ''     || exit 1
./test_file        ../           ../        ''      ''     || exit 1

#these tests should all return non-zero (parse failed)
./test_file && exit 1


echo "$0 TESTS PASSED"
echo "----------------------------------"
