#include <check.h>

START_TEST(test_example) {
    ck_assert(1);
}
END_TEST

Suite *main_suite(void) {
    Suite *s;
    TCase *tc_core;

    s = suite_create("Main");

    /* Core test case */
    tc_core = tcase_create("Core");

    tcase_add_test(tc_core, test_example);
    suite_add_tcase(s, tc_core);

    return s;
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
