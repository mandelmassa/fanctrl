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
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>

#include "util.h"

char *_util_bin = NULL;

void util_init(int argc, char *argv[], char *env[])
{
    _util_bin = argv[0];
}

char *util_timestamp(void)
{
    time_t now;
    struct tm *now_tm;
    char *asc, *rasc;

    asc = (char *)malloc(200);
    now = time(NULL);
    now_tm = localtime(&now);
    strftime(asc, 200, "%c", now_tm);
    rasc = strdup(asc);

    return rasc;
}

void util_appendstr(char **str, char *fmt, ...)
{
    char *fmtstr, *appstr;
    va_list ap;

    va_start(ap, fmt);
    vasprintf(&fmtstr, fmt, ap);
    va_end(ap);

    if (*str == NULL) {
	*str = fmtstr;
    }
    else {
	asprintf(&appstr, "%s%s", *str, fmtstr);
	free(*str);
	free(fmtstr);
	*str = appstr;
    }
}

int util_file_exists(char *name)
{
    struct stat dummy;

    return (stat(name, &dummy) == 0);
}
