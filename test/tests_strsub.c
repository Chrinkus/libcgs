#include "cmocka_headers.h"

#include "cgs_string.h"

static void
strsub_new(void** state)
{
        (void)state;

        const char* s1 = "hello";
        struct cgs_strsub ss1 = cgs_strsub_new(s1, 4);
        assert_int_equal(ss1.length, 4);
        assert_memory_equal(ss1.data, "hell", 4);

        const char s2[] = "world";
        struct cgs_strsub ss2 = cgs_strsub_new(s2, 2);
        assert_int_equal(ss2.length, 2);
        assert_memory_equal(ss2.data, "wo", 2);

        struct cgs_strsub ss3 = cgs_strsub_new("subulous", 6);
        assert_int_equal(ss3.length, 6);
        assert_memory_equal(ss3.data, "subulo", 6);
}

static void
strsub_cmp(void** state)
{
        (void)state;

        // same length tests
        const char* p1 = "hello";
        const char* p2 = "help";
        const char* p3 = "helicopter";
        const char* p4 = "hell";

        struct cgs_strsub ss1 = cgs_strsub_new(p1, 4);
        struct cgs_strsub ss2 = cgs_strsub_new(p2, 4);
        struct cgs_strsub ss3 = cgs_strsub_new(p3, 4);
        struct cgs_strsub ss4 = cgs_strsub_new(p4, 4);

        assert_true(cgs_strsub_cmp(&ss1, &ss2) < 0);    // hell < help
        assert_true(cgs_strsub_cmp(&ss1, &ss3) > 0);    // hell > heli
        assert_true(cgs_strsub_cmp(&ss1, &ss4) == 0);   // hell == hell

        // different length tests
        const char* p5 = "world";

        struct cgs_strsub ss5 = cgs_strsub_new(p5, 5);
        struct cgs_strsub ss6 = cgs_strsub_new(p5, 3);

        assert_true(cgs_strsub_cmp(&ss5, &ss6) > 0);
        assert_true(cgs_strsub_cmp(&ss6, &ss5) < 0);
}

int main(void)
{
        const struct CMUnitTest tests[] = {
                cmocka_unit_test(strsub_new),
                cmocka_unit_test(strsub_cmp),
        };

        return cmocka_run_group_tests(tests, NULL, NULL);
}
