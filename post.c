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

static char *_post_data = NULL;

static char *post_get_data(void);

char *post_get_val(char *name)
{
    char *post;
    char *param, *paramsave;
    char *pname, *pnamesave;
    char *pval;

    post = post_get_data();
    if (post == NULL) {
	return NULL;
    }

    pval = NULL;
    for (param = strtok_r(post, "&", &paramsave);
	 param != NULL;
	 param = strtok_r(NULL, "&", &paramsave))
    {
	pname = strtok_r(param, "=", &pnamesave);
	if (strcmp(pname, name) == 0) {
	    pval = strtok_r(NULL, "=", &pnamesave);
	    if (pval == NULL || strlen(pval) == 0) {
		pval = NULL;
	    }
	    else {
		pval = strdup(pval);
	    }
	    break;
	}
    }

    free(post);
    return pval;
}

static char *post_get_data(void)
{
#define INPUTBUF 1024

    char *len;
    size_t leni;
    static char input[INPUTBUF];

    if (_post_data != NULL) {
	return strdup(_post_data);
    }

    len = getenv("CONTENT_LENGTH");
    if (len == NULL) {
	return NULL;
    }
    leni = strtoul(len, NULL, 0);

    if (leni > INPUTBUF) {
	return NULL;
    }
    if (leni == 0) {
	return NULL;
    }

    memset(input, 0, sizeof(input));
    fgets(input, INPUTBUF - 1, stdin);

    _post_data = input;
    return strdup(_post_data);
}
