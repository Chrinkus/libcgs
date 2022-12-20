#include "cmocka_headers.h"

#include "cgs_heap.h"
#include "cgs_compare.h"

const int ai[] = { 4, 1, 3, 2, 16, 9, 10, 14, 8, 7 };
const int len = sizeof(ai) / sizeof(ai[0]);

static void
heap_new_test(void** state)
{
        (void)state;

        struct cgs_heap h1 = cgs_heap_new(sizeof(int), cgs_int_cmp);

        assert_null(h1.data);
        assert_int_equal(h1.length, 0);
        assert_int_equal(h1.capacity, 0);

        cgs_heap_free(&h1);
}

static void heap_push_test(void** state)
{
        (void)state;

        struct cgs_heap h = cgs_heap_new(sizeof(int), cgs_int_cmp);

        const int* peek = cgs_heap_peek(&h);
        assert_null(peek);

        int n = 16;
        const void* res = cgs_heap_push(&h, &n);
        assert_non_null(res);

        peek = cgs_heap_peek(&h);
        assert_non_null(peek);
        assert_int_equal(*peek, 16);

        n = 20;
        res = cgs_heap_push(&h, &n);
        assert_non_null(res);

        peek = cgs_heap_peek(&h);
        assert_non_null(peek);
        assert_int_equal(*peek, 16);

        n = 5;
        res = cgs_heap_push(&h, &n);
        assert_non_null(res);

        peek = cgs_heap_peek(&h);
        assert_non_null(peek);
        assert_int_equal(*peek, 5);

        n = -3;
        cgs_heap_push(&h, &n);
        peek = cgs_heap_peek(&h);
        assert_int_equal(*peek, -3);

        n = 0;
        cgs_heap_push(&h, &n);
        peek = cgs_heap_peek(&h);
        assert_int_equal(*peek, -3);

        cgs_heap_free(&h);
}

static void heap_growth_test(void** state)
{
        (void)state;

        struct cgs_heap h = cgs_heap_new(sizeof(int), cgs_int_cmp);

        assert_int_equal(cgs_heap_length(&h), 0);
        assert_int_equal(h.capacity, 0);

        for (int i = 0; i < len; ++i)
                assert_non_null(cgs_heap_push(&h, &ai[i]));

        assert_int_equal(cgs_heap_length(&h), 10);
        assert_int_equal(h.capacity, 15);

        cgs_heap_free(&h);
}

static void heap_pop_test(void** state)
{
        (void)state;

        struct cgs_heap h = cgs_heap_new(sizeof(int), cgs_int_cmp);

        for (int i = 0; i < len; ++i)
                cgs_heap_push(&h, &ai[i]);

        assert_int_equal(len, 10);
        assert_int_equal(cgs_heap_length(&h), 10);
        int n = 0;

        assert_non_null(cgs_heap_pop(&h, &n));
        assert_int_equal(cgs_heap_length(&h), 9);
        assert_int_equal(n, 1);

        assert_non_null(cgs_heap_pop(&h, &n));
        assert_int_equal(cgs_heap_length(&h), 8);
        assert_int_equal(n, 2);

        assert_non_null(cgs_heap_pop(&h, &n));
        assert_int_equal(cgs_heap_length(&h), 7);
        assert_int_equal(n, 3);

        assert_non_null(cgs_heap_pop(&h, &n));
        assert_int_equal(cgs_heap_length(&h), 6);
        assert_int_equal(n, 4);

        assert_non_null(cgs_heap_pop(&h, &n));
        assert_int_equal(cgs_heap_length(&h), 5);
        assert_int_equal(n, 7);

        assert_non_null(cgs_heap_pop(&h, &n));
        assert_int_equal(cgs_heap_length(&h), 4);
        assert_int_equal(n, 8);

        assert_non_null(cgs_heap_pop(&h, &n));
        assert_int_equal(cgs_heap_length(&h), 3);
        assert_int_equal(n, 9);

        assert_non_null(cgs_heap_pop(&h, &n));
        assert_int_equal(cgs_heap_length(&h), 2);
        assert_int_equal(n, 10);

        assert_non_null(cgs_heap_pop(&h, &n));
        assert_int_equal(cgs_heap_length(&h), 1);
        assert_int_equal(n, 14);

        assert_non_null(cgs_heap_pop(&h, &n));
        assert_int_equal(cgs_heap_length(&h), 0);
        assert_int_equal(n, 16);

        assert_null(cgs_heap_pop(&h, &n));

        cgs_heap_free(&h);
}

int main(void)
{
        const struct CMUnitTest tests[] = {
                cmocka_unit_test(heap_new_test),
                cmocka_unit_test(heap_push_test),
                cmocka_unit_test(heap_growth_test),
                cmocka_unit_test(heap_pop_test),
        };

        return cmocka_run_group_tests(tests, NULL, NULL);
}

