#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>              // toupper
#include <string.h>             // strlen
#include <cgs/cgs_string.h>

void upperizer(char* s)
{
        for ( ; *s; ++s)
                *s = toupper(*s);
}

int main(void)
{
        struct cgs_string s1 = { 0 };
        if (!cgs_string_new(&s1))               // check allocation
                return EXIT_FAILURE;

        for (int c; (c = getchar()) != EOF && c != '\n'; )
                if (!cgs_string_push(&s1, c))   // push may grow string
                        return EXIT_FAILURE;

        upperizer(cgs_string_data_mutable(&s1));

        const char* s2 = " days ahead with ";

        struct cgs_string s3 = { 0 };
        if (!cgs_string_new_from_str(&s3, "Good"))      // check allocation
                return EXIT_FAILURE;

        if (!cgs_string_append_str(&s3, s2, strlen(s2)))
                return EXIT_FAILURE;

        if (!cgs_string_prepend(&s1, &s3))
                return EXIT_FAILURE;

        cgs_string_free(&s3);                   // clean up..

        char* p = cgs_string_xfer(&s1);         // ..or transfer ownership!

        printf("%s\n", p);

        free(p);                                // don't forget to free xfer'd
        return EXIT_SUCCESS;
}
