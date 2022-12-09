#include "cmocka_headers.h"

#include "cgs_string.h"

#include <stdlib.h>             // free

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
strsub_from_str(void** state)
{
        (void)state;

        const char* s1 = "coffee";
        struct cgs_strsub ss1 = cgs_strsub_from_str(s1);
        assert_int_equal(ss1.length, 6);
        assert_memory_equal(ss1.data, "coffee", 6);
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

static void
strsub_to_int(void** state)
{
        (void)state;
        const char* p1 = "531";
        struct cgs_strsub ss1 = cgs_strsub_new(p1, 3);
        struct cgs_strsub ss2 = cgs_strsub_new(p1, 2);

        int n = 0;
        assert_non_null(cgs_strsub_to_int(&ss1, &n));
        assert_int_equal(n, 531);

        assert_non_null(cgs_strsub_to_int(&ss2, &n));
        assert_int_equal(n, 53);

        const char* p2 = "-298310";
        struct cgs_strsub ss3 = cgs_strsub_new(p2, 7);
        assert_non_null(cgs_strsub_to_int(&ss3, &n));
        assert_int_equal(n, -298310);

        const char* p3 = "Not a number";
        struct cgs_strsub ss4 = cgs_strsub_new(p3, 12);
        assert_null(cgs_strsub_to_int(&ss4, &n));

        const char* p4 = "";
        struct cgs_strsub ss5 = cgs_strsub_new(p4, 0);
        assert_null(cgs_strsub_to_int(&ss5, &n));

        const char* p5 = "+-76";
        struct cgs_strsub ss6 = cgs_strsub_new(p5, 4);
        assert_null(cgs_strsub_to_int(&ss6, &n));

        const char* p6 = "+9000";
        struct cgs_strsub ss7 = cgs_strsub_new(p6, 5);
        assert_non_null(cgs_strsub_to_int(&ss7, &n));
        assert_int_equal(n, 9000);
}

static void
strsub_to_str(void** state)
{
        (void)state;

        const char* s1 = "Hello World";
        const char* s2 = s1 + 6;

        struct cgs_strsub ss1 = cgs_strsub_new(s1, 5);
        struct cgs_strsub ss2 = cgs_strsub_new(s2, 5);

        char* p1 = cgs_strsub_to_str(&ss1);
        assert_non_null(p1);
        assert_string_equal(p1, "Hello");

        char* p2 = cgs_strsub_to_str(&ss2);
        assert_non_null(p2);
        assert_string_equal(p2, "World");

        struct cgs_strsub ss3 = cgs_strsub_new(s1, 3);
        struct cgs_strsub ss4 = cgs_strsub_new(s2, 2);

        char* p3 = cgs_strsub_to_str(&ss3);
        assert_non_null(p3);
        assert_string_equal(p3, "Hel");

        char* p4 = cgs_strsub_to_str(&ss4);
        assert_non_null(p4);
        assert_string_equal(p4, "Wo");

        free(p1);
        free(p2);
        free(p3);
        free(p4);
}

static void
strsub_to_string(void** state)
{
        (void)state;

        const char* p = "aaa bbb ccc ddd";
        void* res = NULL;

        struct cgs_strsub ss1 = cgs_strsub_new(p, 3);
        struct cgs_string s1 = { 0 };
        res = cgs_strsub_to_string(&ss1, &s1);
        assert_non_null(res);
        assert_string_equal(cgs_string_data(&s1), "aaa");

        struct cgs_strsub ss2 = cgs_strsub_new(p+4, 3);
        struct cgs_string s2 = { 0 };
        res = cgs_strsub_to_string(&ss2, &s2);
        assert_non_null(res);
        assert_string_equal(cgs_string_data(&s2), "bbb");

        cgs_string_free(&s1);
        cgs_string_free(&s2);
}

int main(void)
{
        const struct CMUnitTest tests[] = {
                cmocka_unit_test(strsub_new),
                cmocka_unit_test(strsub_from_str),
                cmocka_unit_test(strsub_cmp),
                cmocka_unit_test(strsub_to_int),
                cmocka_unit_test(strsub_to_str),
                cmocka_unit_test(strsub_to_string),
        };

        return cmocka_run_group_tests(tests, NULL, NULL);
}
