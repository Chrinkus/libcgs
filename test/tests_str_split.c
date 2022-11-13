#include "cmocka_headers.h"
#include <stdlib.h>

#include "cgs_string.h"
#include "cgs_array.h"

static int
su_sub_arr(void** state)
{
        struct cgs_array* arr = malloc(sizeof(struct cgs_array));
        if (!arr || !cgs_array_new(arr, sizeof(struct cgs_strsub)))
                return -1;

        *state = arr;
        return 0;
}

static int
td_sub_arr(void** state)
{
        struct cgs_array* arr = *state;
        cgs_array_free(arr);
        free(arr);
        return 0;
}

static void
str_split_normal(void** state)
{
        struct cgs_array* a1 = *state;
        const char* s1 = "aaa bbb ccc ddd";

        void* res = cgs_str_split(s1, ' ', a1);
        assert_non_null(res);
        assert_int_equal(cgs_array_length(a1), 4);

        const struct cgs_strsub* ss1 = cgs_array_get(a1, 0);
        assert_int_equal(cgs_strsub_eq_str(ss1, "aaa"), 1);

        const struct cgs_strsub* ss2 = cgs_array_get(a1, 1);
        assert_int_equal(cgs_strsub_eq_str(ss2, "bbb"), 1);

        const struct cgs_strsub* ss3 = cgs_array_get(a1, 2);
        assert_int_equal(cgs_strsub_eq_str(ss3, "ccc"), 1);

        const struct cgs_strsub* ss4 = cgs_array_get(a1, 3);
        assert_int_equal(cgs_strsub_eq_str(ss4, "ddd"), 1);
}

static void
str_split_all_delims(void** state)
{
        struct cgs_array* a1 = *state;
        const char* s1 = "????????";

        void* res = cgs_str_split(s1, '?', a1);
        assert_non_null(res);
        assert_int_equal(cgs_array_length(a1), 0);
}

static void
str_split_many_delims(void** state)
{
        struct cgs_array* a1 = *state;
        const char* s1 = "###cat##dog#bird##";
        void* res = cgs_str_split(s1, '#', a1);
        assert_non_null(res);
        assert_int_equal(cgs_array_length(a1), 3);

        const struct cgs_strsub* ss1 = cgs_array_get(a1, 0);
        assert_int_equal(cgs_strsub_eq_str(ss1, "cat"), 1);

        const struct cgs_strsub* ss2 = cgs_array_get(a1, 1);
        assert_int_equal(cgs_strsub_eq_str(ss2, "dog"), 1);

        const struct cgs_strsub* ss3 = cgs_array_get(a1, 2);
        assert_int_equal(cgs_strsub_eq_str(ss3, "bird"), 1);
}

int main(void)
{
        const struct CMUnitTest tests[] = {
                cmocka_unit_test_setup_teardown(str_split_normal,
                                su_sub_arr, td_sub_arr),
                cmocka_unit_test_setup_teardown(str_split_all_delims,
                                su_sub_arr, td_sub_arr),
                cmocka_unit_test_setup_teardown(str_split_many_delims,
                                su_sub_arr, td_sub_arr),
        };

        return cmocka_run_group_tests(tests, NULL, NULL);
}
