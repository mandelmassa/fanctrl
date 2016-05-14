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
#include <stdarg.h>

#include "fanform.h"
#include "fanctrl.h"
#include "post.h"
#include "util.h"

static char *state_names[] = {
    "off",
    "on",
    "unknown"
};

char *macro_fanform(void)
{
    int fan_ok, fan_state;
    char *fcmd;
    char *formstr;
    char *form = ""
	"<form action=\"/\" method=\"post\">\n"
	" <input type=\"submit\" name=\"fan\" value=\"On\" />\n"
	" <input type=\"submit\" name=\"fan\" value=\"Off\" />\n"
	"</form>\n";

    fan_ok = fanctrl_init();

    if (fan_ok == FAN_OK) {
	fcmd = post_get_val("fan");
	if (fcmd != NULL) {
	    if (strcmp(fcmd, "On") == 0) {
		fan_ok = fanctrl_on();
	    }
	    else if (strcmp(fcmd, "Off") == 0) {
		fan_ok = fanctrl_off();
	    }
	    free(fcmd);
	}
    }

    /* fan_state
     * 0  fan off
     * 1  fan on
     * 2  fan in unknown state
     */
    fan_state = 2;
    if (fan_ok == FAN_OK) {
	fanctrl_status(&fan_state);
    }

    formstr = NULL;
    util_appendstr(&formstr, "<div class=\"fanform\">\n");
    util_appendstr(&formstr, form);
    util_appendstr(&formstr, "<p class=\"status\">Fan state: %s</p>\n",
		   state_names[fan_state]);
    util_appendstr(&formstr, "</div>\n");

    return formstr;
}
