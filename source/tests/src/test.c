#include <check.h>
#include "test_logger.c"

START_TEST(test_setup) {
    ck_assert(1);
}
END_TEST

Suite *main_suite(void) {
    Suite *tests = suite_create("Tests");

    TCase *tc_setup = tcase_create("Setup");
    TCase *tc_logger = tcase_create("Logger");


    /* Core test case */
    tcase_add_test(tc_setup, test_setup);
    suite_add_tcase(tests, tc_setup);

    tcase_add_test(tc_logger, test_log_level_set);
    tcase_add_test(tc_logger, test_log_level_filtering);
    suite_add_tcase(tests, tc_logger);

    return tests;
}

int main(void) {
    int number_failed;
    SRunner *sr;

    sr = srunner_create(main_suite());
    srunner_run_all(sr, CK_NORMAL);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? 0 : 1;
}
