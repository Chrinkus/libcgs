/* cgs_numeric.c
 *
 * MIT License
 * 
 * Copyright (c) 2022 Chris Schick
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#include "cgs_numeric.h"
#include "cgs_defs.h"

enum cgs_numeric_magic {
        PRIME_MAGIC = 6,
};

int
cgs_is_prime(int n)
        // Adapted from Wikipedia article "Primality test"
        // https://en.wikipedia.org/wiki/Primality_test
{
        if (n == 2 || n == 3)
                return CGS_TRUE;

        if (n <= 1 || n % 2 == 0 || n % 3 == 0)
                return CGS_FALSE;

        for (int i = 5; i * i <= n; i += PRIME_MAGIC)
                if (n % i == 0 || n % (i + 2) == 0)
                        return CGS_FALSE;

        return CGS_TRUE;
}

int
cgs_next_prime(int n)
{
        n += n % 2 == 0 ? 1 : 2;

        if (cgs_is_prime(n))
                return n;

        // set n to the next multiple of PRIME_MAGIC (6)
        n = (n / PRIME_MAGIC + 1) * PRIME_MAGIC;

        for ( ; 1; n += PRIME_MAGIC) {
                if (cgs_is_prime(n-1))
                        return n-1;
                if (cgs_is_prime(n+1))
                        return n+1;
        }
}

