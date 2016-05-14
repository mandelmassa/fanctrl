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
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "post.h"

static char *post_data = NULL;

static char *post_get_data(void);

char *post_get_val(char *name)
{
    char *post;
    char *hit, *hitend, *strret;
    int namelen;

    post = post_get_data();
    if (post == NULL) {
	return NULL;
    }

    namelen = strlen(name);
    for (hit = strstr(post, name); hit; hit = strstr(hit, name)) {
	// check for phony prefix match
	if (hit[namelen] != '=') {
	    hit++;
	    continue;
	}

	// check for phony suffix match
	if (hit != post &&
	    hit[-1] != '&')
	{
	    hit++;
	    continue;
	}

	// ok, extract value
	hitend = strstr(hit, "&");
	if (hitend) {
	    *hitend = '\0';
	}
	strret = strdup(hit + namelen + 1);
	if (hitend) {
	    *hitend = '&';
	}
	return strret;
    }

    return NULL;
}

static char *post_get_data(void)
{
#define INPUTBUF 1024

    char *len;
    size_t leni;
    static char input[INPUTBUF];

    if (post_data != NULL) {
	return post_data;
    }

    len = getenv("CONTENT_LENGTH");
    if (len == NULL) {
	return NULL;
    }
    leni = strtoul(len, NULL, 0);

    if (leni > INPUTBUF) {
	return NULL;
    }

    memset(input, 0, sizeof(input));
    fgets(input, INPUTBUF - 1, stdin);

    post_data = input;
    return post_data;
}
