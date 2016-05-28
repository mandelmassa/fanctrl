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
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "macro.h"
#include "fanform.h"
#include "util.h"

static macro_func_t macro_bgcolor;
static macro_func_t macro_color;
static macro_func_t macro_time;

#define MACRO_BGCOLOR "%bgcolor%"
#define MACRO_COLOR   "%color%"
#define MACRO_TIME    "%time%"

static macro_t macros[] = {
    { MACRO_BGCOLOR, macro_bgcolor },
    { MACRO_COLOR,   macro_color },
    { MACRO_TIME,    macro_time },
    { MACRO_FANFORM, macro_fanform },
    { NULL,          NULL },
};

int main(int argc, char *argv[], char *env[])
{
    FILE *fp;
    int rv = 0;
    char *line;
    ssize_t read;
    size_t len;

    util_init(argc, argv, env);

    fp = fopen("template.html", "r");
    if (fp == NULL) {
	TRACE_ERROR("templates missing");
	rv = 1;
	goto bail;
    }

    line = NULL;
    while ((read = getline(&line, &len, fp)) != -1) {
	line = macro_apply(line, macros);
	printf("%s", line);
	free(line);
	line = NULL;
    }

  bail:
    if (fp) {
	fclose(fp);
    }
    return rv;
}

static char *macro_color(void)
{
    time_t now;
    struct tm *now_tm;
    int hour;
    static char *bgcolors[] = {
	"#fff", //  0
	"#fff", //  6
	"#000", // 12
	"#fff", // 18
    };

    now = time(NULL);
    now_tm = localtime(&now);
    hour = now_tm->tm_hour;

    return strdup(bgcolors[hour / 6]);
}

static char *macro_bgcolor(void)
{
    time_t now;
    struct tm *now_tm;
    int hour;
    static char *bgcolors[] = {
	"#000000", //  0
	"#2a5b89", //  6
	"#8ab4db", // 12
	"#0c1a27", // 18
    };

    now = time(NULL);
    now_tm = localtime(&now);
    hour = now_tm->tm_hour;

    return strdup(bgcolors[hour / 6]);
}

static char *macro_time(void)
{
    return util_timestamp();
}
