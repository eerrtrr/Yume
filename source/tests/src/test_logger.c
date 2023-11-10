// File: tests/test_logger.c

#include <check.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "core/logger.h"

// Function to redirect stdout to a buffer
static int redirect_stdout_to_buffer(char* buffer, size_t size) {
    fflush(stdout);
    if (freopen("/dev/null", "a", stdout) == NULL) {
        return -1; // freopen failed
    }
    setbuf(stdout, buffer);
    memset(buffer, 0, size);
    return 0;
}

// Function to restore the normal stdout
static void restore_stdout(void) {
    freopen("/dev/tty", "a", stdout);
}

// A test function to ensure that the log level can be set and retrieved correctly
START_TEST(test_log_level_set) {
    set_log_level(LOG_LEVEL_WARN);
    // Assuming we have a function to get the current log level
    ck_assert_int_eq(get_current_log_level(), LOG_LEVEL_WARN);
}
END_TEST

// A test function to ensure that messages below the current log level are not logged
START_TEST(test_log_level_filtering) {
    char buffer[1024];
    if (redirect_stdout_to_buffer(buffer, sizeof(buffer)) != 0) {
        ck_abort_msg("Failed to redirect stdout to buffer");
    }

    set_log_level(LOG_LEVEL_ERROR);
    LOG_DEBUG("This should not appear");
    LOG_INFO("This should not appear");
    LOG_WARN("This should not appear");
    LOG_ERROR("This should appear");
    LOG_FATAL("This should also appear");

    restore_stdout();

    // Check that only the ERROR and FATAL messages are in the buffer
    ck_assert_str_eq(buffer, "[ERROR] This should appear\n[FATAL] This should also appear\n");
}
END_TEST
