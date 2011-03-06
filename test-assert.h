#ifndef TEST_ASSERT_H
#define TEST_ASSERT_H

/**
 * Constants used as return values for the test functions.
 */
enum {
    /** The test passed */
    TR_PASS,

    /** The test failed */
    TR_FAIL,

    /** The test could not be run, and the entire test suite should be
        aborted */
    TR_ABORT
};

/**
 * Allocates a string and prints format with the variable argument list into it.
 *
 * The caller must free the returned string with free.
 *
 * @param format
 *     The print format to use. This is the same format as used by printf.
 * @param ...
 *     Any extra parameters to printf.
 * @return a newly allocated string or NULL if there is not enough memory
 * @see printf
 */
char*
test_printf(const char *format, ...);

/**
 * Asserts that an expression is true. If the expression is false, the test
 * fails and the error message specified is printed.
 *
 * @param expression
 *     The expression to evaluate. It is only evaluated once.
 * @param format
 *     The format string used for the error message. This is the same format as
 *     used by printf.
 * @param ...
 *     Any extra parameters to printf.
 */
#define assert_true(expression, format, ...) \
    if (!(expression)) { \
        *message = test_printf(format, __VA_ARGS__); \
        internal.result = TR_FAIL; \
        trace(format, __VA_ARGS__); \
        break; \
    } \
    else do {} while(0)

/**
 * Asserts that that an expression is true. The message is based on the
 * expression.
 *
 * @param expression
 *     The expression to evaluate. It is only evaluated once.
 */
#define assert(expression) \
    assert_true(expression, "Assertion %s failed", #expression)

/**
 * Asserts that two strings are equal.
 *
 * @param s1, s2
 *     The strings to compare.
 */
#define assert_streq(s1, s2) \
    assert_true(strcmp(s1, s2) == 0, "%s is not %s", s1, s2)

/**
 * Asserts that two wide strings are equal.
 *
 * @param s1, s2
 *     The strings to compare.
 */
#define assert_wcseq(s1, s2) \
    assert_true(wcscmp(s1, s2) == 0, "%ls is not %ls", s1, s2)


/**
 * Fails the entire test suite.
 *
 * This may be called both during setup and test run.
 */
#define abort() \
    internal.result = TR_ABORT; \
    break

#endif
