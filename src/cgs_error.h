/* cgs_error.h
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
#pragma once

/**
 * cgs_error_retfail
 *
 * Prints the error message to stderr and returns EXIT_FAILURE.
 *
 * @param format        Printf-style format string.
 * @param ...           Additional arguments to format string.
 *
 * @return              EXIT_FAILURE.
 */
int
cgs_error_retfail(const char* format, ...);

/**
 * cgs_error_retnull
 *
 * Prints the error message to stderr and returns NULL.
 *
 * @param format        Printf-style format string.
 * @param ...           Additional arguments to format string.
 *
 * @return              NULL.
 */
void*
cgs_error_retnull(const char* format, ...);

/**
 * cgs_error_retbool
 *
 * Prints the error message to stderr and returns zero.
 *
 * @param format        Printf-style format string.
 * @param ...           Additional arguments to format string.
 *
 * @return              Zero (0).
 */
int
cgs_error_retbool(const char* format, ...);

/**
 * cgs_error_sysstr
 *
 * Gets the corresponding error string to the current value of errno. Saves
 * the caller from having to include headers for error handling.
 *
 * Warning: Though the string returned is a 'char*' do not modify it, the
 * buffer belongs to the system and may be reused by another system call.
 */
char*
cgs_error_sysstr(void);

