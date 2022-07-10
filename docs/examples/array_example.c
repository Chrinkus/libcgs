#include <stdio.h>
#include <stdlib.h>

#include <cgs/cgs_array.h>

int main(void)
{
        struct cgs_array arr = { 0 };
        if (!cgs_array_new(&arr, sizeof(int)))
                return EXIT_FAILURE;

        for (int n; scanf(" %d", &n) == 1; )
                if (!cgs_array_push(&arr, &n))
                        return EXIT_FAILURE;

        printf("%zu integers read\n", cgs_array_length(&arr));

        int positive_sum = 0;

        for (size_t i = 0; i < cgs_array_length(&arr); ++i) {
                const int* pi = cgs_array_get(&arr, i);
                if (*pi > 0)
                        positive_sum += *pi;
        }

        printf("The sum of positive integers is %d\n", positive_sum);

        int negative_sum = 0;
        const int* ai = cgs_array_data(&arr);
        for (size_t i = 0; i < cgs_array_length(&arr); ++i)
                if (ai[i] < 0)
                        negative_sum += ai[i];

        printf("The sum of negative integers is %d\n", negative_sum);

        cgs_array_free(&arr);
        return EXIT_SUCCESS;
}
