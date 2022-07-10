#include <stdio.h>
#include <cgs/cgs.h>

int main(void)
{
        struct cgs_string s = { 0 };
        cgs_string_new_from_string(&s, "Hello World!");

        printf("%s\n", cgs_string_data(&s));

        cgs_string_free(&s);
}
