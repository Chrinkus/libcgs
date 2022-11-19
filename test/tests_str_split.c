#include "cmocka_headers.h"
#include <stdlib.h>
#include <string.h>

#include "cgs_string.h"
#include "cgs_vector.h"

static int
su_sub_vec(void** state)
{
        struct cgs_vector* vec = malloc(sizeof(struct cgs_vector));
        if (!vec || !cgs_vector_new(vec, sizeof(struct cgs_strsub)))
                return -1;

        *state = vec;
        return 0;
}

static int
td_sub_vec(void** state)
{
        struct cgs_vector* vec = *state;
        cgs_vector_free(vec);
        free(vec);
        return 0;
}

static void
str_split_normal(void** state)
{
        struct cgs_vector* a1 = *state;
        const char* s1 = "aaa bbb ccc ddd";

        void* res = cgs_str_split(s1, ' ', a1);
        assert_non_null(res);
        assert_int_equal(cgs_vector_length(a1), 4);

        const struct cgs_strsub* ss1 = cgs_vector_get(a1, 0);
        assert_int_equal(cgs_strsub_eq_str(ss1, "aaa"), 1);

        const struct cgs_strsub* ss2 = cgs_vector_get(a1, 1);
        assert_int_equal(cgs_strsub_eq_str(ss2, "bbb"), 1);

        const struct cgs_strsub* ss3 = cgs_vector_get(a1, 2);
        assert_int_equal(cgs_strsub_eq_str(ss3, "ccc"), 1);

        const struct cgs_strsub* ss4 = cgs_vector_get(a1, 3);
        assert_int_equal(cgs_strsub_eq_str(ss4, "ddd"), 1);
}

static void
str_split_all_delims(void** state)
{
        struct cgs_vector* a1 = *state;
        const char* s1 = "????????";

        void* res = cgs_str_split(s1, '?', a1);
        assert_non_null(res);
        assert_int_equal(cgs_vector_length(a1), 0);
}

static void
str_split_many_delims(void** state)
{
        struct cgs_vector* a1 = *state;
        const char* s1 = "###cat##dog#bird##";
        void* res = cgs_str_split(s1, '#', a1);
        assert_non_null(res);
        assert_int_equal(cgs_vector_length(a1), 3);

        const struct cgs_strsub* ss1 = cgs_vector_get(a1, 0);
        assert_int_equal(cgs_strsub_eq_str(ss1, "cat"), 1);

        const struct cgs_strsub* ss2 = cgs_vector_get(a1, 1);
        assert_int_equal(cgs_strsub_eq_str(ss2, "dog"), 1);

        const struct cgs_strsub* ss3 = cgs_vector_get(a1, 2);
        assert_int_equal(cgs_strsub_eq_str(ss3, "bird"), 1);
}

static void
str_split_no_delims(void** state)
{
        struct cgs_vector* a1 = *state;

        const char* s1 = "This string has no money";

        void* res = cgs_str_split(s1, '$', a1);
        assert_non_null(res);
        assert_int_equal(cgs_vector_length(a1), 1);

        const struct cgs_strsub* ss1 = cgs_vector_get(a1, 0);
        assert_int_equal(cgs_strsub_eq_str(ss1, "This string has no money"), 1);
}

static void
strsub_split_normal(void** state)
{
        struct cgs_vector* a1 = *state;
        const char* s1 = "Fedorov\tKonstantinov\tKozlov\tFetisov\tLarionov";
        struct cgs_strsub ss = cgs_strsub_from_str(s1);

        void* res = cgs_strsub_split(&ss, '\t', a1);
        assert_non_null(res);
        assert_int_equal(cgs_vector_length(a1), 5);

        const struct cgs_strsub* ss1 = cgs_vector_get(a1, 0);
        assert_int_equal(cgs_strsub_eq_str(ss1, "Fedorov"), 1);
        const struct cgs_strsub* ss2 = cgs_vector_get(a1, 1);
        assert_int_equal(cgs_strsub_eq_str(ss2, "Konstantinov"), 1);
        const struct cgs_strsub* ss3 = cgs_vector_get(a1, 2);
        assert_int_equal(cgs_strsub_eq_str(ss3, "Kozlov"), 1);
        const struct cgs_strsub* ss4 = cgs_vector_get(a1, 3);
        assert_int_equal(cgs_strsub_eq_str(ss4, "Fetisov"), 1);
        const struct cgs_strsub* ss5 = cgs_vector_get(a1, 4);
        assert_int_equal(cgs_strsub_eq_str(ss5, "Larionov"), 1);
}

int main(void)
{
        const struct CMUnitTest tests[] = {
                // str_split
                cmocka_unit_test_setup_teardown(str_split_normal,
                                su_sub_vec, td_sub_vec),
                cmocka_unit_test_setup_teardown(str_split_all_delims,
                                su_sub_vec, td_sub_vec),
                cmocka_unit_test_setup_teardown(str_split_many_delims,
                                su_sub_vec, td_sub_vec),
                cmocka_unit_test_setup_teardown(str_split_no_delims,
                                su_sub_vec, td_sub_vec),
                // strsub_split
                cmocka_unit_test_setup_teardown(strsub_split_normal,
                                su_sub_vec, td_sub_vec),
        };

        return cmocka_run_group_tests(tests, NULL, NULL);
}
