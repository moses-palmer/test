#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * The current indentation of the log messages.
 */
static int log_indent_value = 0;

#include "test-private.h"

/**
 * The order in which to run the tests.
 */
static int test_order[test_count];

/**
 * Randomises the test function table.
 */
static void
test_randomize(void)
{
    int i;

    for (i = 0; i < test_count; i++) {
        int s, d, t;

        s = rand() % test_count;
        d = rand() % test_count;

        t = test_order[d];
        test_order[d] = test_order[s];
        test_order[s] = t;
    }
}

static char*
test_printf(const char *format, ...)
{
    size_t size = 64;
    char *p;
    va_list ap;

    if ((p = malloc (size)) == NULL) {
        return NULL;
    }

    while (1) {
        int n;
        char *np;

        /* Try to print in the allocated space. */
        va_start(ap, format);
        n = vsnprintf(p, size, format, ap);
        va_end(ap);

        /* If that worked, return the string. */
        if (n > -1 && n < (int)size) {
            return p;
        }

        /* Otherwise try again with more space. */
        size *= 2;
        if ((np = realloc (p, size)) == NULL) {
            free(p);
            return NULL;
        }
        else {
            p = np;
        }
    }
}

int
test_main(int log_level)
{
    int result;
    int count, failures, i;

    /* Initialise the test suite */
    debug("Running test suite " test_suite " with %d tests", test_count);
    result = test_setup();
    if (result) {
        if (result == -1) {
            debug("Test suite " test_suite " will be skipped");
            return 0;
        }
        else {
            debug("Test suite " test_suite " failed to setup: %d", result);
            return result;
        }
    }

    /* Initialise the test order */
    for (i = 0; i < test_count; i++) {
        test_order[i] = i;
    }

    /* Initialise the failure count to the total number of tests we are going to
       perform */
    count = TEST_ITERATIONS;
    failures = count * test_count;

    log_indent();
    do {
        for (i = 0; i < test_count; i++) {
            const char *name =
                test_functions[test_order[i]].name;
            const char *description =
                test_functions[test_order[i]].description;
            int (*function)(char**, int) =
                test_functions[test_order[i]].function;
            char *message =
                NULL;

            info("%s - %s", name, description);

            /* Run the test */
            log_indent();
            result = function(&message, log_level);
            log_unindent();

            switch (result) {
            case TR_PASS:
                /* The test passed */
                trace("%s: Test passed", name);
                failures--;
                break;

            case TR_FAIL:
            case TR_ABORT:
                /* The test failed; isf the result is TR_ABORT, no more tests
                   will be run */
                error("%d: %s: Test failed: %s", test_order[i], name,
                    message ? message : "no error specified");
                break;

            default:
                error("%s: Unknown error code: %d (%s)", name,
                     result, message ? message : "no error specified");
                break;
            }

            /* Free the message buffer if it has been allocated */
            if (message) {
                free(message);
            }

            /* Cancel the test suite if a fatal error has occurred */
            if (result == TR_ABORT) {
                debug("Test suite aborted");
                break;
            }
        }

        test_randomize();
    } while (--count && (result != TR_ABORT));
    log_unindent();

    test_teardown();

    return failures;
}

#ifdef TEST_RUN

#define ARGUMENTS_NO_SETUP
#define ARGUMENTS_NO_TEARDOWN
#include "arguments/arguments.h"

static int
main(int argc, char *argv[],
    int log_level)
{
    return test_main(log_level);
}

#endif
