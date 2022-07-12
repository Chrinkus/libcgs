#include "cmocka_headers.h"

#include "cgs_hashtab.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * NHL Goal Scorer Data
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
struct goal_scorer {
        char* name;
        int goals;
        int games;
};

const struct goal_scorer top_scorers[] = {
        { .name = "Wayne Gretzky",      .goals = 894,   .games = 1487 },
        { .name = "Gordie Howe",        .goals = 801,   .games = 1767 },
        { .name = "Alex Ovechkin",      .goals = 780,   .games = 1274 },
        { .name = "Jaromir Jagr",       .goals = 766,   .games = 1733 },
        { .name = "Brett Hull",         .goals = 741,   .games = 1269 },
        { .name = "Marcel Dionne",      .goals = 731,   .games = 1348 },
        { .name = "Phil Esposito",      .goals = 717,   .games = 1282 },
        { .name = "Mike Gartner",       .goals = 708,   .games = 1432 },
        { .name = "Mark Messier",       .goals = 694,   .games = 1756 },
        { .name = "Steve Yzerman",      .goals = 692,   .games = 1514 },
        { .name = "Mario Lemieux",      .goals = 690,   .games = 915 },
        { .name = "Teemu Selanne",      .goals = 684,   .games = 1451 },
        { .name = "Luc Robitaille",     .goals = 668,   .games = 1431 },
        { .name = "Brendan Shanahan",   .goals = 656,   .games = 1524 },
        { .name = "Dave Andreychuk",    .goals = 640,   .games = 1639 },
        { .name = "Joe Sakic",          .goals = 625,   .games = 1378 },
        { .name = "Jerome Iginla",      .goals = 625,   .games = 1554 },
        { .name = "Bobby Hull",         .goals = 610,   .games = 1063 },
        { .name = "Dino Ciccarelli",    .goals = 608,   .games = 1232 },
        { .name = "Jari Kurri",         .goals = 601,   .games = 1251 },
};
const size_t top_scorers_len = sizeof(top_scorers) / sizeof(top_scorers[0]);

static double
goals_per_game(double goals, double games)
{
        return goals / games;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Number Data
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
struct number {
        char* word;
        int n;
};

const struct number numdata[] = {
        { .word = "one",                .n = 1 },
        { .word = "two",                .n = 2 },
        { .word = "three",              .n = 3 },
        { .word = "four",               .n = 4 },
        { .word = "five",               .n = 5 },
        { .word = "six",                .n = 6 },
        { .word = "seven",              .n = 7 },
        { .word = "eight",              .n = 8 },
        { .word = "nine",               .n = 9 },
        { .word = "ten",                .n = 10 },
        { .word = "eleven",             .n = 11 },
        { .word = "twelve",             .n = 12 },
        { .word = "thirteen",           .n = 13 },
        { .word = "fourteen",           .n = 14 },
        { .word = "fifteen",            .n = 15 },
        { .word = "sixteen",            .n = 16 },
        { .word = "seventeen",          .n = 17 },
        { .word = "eighteen",           .n = 18 },
        { .word = "nineteen",           .n = 19 },
        { .word = "twenty",             .n = 20 },
        { .word = "twenty-one",         .n = 21 },
        { .word = "twenty-two",         .n = 22 },
        { .word = "twenty-three",       .n = 23 },
        { .word = "twenty-four",        .n = 24 },
        { .word = "twenty-five",        .n = 25 },
        { .word = "twenty-six",         .n = 26 },
        { .word = "twenty-seven",       .n = 27 },
};
const size_t numdata_len = sizeof(numdata) / sizeof(numdata[0]);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * 
 * Tests
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 
static void
hashtab_new_test(void** state)
{
        (void)state;

        struct cgs_hashtab h = { 0 };
        assert_non_null(cgs_hashtab_new(&h));

        assert_int_equal(cgs_hashtab_length(&h), 0);

        cgs_hashtab_free(&h);
}

static void
hashtab_lookup_fail_test(void** state)
{
        (void)state;

        struct cgs_hashtab h = { 0 };
        assert_non_null(cgs_hashtab_new(&h));

        assert_null(cgs_hashtab_lookup(&h, "article"));
        assert_null(cgs_hashtab_lookup(&h, "section"));
        assert_null(cgs_hashtab_lookup(&h, "h1"));
        assert_null(cgs_hashtab_lookup(&h, "div"));

        cgs_hashtab_free(&h);
}

static void
hashtab_get_success_test(void** state)
{
        (void)state;
        struct cgs_hashtab h = { 0 };
        assert_non_null(cgs_hashtab_new(&h));

        for (size_t i = 0; i < top_scorers_len; ++i) {
                struct cgs_variant* pv = NULL;
                pv = cgs_hashtab_get(&h, top_scorers[i].name);
                assert_non_null(pv);
                cgs_variant_set_int(pv, top_scorers[i].goals);
        }

        assert_int_equal(cgs_hashtab_length(&h), top_scorers_len);

        cgs_hashtab_free(&h);
}

static void
hashtab_get_lookup_test(void** state)
{
        (void)state;
        struct cgs_hashtab h = { 0 };
        assert_non_null(cgs_hashtab_new(&h));

        for (size_t i = 0; i < top_scorers_len; ++i) {
                const struct goal_scorer* gs = &top_scorers[i];
                struct cgs_variant* pv = cgs_hashtab_get(&h, gs->name);
                cgs_variant_set_int(pv, gs->goals);
        }

        const int* g = NULL;
        g = cgs_hashtab_lookup(&h, "Joe Sakic");
        assert_non_null(g);
        assert_int_equal(*g, 625);

        g = cgs_hashtab_lookup(&h, "Teemu Selanne");
        assert_non_null(g);
        assert_int_equal(*g, 684);

        g = cgs_hashtab_lookup(&h, "Ron Francis");
        assert_null(g);

        cgs_hashtab_free(&h);
}

static void
hashtab_remove_test(void** state)
{
        (void)state;
        struct cgs_hashtab h = { 0 };
        assert_non_null(cgs_hashtab_new(&h));

        for (size_t i = 0; i < top_scorers_len; ++i) {
                const struct goal_scorer* gs = &top_scorers[i];
                double gpg = goals_per_game(gs->goals, gs->games);

                struct cgs_variant* pv = cgs_hashtab_get(&h, gs->name);
                cgs_variant_set_double(pv, gpg);
        }

        assert_int_equal(cgs_hashtab_length(&h), 20);

        const double* pd = NULL;
        pd = cgs_hashtab_lookup(&h, "Mario Lemieux");
        assert_non_null(pd);

        cgs_hashtab_remove(&h, "Mario Lemieux");
        pd = cgs_hashtab_lookup(&h, "Mario Lemieux");
        assert_null(pd);

        assert_int_equal(cgs_hashtab_length(&h), 19);

        cgs_hashtab_remove(&h, "Ron Francis");             // not in list
        assert_int_equal(cgs_hashtab_length(&h), 19);      // length is same

        cgs_hashtab_free(&h);
}

static void
hashtab_load_test(void** state)
{
        (void)state;

        struct cgs_hashtab ht = { 0 };
        cgs_hashtab_new(&ht);
        assert_float_equal(cgs_hashtab_current_load(&ht), 0.0, 0.0);

        size_t i = 0;           // bookmark index

        const struct number* pn = &numdata[i];
        struct cgs_variant* pv = cgs_hashtab_get(&ht, pn->word);
        cgs_variant_set_int(pv, pn->n);
        // load factor with 1 value: 1 / 31 = 0.032258
        assert_float_equal(cgs_hashtab_current_load(&ht), 0.03, 0.01);

        for ( ; i < 10; ++i) {
                pn = &numdata[i];
                pv = cgs_hashtab_get(&ht, pn->word);
                cgs_variant_set_int(pv, pn->n);
        }
        // load factor with 10 values: 10 / 31 = 0.32258
        assert_float_equal(cgs_hashtab_current_load(&ht), 0.322, 0.01);

        for ( ; i < 24; ++i) {
                pn = &numdata[i];
                pv = cgs_hashtab_get(&ht, pn->word);
                cgs_variant_set_int(pv, pn->n);
        }
        // load factor with 24 values: 24 / 31 = 0.77419
        assert_float_equal(cgs_hashtab_current_load(&ht), 0.774, 0.01);

        // adding a value should trigger re-hash
        pn = &numdata[i++];
        pv = cgs_hashtab_get(&ht, pn->word);
        cgs_variant_set_int(pv, pn->n);
        // hashtab size jumps from 31 to next prime after 31 * 2 => 67
        // load factor with 25 elements: 25 / 67 = 0.37313
        assert_float_equal(cgs_hashtab_current_load(&ht), 0.373, 0.01);

        cgs_hashtab_free(&ht);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Main
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(hashtab_new_test),
                cmocka_unit_test(hashtab_lookup_fail_test),
		cmocka_unit_test(hashtab_get_success_test),
                cmocka_unit_test(hashtab_get_lookup_test),
                cmocka_unit_test(hashtab_remove_test),
                cmocka_unit_test(hashtab_load_test),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

