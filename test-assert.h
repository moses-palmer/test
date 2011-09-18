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
 * Asserts that an expression is true. If the expression is false, the test
 * fails and the error message specified is printed.
 *
 * @param expression
 *     The expression to evaluate. It is only evaluated once.
 * @param ...
 *     Parameters to pass to printf.
 */
#define assert_true(expression, ...) \
    if (!(expression)) { \
        *message = test_printf(__VA_ARGS__); \
        internal.result = TR_FAIL; \
        debug(__VA_ARGS__); \
        goto end; \
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
 * Asserts that two expression evaluate to the same value.
 *
 * The expressions must evaluate to a value that is typecastable to
 * long long int.
 *
 * @param e1, e2
 *     The expressions to compare.
 */
#define assert_eq(e1, e2) \
    do { \
        long long int v1 = (long long int)e1; \
        long long int v2 = (long long int)e2; \
        assert_true(v1 == v2, "%s is not %s", #e1, #e2); \
    } while (0)

/**
 * Asserts that two strings are equal.
 *
 * @param s1, s2
 *     The strings to compare.
 */
#define assert_streq(s1, s2) \
    assert(s1 != NULL); \
    assert(s2 != NULL); \
    assert_true(strcmp(s1, s2) == 0, "%s is not %s", s1, s2)

/**
 * Asserts that two wide strings are equal.
 *
 * @param s1, s2
 *     The strings to compare.
 */
#define assert_wcseq(s1, s2) \
    assert(s1 != NULL); \
    assert(s2 != NULL); \
    assert_true(wcscmp(s1, s2) == 0, "%ls is not %ls", s1, s2)


/**
 * Passes the current test without reaching the end.
 *
 * @param ...
 *     Parameters to pass to printf.
 */
#define pass(...) \
    debug(__VA_ARGS__); \
    goto end

/**
 * Fails the current test unconditionally.
 *
 * @param ...
 *     Parameters to pass to printf.
 */
#define fail(...) \
    assert_true(0, __VA_ARGS__)

/**
 * Fails the entire test suite.
 */
#define abort() \
    debug("Aborting test suite"); \
    internal.result = TR_ABORT; \
    goto end

#endif
