#include "cmocka_headers.h"

#include "cgs_vector.h"
#include "cgs_string.h"

static void
vector_of_strings_test(void** state)
{
        (void)state;

        struct cgs_vector v = cgs_vector_new(sizeof(struct cgs_string));
        struct cgs_string buffer = cgs_string_new();
        struct cgs_string mover = cgs_string_new();

        cgs_string_from("Hello darling", &buffer);
        cgs_string_move(&buffer, &mover);
        cgs_vector_push(&v, &mover);

        cgs_string_from("Nice to see you", &buffer);
        cgs_string_move(&buffer, &mover);
        cgs_vector_push(&v, &mover);

        cgs_string_from("It's BEEN A LONG TIIIIIME", &buffer);
        cgs_string_move(&buffer, &mover);
        cgs_vector_push(&v, &mover);

        assert_int_equal(cgs_vector_length(&v), 3);

        const struct cgs_string* s1 = cgs_vector_get(&v, 0);
        assert_int_equal(cgs_string_length(s1), 13);
        assert_string_equal(cgs_string_data(s1), "Hello darling");

        const struct cgs_string* s2 = cgs_vector_get(&v, 1);
        assert_int_equal(cgs_string_length(s2), 15);
        assert_string_equal(cgs_string_data(s2), "Nice to see you");

        const struct cgs_string* s3 = cgs_vector_get(&v, 2);
        assert_int_equal(cgs_string_length(s3), 25);
        assert_string_equal(cgs_string_data(s3), "It's BEEN A LONG TIIIIIME");

        cgs_vector_free_all_with(&v, cgs_string_free);
        // last move from 'buff' leaves it empty and unallocated
}

int main(void)
{
        const struct CMUnitTest tests[] = {
                cmocka_unit_test(vector_of_strings_test),
        };

        return cmocka_run_group_tests(tests, NULL, NULL);
}
