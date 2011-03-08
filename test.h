#ifndef TEST_H
#define TEST_H

#include "test-logging.h"
#include "test-assert.h"

/**
 * Converts an expression to a string.
 */
#define TEST_S(n) \
    #n
#define TEST_SS(n) \
    TEST_S(n)

/*
 * Include tests.def to make sure that all symbols declared in its include
 * guarded section are available.
 */
#undef TEST
#define TEST(name, description, locals, test, teardown)
#include "../tests.def"

/*
 * Make sure that the test suite name is defined.
 */
#ifndef TEST_SUITE
    #error You must define a test suite name by defining TEST_SUITE
#endif

/**
 * The string version of the test suite name.
 */
#define test_suite TEST_SS(TEST_SUITE)

/**
 * The name of the test suite function.
 */
#define test_main TEST_SUITE##_suite_run

/**
 * The number of times to rerun the test suite.
 *
 * The order of the tests is shuffled after every iteration has completed. By
 * defining TEST_ITERATIONS, you may verify that no tests depend on side effect
 * of other tests.
 */
#ifndef TEST_ITERATIONS
    #define TEST_ITERATIONS 1
#endif


/**
 * A function that is called before the test suite is run.
 *
 * Unless you define TEST_NO_SETUP, you must implement this function.
 *
 * @return 0 if the setup succeded, or an error code otherwise; the test suite
 *     is not run if an error code is returned
 */
static int
test_setup(void)
#ifdef TEST_NO_SETUP
{
    return 0;
}
#else
;
#endif

/**
 * A function that is called after the test suite is run.
 *
 * Unless you define TEST_NO_TEARDOWN, you must implement this function.
 */
static void
test_teardown(void)
#ifdef TEST_NO_TEARDOWN
{
}
#else
;
#endif

/**
 * The function that runs all tests in the test suite.
 *
 * Please note that test_main is only a placeholder name; the actual name of
 * this function will be the value of the test_main macro.
 *
 * @param log_level
 *     The log level to use.
 * @return the number of tests that failed
 * @see log
 * @see test_main
 */
int
test_main(int log_level);

#endif
