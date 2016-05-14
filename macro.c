/***************************************************************************
 *
 * Copyright (c) 2016 Mathias Thore
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS
 * BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
 * ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 ***************************************************************************/
#define _GNU_SOURCE
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "macro.h"

static char *process_macro(char *line, char *macro, macro_func_t *func);

char *macro_apply(char *line, macro_t *macros)
{
    int m;

    for (m = 0; macros[m].pattern != NULL; m++) {
	line = process_macro(line, macros[m].pattern, macros[m].func);
    }

    return line;
}

static char *process_macro(char *line, char *macro, macro_func_t *func)
{
    char *tpre, *tpost, *tmacro, *replace, *newline;

    tmacro = strstr(line, macro);
    if (tmacro == NULL) {
	return line;
    }

    *tmacro = '\0';
    tpre = line;
    tpost = tmacro + strlen(macro);

    replace = func();

    asprintf(&newline, "%s%s%s", tpre, replace, tpost);

    free(replace);
    free(line);

    return newline;
}
