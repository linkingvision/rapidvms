
#ifndef ut_framework_h
#define ut_framework_h

#include <list>
#include <exception>
#include <string>
#include <stdio.h>
#include <string.h>

#ifdef WIN32
#define SNPRINTF _snprintf_s
#else
#define SNPRINTF snprintf
#endif

void ut_usleep(unsigned int usec);

/// Normally, you will use TEST_SUITE like this:
///
/// TEST_SUITE(MyTestFixture);
///     TEST(MyTestFixture::TestFoo);
///     TEST(MyTestFixture::TestBar);
/// TEST_SUITE_END();
///
/// But if your fixture has its own member variables that you really need to initialize in its constructor
/// you can do so like this (note the slightly different starting macro, and the presence of TEST_SUITE_BEGIN()).
///
/// TEST_SUITE_INIT(MyTestFixture)
///     _lok(),
///     _cond(_lok)
/// TEST_SUITE_BEGIN()
///     TEST(MyTestFixture::TestFoo);
///     TEST(MyTestFixture::TestBar);
/// TEST_SUITE_END()

#define TEST_SUITE(a) \
        a() : test_fixture(#a) {

#define TEST_SUITE_INIT(a) a() : test_fixture(#a),
#define TEST_SUITE_BEGIN() {

#define TEST(a) \
        add_test((void (test_fixture::*)())&a,#a)

#define TEST_SUITE_END() }

class test_fixture;

struct test_container
{
    test_container() :
        fixture(NULL),
        test(NULL),
        test_name(),
        exception_msg(),
        passed(false)
    {
    }

    test_fixture* fixture;
    void (test_fixture::*test)();
    std::string test_name;
    char exception_msg[4096];
    bool passed;
};

class ut_fail_exception : public std::exception
{
public:
    ut_fail_exception()
        : std::exception(),
          _line_num( 0 ),
          _src_file(),
          _what_msg(),
          _msg()
    {
    }

    virtual ~ut_fail_exception() throw()
    {
    }

    virtual const char* what() const throw()
    {
        return _what_msg.c_str();
    }

    const char* get_msg() const
    {
        return _msg.c_str();
    }

    void set_msg(std::string msg)
    {
        _msg = msg;

        char line_num_msg[36];
        SNPRINTF(line_num_msg,36,"%d",_line_num);

        _what_msg = _msg + " in file " + _src_file + " at line " + line_num_msg;
    }

    void set_throw_point(int line, const char* file)
    {
        _line_num = line;
        _src_file = file;

        char line_num_msg[36];
        SNPRINTF(line_num_msg,36,"%d",_line_num);

        _what_msg = _msg + " in file " + _src_file + " at line " + line_num_msg;
    }

    int get_line_num() const
    {
        return _line_num;
    }

    const char* get_src_file() const
    {
        return _src_file.c_str();
    }

private:
    int _line_num;
    std::string _src_file;
    mutable std::string _what_msg;
    std::string _msg;
};

#define UT_ASSERT(a) do{ if( !(a) ) { ut_fail_exception e; e.set_msg(#a); e.set_throw_point(__LINE__,__FILE__); throw e; } } while(false)

#define UT_ASSERT_EQUAL(a,b) do{ if( !(a==b) ) { ut_fail_exception e; e.set_msg(#a); e.set_throw_point(__LINE__,__FILE__); throw e; } } while(false)

#define UT_ASSERT_EQUAL_MESSAGE(message,a,b) do{ if( !(a==b) ) { ut_fail_exception e; e.set_msg(message); e.set_throw_point(__LINE__,__FILE__); throw e; } } while(false)

#define UT_ASSERT_MESSAGE(message,a) do{ if( !(a) ) { ut_fail_exception e; e.set_msg(message); e.set_throw_point(__LINE__,__FILE__); throw e; } } while(false)

#define UT_ASSERT_THROWS(thing_that_throws,what_is_thrown) do { try { bool threw = false; try { thing_that_throws; } catch( what_is_thrown& ) { threw=true; } if(!threw) throw false; } catch(...) {ut_fail_exception e; e.set_msg("Test failed to throw expected exception."); e.set_throw_point(__LINE__,__FILE__); throw e;} } while(false)

#define UT_ASSERT_THROWS_MESSAGE(message,thing_that_throws,what_is_thrown) do { try { bool threw = false; try { thing_that_throws; } catch( what_is_thrown& ) { threw=true; } if(!threw) throw false; } catch(...) {ut_fail_exception e; e.set_msg(message); e.set_throw_point(__LINE__,__FILE__); throw e;} } while(false)

#define UT_ASSERT_NO_THROW(thing_that_doesnt_throw) do { { bool threw = false; try { thing_that_doesnt_throw; } catch( ... ) { threw=true; } if(threw) {ut_fail_exception e; e.set_msg("Test threw unexpected exception."); e.set_throw_point(__LINE__,__FILE__); throw e;} } } while(false)

class test_fixture
{
public:
    test_fixture(std::string fixture_name)
        : _tests(),
          _something_failed( false ),
          _fixture_name( fixture_name )
    {
    }

    virtual ~test_fixture() throw()
    {
    }

    void run_tests()
    {
        std::list<struct test_container>::iterator i = _tests.begin();
        for( ; i != _tests.end(); i++ )
        {
            setup();

            try
            {
                printf("%-50s [",(*i).test_name.c_str());
                ((*i).fixture->*(*i).test)();
                (*i).passed = true;
                printf("P]\n");
            }
            catch(std::exception& ex)
            {
                _something_failed = true;
                (*i).passed = false;
                size_t msgLen = strlen( ex.what() );
                memset( &(*i).exception_msg, 0, 4096 );
                memcpy( &(*i).exception_msg, ex.what(), (msgLen<4096) ? msgLen : 4096 );
                printf("F]\n");
            }
            catch(...)
            {
                _something_failed = true;
                (*i).passed = false;
                printf("F]\n");
            }

            teardown();
        }
    }

    bool something_failed()
    {
        return _something_failed;
    }

    void print_failures()
    {
        std::list<struct test_container>::iterator i = _tests.begin();
        for( ; i != _tests.end(); i++ )
        {
            if(!(*i).passed)
            {
                printf("\nUT_FAIL: %s failed with exception: %s\n",
                       (*i).test_name.c_str(),
                       (*i).exception_msg);
            }
        }
    }

    std::string get_name() const { return _fixture_name; }

protected:
    virtual void setup() {}
    virtual void teardown() {}
    void add_test( void (test_fixture::*test)(), std::string name )
    {
        struct test_container tc;
        tc.test = test;
        tc.test_name = name;
        tc.fixture = this;
        _tests.push_back( tc );
    }

    std::list<struct test_container> _tests;
    bool _something_failed;
    std::string _fixture_name;
};

extern std::list<test_fixture*> _test_fixtures;

#define REGISTER_TEST_FIXTURE(a) \
class a##_static_init \
{ \
public: \
    a##_static_init() { \
        _test_fixtures.push_back(new a()); \
    } \
}; \
a##_static_init a##_static_init_instance;

// This is a globally (across test) incrementing counter so that tests can avoid having hardcoded port
// numbers but can avoid stepping on eachothers ports.
extern int _next_port;

int ut_next_port();

#define UT_NEXT_PORT() ut_next_port()

#endif
