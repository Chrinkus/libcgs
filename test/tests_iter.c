#include "cmocka_headers.h"

#include <stdlib.h>     // malloc, free

#include "cgs_iter.h"
#include "cgs_vector.h"

static int
setup_vec(void** state)
{
        struct cgs_vector* v = malloc(sizeof(struct cgs_vector));

        int ai[] = { 9, 22, 4, 13, 8, };
        cgs_vector_from_array(ai, 5, sizeof(int), v);

        *state = v;
        return 0;
}

static int
teardown_vec(void** state)
{
        struct cgs_vector* v = *state;
        cgs_vector_free(v);
        free(v);
        return 0;
}

static void
iter_vec_test(void** state)
{
        struct cgs_vector* v1 = *state;

        struct cgs_iter it1 = cgs_vector_iter(v1);
        const int* pi = NULL;

        pi = cgs_iter_next(&it1);
        assert_non_null(pi);
        assert_int_equal(*pi, 9);

        pi = cgs_iter_next(&it1);
        assert_non_null(pi);
        assert_int_equal(*pi, 22);

        pi = cgs_iter_next(&it1);
        assert_non_null(pi);
        assert_int_equal(*pi, 4);

        pi = cgs_iter_next(&it1);
        assert_non_null(pi);
        assert_int_equal(*pi, 13);

        pi = cgs_iter_next(&it1);
        assert_non_null(pi);
        assert_int_equal(*pi, 8);

        pi = cgs_iter_next(&it1);
        assert_null(pi);

        int sum = 0;
        struct cgs_iter it2 = cgs_vector_iter(v1);
        for (const int* p = NULL; (p = cgs_iter_next(&it2)); ) {
                sum += *p;
        }
        assert_int_equal(sum, 56);
}

static void
iter_vec_mut_test(void** state)
{
        (void)state;
        struct cgs_vector v1 = cgs_vector_new(sizeof(int));
        for (int i = 1; i <= 10; ++i)
                cgs_vector_push(&v1, &i);

        struct cgs_iter it1 = cgs_vector_iter_mut(&v1);
        for (int* p = NULL; (p = cgs_iter_mut_next(&it1)); )
                *p *= 2;

        const int* p1 = cgs_vector_get(&v1, 0);
        assert_int_equal(*p1, 2);
        p1 = cgs_vector_get(&v1, 9);
        assert_int_equal(*p1, 20);

        cgs_vector_free(&v1);
}

int main(void)
{
        const struct CMUnitTest tests[] = {
                cmocka_unit_test_setup_teardown(iter_vec_test,
                                setup_vec, teardown_vec),
                cmocka_unit_test(iter_vec_mut_test),
                                
        };

        return cmocka_run_group_tests(tests, NULL, NULL);
}
