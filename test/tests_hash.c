#include "cmocka_headers.h"

#include "cgs_hash.h"

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

static void
hash_new_test(void** state)
{
        (void)state;

        struct cgs_hash h = { 0 };
        assert_non_null(cgs_hash_new(&h));

        assert_int_equal(cgs_hash_length(&h), 0);

        cgs_hash_free(&h);
}

static void
hash_lookup_fail_test(void** state)
{
        (void)state;

        struct cgs_hash h = { 0 };
        assert_non_null(cgs_hash_new(&h));

        assert_null(cgs_hash_lookup(&h, "article"));
        assert_null(cgs_hash_lookup(&h, "section"));
        assert_null(cgs_hash_lookup(&h, "h1"));
        assert_null(cgs_hash_lookup(&h, "div"));

        cgs_hash_free(&h);
}

static void
hash_get_success_test(void** state)
{
        (void)state;
        struct cgs_hash h = { 0 };
        assert_non_null(cgs_hash_new(&h));

        for (size_t i = 0; i < top_scorers_len; ++i) {
                struct cgs_variant* pv = NULL;
                pv = cgs_hash_get(&h, top_scorers[i].name);
                assert_non_null(pv);
                cgs_variant_set_int(pv, top_scorers[i].goals);
        }

        assert_int_equal(cgs_hash_length(&h), top_scorers_len);

        cgs_hash_free(&h);
}

static void
hash_get_lookup_test(void** state)
{
        (void)state;
        struct cgs_hash h = { 0 };
        assert_non_null(cgs_hash_new(&h));

        for (size_t i = 0; i < top_scorers_len; ++i) {
                struct cgs_variant* pv = cgs_hash_get(&h, top_scorers[i].name);
                cgs_variant_set_int(pv, top_scorers[i].goals);
        }

        const int* g = NULL;
        g = cgs_hash_lookup(&h, "Joe Sakic");
        assert_non_null(g);
        assert_int_equal(*g, 625);

        g = cgs_hash_lookup(&h, "Teemu Selanne");
        assert_non_null(g);
        assert_int_equal(*g, 684);

        g = cgs_hash_lookup(&h, "Ron Francis");
        assert_null(g);

        cgs_hash_free(&h);
}

static void
hash_remove_test(void** state)
{
        (void)state;
        struct cgs_hash h = { 0 };
        assert_non_null(cgs_hash_new(&h));

        for (size_t i = 0; i < top_scorers_len; ++i) {
                const struct goal_scorer* gs = &top_scorers[i];
                double gpg = goals_per_game(gs->goals, gs->games);

                struct cgs_variant* pv = cgs_hash_get(&h, gs->name);
                cgs_variant_set_double(pv, gpg);
        }

        assert_int_equal(cgs_hash_length(&h), 20);

        const double* pd = NULL;
        pd = cgs_hash_lookup(&h, "Mario Lemieux");
        assert_non_null(pd);

        cgs_hash_remove(&h, "Mario Lemieux");
        pd = cgs_hash_lookup(&h, "Mario Lemieux");
        assert_null(pd);

        assert_int_equal(cgs_hash_length(&h), 19);

        cgs_hash_remove(&h, "Ron Francis");             // not in list
        assert_int_equal(cgs_hash_length(&h), 19);      // length is same

        cgs_hash_free(&h);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Main
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */ 

int main(void)
{
	const struct CMUnitTest tests[] = {
		cmocka_unit_test(hash_new_test),
                cmocka_unit_test(hash_lookup_fail_test),
		cmocka_unit_test(hash_get_success_test),
                cmocka_unit_test(hash_get_lookup_test),
                cmocka_unit_test(hash_remove_test),
	};

	return cmocka_run_group_tests(tests, NULL, NULL);
}

