#include "cmocka_headers.h"

#include "cgs_heap.h"
#include "cgs_compare.h"

static void heap_new_test(void** state)
{
        (void)state;

        struct cgs_heap h1 = { 0 };

        const void* res = cgs_heap_new(&h1, sizeof(int), cgs_int_cmp);
        assert_non_null(res);

        cgs_heap_free(&h1);
}

int main(void)
{
        const struct CMUnitTest tests[] = {
                cmocka_unit_test(heap_new_test),
        };

        return cmocka_run_group_tests(tests, NULL, NULL);
}

