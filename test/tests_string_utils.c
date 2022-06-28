#include "cgs_string_utils.h"
#include "cgs_test.h"

int strdup_test(void* data)
{
	const char* s1 = data;
	char* s2 = cgs_strdup(s1);

	assert(s2 != NULL);
	assert(s2 != s1);
	assert(strcmp(s1, s2) == 0);

	free(s2);

	const char* s3 = "";
	char* s4 = cgs_strdup(s3);

	assert(s3 != NULL);
	assert(s4 != s3);
	assert(strcmp(s3, s4) == 0);

	free(s4);

	return TEST_SUCCESS;
}

int strmove_test(void* data)
{
        (void)data;

        char s1[12] = "World";
        cgs_strmove(s1, 6);
        assert(s1[6] == 'W');
        assert(s1[7] == 'o');
        assert(s1[8] == 'r');
        assert(s1[9] == 'l');
        assert(s1[10] == 'd');
        assert(s1[11] == '\0');

        char* s2 = malloc(128);
        for (int i = 0; i < 128; ++i)
                s2[i] = '\0';

        s2[0] = 'H';
        s2[1] = 'e';
        s2[2] = 'l';
        s2[3] = 'l';
        s2[4] = 'o';

        cgs_strmove(s2, 60);
        assert(s2[60] == 'H');
        assert(s2[61] == 'e');
        assert(s2[62] == 'l');
        assert(s2[63] == 'l');
        assert(s2[64] == 'o');

        free(s2);

        return TEST_SUCCESS;
}

int strprepend_test(void* data)
{
        (void)data;

        char s1[32] = "World!";
        const char s2[] = "Hello ";
        cgs_strprepend(s1, s2, strlen(s2));
        assert(strcmp(s1, "Hello World!") == 0);

        char* s3 = malloc(128);
        strcpy(s3, " useful?");
        cgs_strprepend(s3, " it", 3);
        cgs_strprepend(s3, " is", 3);
        cgs_strprepend(s3, "Butter,", 3);
        assert(strcmp(s3, "But is it useful?") == 0);

        free(s3);

        return TEST_SUCCESS;
}

int strappendch_test(void* data)
{
	(void)data;

	char s1[10] = "";

	cgs_strappendch(s1, 'a');
	assert(strcmp(s1, "a") == 0);

	cgs_strappendch(s1, 'b');
	assert(strcmp(s1, "ab") == 0);

	char* s2 = malloc(sizeof(char) * 10);
	*s2 = '\0';

	cgs_strappendch(s2, '0');
	assert(strcmp(s2, "0") == 0);

	cgs_strappendch(s2, '1');
	assert(strcmp(s2, "01") == 0);

	free(s2);

	return TEST_SUCCESS;
}

int strprependch_test(void* data)
{
	(void)data;

	char s1[10] = "mile";

	cgs_strprependch(s1, 's');
	assert(strcmp(s1, "smile") == 0);

	char* s2 = malloc(sizeof(char) * 10);
	*s2 = '\0';

	cgs_strprependch(s2, 't');
	cgs_strprependch(s2, 'a');
	cgs_strprependch(s2, 'o');
	cgs_strprependch(s2, 'g');
	assert(strcmp(s2, "goat") == 0);

	free(s2);

	return TEST_SUCCESS;
}

int strshiftl_test(void* data)
{
	(void)data;

	char s1[] = "Hello!";

	cgs_strshiftl(s1, 1, '\0');
	assert(strcmp(s1, "ello!") == 0);

	cgs_strshiftl(s1, 0, '\0');
	assert(strcmp(s1, "ello!") == 0);

	cgs_strshiftl(s1, 2, '!');
	assert(strcmp(s1, "lo!!!") == 0);

	cgs_strshiftl(s1, 10, '\0');
	assert(strcmp(s1, "") == 0);

	char* s2 = malloc(sizeof(char) * 20);
	strcpy(s2, "less than 18");

	cgs_strshiftl(s2, 5, '-');
	assert(strcmp(s2, "than 18-----") == 0);

	free(s2);

	char s3[] = "Such a long string! Who knows how many characters?!";
	cgs_strshiftl(s3, -1, '\0');
	assert(strcmp(s3, "") == 0);

	return TEST_SUCCESS;
}

int strshiftr_test(void* data)
{
        (void) data;

        char s1[] = "Hello";
        cgs_strshiftr(s1, 2, '\0');
        assert(strlen(s1) == 0);
        assert(s1[0] == '\0');
        assert(s1[1] == '\0');
        assert(s1[2] == 'H');
        assert(s1[3] == 'e');
        assert(s1[4] == 'l');
        assert(s1[5] == '\0');

        char* s2 = malloc(32);
        strcpy(s2, "00000000");

        cgs_strshiftr(s2, 4, '1');
        assert(strcmp(s2, "11110000") == 0);
        free(s2);

        return TEST_SUCCESS;
}

int strtrim_test(void* data)
{
	(void)data;

	char s1[] = " \t trim it! \n\t \t";

	cgs_strtrim(s1);
	assert(strcmp(s1, "trim it!") == 0);

	char* s2 = malloc(sizeof(char) * 20);
	strcpy(s2, " typical\n");

	cgs_strtrim(s2);
	assert(strcmp(s2, "typical") == 0);

	free(s2);

	return TEST_SUCCESS;
}

int strtrimch_test(void* data)
{
	(void)data;

	char s1[] = "00Hello 0!000";

	cgs_strtrimch(s1, '0');
	assert(strcmp(s1, "Hello 0!") == 0);

	cgs_strtrimch(s1, 'H');
	assert(strcmp(s1, "ello 0!") == 0);
	cgs_strtrimch(s1, '!');
	assert(strcmp(s1, "ello 0") == 0);
	cgs_strtrimch(s1, 'l');
	assert(strcmp(s1, "ello 0") == 0);

	char* s2 = malloc(sizeof(char) * 20);
	strcpy(s2, "   christmas   ");

	cgs_strtrimch(s2, '-');
	assert(strcmp(s2, "   christmas   ") == 0);
	cgs_strtrimch(s2, ' ');
	assert(strcmp(s2, "christmas") == 0);

	free(s2);

	return TEST_SUCCESS;
}

int main(void)
{
	struct test tests[] = {
		{ "strdup", strdup_test, "Ninja doesn't like gnu_strdup" },
                { "strmove", strmove_test, NULL },
                { "strprepend", strprepend_test, NULL },
		{ "strappendch", strappendch_test, NULL },
		{ "strprependch", strprependch_test, NULL },
		{ "strshiftl", strshiftl_test, NULL },
		{ "strshiftr", strshiftr_test, NULL },
		{ "strtrim", strtrim_test, NULL },
		{ "strtrimch", strtrimch_test, NULL },
	};

	return cgs_run_tests(tests);
}

