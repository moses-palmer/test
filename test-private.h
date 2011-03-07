#ifndef TEST_PRIVATE_H
#define TEST_PRIVATE_H

#include <string.h>

#include "test.h"

/**
 * Expands to the name of a test function.
 *
 * @param name
 *     The name of the test.
 */
#define test_function(name) \
    name##_test


/******************************************************************************/
/* Forward declare all test functions and define the test function table.     */
/******************************************************************************/
#undef TEST
#define TEST(name, description, locals, test, teardown) \
    static int \
    test_function(name)(char **message, int log_level);

typedef struct {
    /**
     * The name of the test.
     */
    const char *name;

    /**
     * The description of the test.
     */
    const char *description;

    /**
     * The test function.
     *
     * @param message
     *     The error message if the result is not zero. If this is not NULL, it
     *     will be freed by the caller using free().
     * @param log_level
     *     The current log level.
     * @return 0 or an error code
     */
    int (*function)(char **message, int log_level);
} TestFunction;

#include "tests.def"
#undef TEST
#define TEST(name, description, locals, test, teardown) \
    {#name, description, test_function(name)},
static TestFunction test_functions[] = {
    #include "tests.def"
    {NULL, NULL, NULL}
};

#define test_count (sizeof(test_functions) / sizeof(test_functions[0]) - 1)


/******************************************************************************/
/* Define the test functions.                                                 */
/******************************************************************************/
#undef TEST
#define TEST(name, description, locals, test, teardown) \
    static int \
    test_function(name)(char **message, int log_level) \
    { \
        struct { \
            int result; \
        } internal; \
        locals \
        \
        internal.result = TR_PASS; \
        \
        *message = NULL; \
        \
        trace("Running test"); \
        log_indent(); \
        do { \
            test \
        } while (0); \
        log_unindent(); \
        \
        trace("Running teardown"); \
        log_indent(); \
        do { \
            teardown \
        } while (0); \
        log_unindent(); \
        \
        return internal.result; \
    }
#include "tests.def"

#endif
