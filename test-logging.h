#ifndef TEST_LOGGING_H
#define TEST_LOGGING_H

enum {
    TLL_NONE,
    TLL_ERROR,
    TLL_INFO,
    TLL_DEBUG,
    TLL_TRACE
};

/**
 * The current log level.
 */
extern int test_log_level;

/**
 * The current indentation of the log messages.
 */
extern int test_log_indent_value;

#define log_indent() \
    test_log_indent_value++
#define log_unindent() \
    test_log_indent_value--

/**
 * Prints a log message if the current log level is at least level.
 *
 * @param level
 *     The minimum required log level for this message to be written.
 * @param ...
 *     Parameters passed on to printf.
 */
#define log(level, ...) \
    if (test_log_level >= (level)) do { \
        int _i = test_log_indent_value; \
        while (_i--) putc('\t', stdout); \
        \
        printf(__VA_ARGS__); \
        printf("\n"); \
        fflush(stdout); \
    } while(0)

/**
 * Prints a log message if the current log level is at least LL_ERROR.
 *
 * @param ...
 *     The format string followed by any parameters.
 * @see log
 */
#define error(...) \
    log(TLL_ERROR, __VA_ARGS__)

/**
 * Prints a log message if the current log level is at least LL_INFO.
 *
 * @param ...
 *     The format string followed by any parameters.
 * @see log
 */
#define info(...) \
    log(TLL_INFO, __VA_ARGS__)

/**
 * Prints a log message if the current log level is at least LL_DEBUG.
 *
 * @param ...
 *     The format string followed by any parameters.
 * @see log
 */
#define debug(...) \
    log(TLL_DEBUG, __VA_ARGS__)

/**
 * Prints a log message if the current log level is at least LL_TRACE.
 *
 * @param ...
 *     The format string followed by any parameters.
 * @see log
 */
#define trace(...) \
    log(TLL_TRACE, __VA_ARGS__)

#endif

