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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "fanctrl.h"
#include "util.h"

#define FAN_PORT 27

#define STR(s) #s
#define GPIO_PATH(PORT) "/sys/class/gpio/gpio" STR(PORT)

#define SYSDELAY 10

static int set_gpio_value(int value);

int fanctrl_init(void)
{
    FILE *fp;
    int delay;
    struct timespec stime;

    if (! util_file_exists(GPIO_PATH(FAN_PORT) "/direction")) {
	fp = fopen("/sys/class/gpio/export", "w");
	if (fp == NULL) {
	    fprintf(stderr, "could not open gpio system\n");
	    return FAN_FAN;
	}
	fprintf(fp, "%d", FAN_PORT);
	fclose(fp);
    }

    // wait while stuff is created
    stime.tv_sec = 0;
    stime.tv_nsec = SYSDELAY * 1000 * 1000;

    // we need the value file...
    for (delay = 0; delay < 2000; delay += SYSDELAY) {
	fp = fopen(GPIO_PATH(FAN_PORT) "/value", "r");
	if (fp != NULL) {
	    break;
	}
	nanosleep(&stime, NULL);
    }
    if (fp == NULL) {
	fprintf(stderr, "could not open port value file\n");
	return FAN_FAN;
    }
    fclose(fp);

    // ...and the direction file
    for (delay = 0; delay < 2000; delay += SYSDELAY) {
	fp = fopen(GPIO_PATH(FAN_PORT) "/direction", "w");
	if (fp != NULL) {
	    break;
	}
	nanosleep(&stime, NULL);
    }

    // set pin direction to out
    if (fp == NULL) {
	fprintf(stderr, "could not open port direction file\n");
	return FAN_FAN;
    }
    fprintf(fp, "out");
    fclose(fp);

    return FAN_OK;
}

int fanctrl_on(void)
{
    return set_gpio_value(1);
}

int fanctrl_off(void)
{
    return set_gpio_value(0);
}

int fanctrl_status(int *status)
{
    FILE *fp;
    int data;

    fp = fopen(GPIO_PATH(FAN_PORT) "/value", "r");
    if (fp == NULL) {
	fprintf(stderr, "cannot open fan for reading\n");
	return FAN_FAN;
    }

    data = fgetc(fp);
    fclose(fp);
    switch (data) {
	case '0':
	    *status = FAN_OFF;
	    return FAN_OK;

	case '1':
	    *status = FAN_ON;
	    return FAN_OK;

	default:
	    return FAN_FAN;
    }
}

static int set_gpio_value(int value)
{
    FILE *fp;

    if (value != 0 &&
	value != 1)
    {
	fprintf(stderr, "illegal fan value %d\n", value);
	return FAN_FAN;
    }

    fp = fopen(GPIO_PATH(FAN_PORT) "/value", "w");
    if (fp == NULL) {
	fprintf(stderr, "could not open port value file\n");
	return FAN_FAN;
    }

    fprintf(fp, "%d", value);
    fclose(fp);
    return FAN_OK;
}
