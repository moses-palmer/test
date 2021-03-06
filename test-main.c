#include <stdarg.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

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
static char*
test_printf(const char *format, ...);

#include "test-logging.h"
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
test_main(void)
{
    int result;
    int count, failures, i;

    /* Initialise the test suite */
    debug("Running test suite " test_suite " with %u tests", test_count);
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
            int (*function)(char**) =
                test_functions[test_order[i]].function;
            char *message =
                NULL;

            info("%s - %s", name, description);

            /* Run the test */
            log_indent();
            result = function(&message);
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

/*
 * Declared in test-logging.h.
 */
int test_log_level = LL_INFO;
int test_log_indent_value = 0;

static int
main(int argc, char *argv[],
    int log_level)
{
    int result;

    printf(
        "Running test suite %s with %d tests...\n", test_suite, test_count);
    result = test_main();
    printf(
        "Test suite %s completed with %d failed test(s).\n",
        test_suite, result);

#ifndef TEST_NONINTERACTIVE
    printf("Press return to continue...");
    fgetc(stdin);
#endif

    return result;
}

#endif
