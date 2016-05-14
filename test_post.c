#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "post.h"
#include "post.c"

static void sendstring(char *teststring)
{
    if (post_data) {
	free(post_data);
    }
    post_data = strdup(teststring);
}

static int dotest(char *input, char *value, char *expect)
{
    static int count = 0;
    char *postval;

#define TEST_SUCCESS fprintf(stdout, "Test % 3d: SUCCESS\n", count)
#define TEST_FAIL    fprintf(stderr, "Test % 3d: FAILURE\n", count)

    count++;

    sendstring(input);
    postval = post_get_val(value);

    if (postval == NULL) {
	if (expect == NULL) {
	    TEST_SUCCESS;
	    return 0;
	}
	else {
	    TEST_FAIL;
	    return 1;
	}
    }

    if (strcmp(expect, postval))
    {
	printf("Expected value: %s\n", expect);
	printf("Returned value: %s\n", postval);
	TEST_FAIL;
	free(postval);
	return 1;
    }
    else {
	TEST_SUCCESS;
	free(postval);
	return 0;
    }
}

int main(void)
{
    int rv;

    rv = 0;
    rv += dotest("apa=right&aapa=wrong&apaa=wrong", "apa", "right");
    rv += dotest("aapa=wrong&apa=right&apaa=wrong", "apa", "right");
    rv += dotest("aapa=wrong&apaa=wrong&apa=right", "apa", "right");
    rv += dotest("apa=wrong&bepa=wrong&cepa=wrong", "wrong", NULL);
    rv += dotest("apa=wrong&bepa=wrong&cepa=wrong", "ap", NULL);

    return rv;
}
