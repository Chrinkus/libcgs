#include "cmocka_headers.h"

#include "cgs_vector.h"
#include "cgs_string.h"

static void
vector_of_strings_test(void** state)
{
        (void)state;

        struct cgs_vector v1 = cgs_vector_new(sizeof(struct cgs_string));
        struct cgs_string buffer = cgs_string_new();
        struct cgs_string mover = cgs_string_new();

        cgs_string_from("Hello darling", &buffer);
        cgs_string_move(&buffer, &mover);
        cgs_vector_push(&v1, &mover);

        cgs_string_from("Nice to see you", &buffer);
        cgs_string_move(&buffer, &mover);
        cgs_vector_push(&v1, &mover);

        cgs_string_from("It's BEEN A LONG TIIIIIME", &buffer);
        cgs_string_move(&buffer, &mover);
        cgs_vector_push(&v1, &mover);

        assert_int_equal(cgs_vector_length(&v1), 3);

        const struct cgs_string* s1 = cgs_vector_get(&v1, 0);
        assert_int_equal(cgs_string_length(s1), 13);
        assert_string_equal(cgs_string_data(s1), "Hello darling");

        const struct cgs_string* s2 = cgs_vector_get(&v1, 1);
        assert_int_equal(cgs_string_length(s2), 15);
        assert_string_equal(cgs_string_data(s2), "Nice to see you");

        const struct cgs_string* s3 = cgs_vector_get(&v1, 2);
        assert_int_equal(cgs_string_length(s3), 25);
        assert_string_equal(cgs_string_data(s3), "It's BEEN A LONG TIIIIIME");

        struct cgs_vector v2 = cgs_vector_new(0);
        assert_non_null(cgs_vector_copy_with(&v1, &v2, cgs_string_copy));

        // last move from 'buff' leaves it empty and unallocated
        cgs_vector_free_all_with(&v1, cgs_string_free);

        const struct cgs_string* s4 = cgs_vector_get(&v2, 0);
        assert_int_equal(cgs_string_length(s4), 13);
        assert_string_equal(cgs_string_data(s4), "Hello darling");
        
        cgs_vector_free_all_with(&v2, cgs_string_free);
}

int main(void)
{
        const struct CMUnitTest tests[] = {
                cmocka_unit_test(vector_of_strings_test),
        };

        return cmocka_run_group_tests(tests, NULL, NULL);
}
