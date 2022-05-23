#include "cmocka_headers.h"

#include "cgs_heap_private.h"
#include "cgs_compare.h"

static void heap_left_test(void** state)
{
        (void)state;
        assert_int_equal(cgs_heap_left(0), 1);
        assert_int_equal(cgs_heap_left(1), 3);
        assert_int_equal(cgs_heap_left(2), 5);
}

static void heap_right_test(void** state)
{
        (void)state;
        assert_int_equal(cgs_heap_right(0), 2);
        assert_int_equal(cgs_heap_right(1), 4);
        assert_int_equal(cgs_heap_right(2), 6);
}

static void heap_parent_test(void** state)
{
        (void)state;
        assert_int_equal(cgs_heap_parent(0), 0);
        assert_int_equal(cgs_heap_parent(1), 0);
        assert_int_equal(cgs_heap_parent(2), 0);
        assert_int_equal(cgs_heap_parent(3), 1);
        assert_int_equal(cgs_heap_parent(4), 1);
        assert_int_equal(cgs_heap_parent(5), 2);
        assert_int_equal(cgs_heap_parent(6), 2);
}

int main(void)
{
        const struct CMUnitTest tests[] = {
                cmocka_unit_test(heap_left_test),
                cmocka_unit_test(heap_right_test),
                cmocka_unit_test(heap_parent_test),
        };

        return cmocka_run_group_tests(tests, NULL, NULL);
}

