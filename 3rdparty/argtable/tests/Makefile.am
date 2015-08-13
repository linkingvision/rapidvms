TEST_PROGS = fntests test_lit test_int test_dbl test_rex test_file
TEST_SCRIPT = fntests.sh test_lit.sh test_int.sh test_dbl.sh test_file.sh
TESTS = $(TEST_SCRIPT)

#noinst_PROGRAMS = $(TEST_PROGS)
check_PROGRAMS = $(TEST_PROGS)
dist_noinst_SCRIPTS = $(TEST_SCRIPT)

# If the system provides regex then we may compile arg_rex test
if USE_ARGREX
check_PROGRAMS += test_rex
dist_noinst_SCRIPTS += test_rex.sh
endif

# If the system provides strptime then we may compile arg_date test
if USE_ARGDATE
check_PROGRAMS += test_date
dist_noinst_SCRIPTS += test_date.sh
endif

fntests_LDADD = ../src/libargtable2.la
test_lit_LDADD = ../src/libargtable2.la
test_int_LDADD = ../src/libargtable2.la
test_dbl_LDADD = ../src/libargtable2.la
test_rex_LDADD = ../src/libargtable2.la
test_file_LDADD = ../src/libargtable2.la
test_date_LDADD = ../src/libargtable2.la

fntests_SOURCES = fntests.c
test_lit_SOURCES = test_lit.c
test_dbl_SOURCES = test_dbl.c
test_rex_SOURCES = test_rex.c
test_file_SOURCES = test_file.c
test_date_SOURCES = test_date.c
test_int_SOURCES = test_int.c

clean:
	-rm -f $(TEST_PROGS)
			
