CFLAGS	+= --std=gnu11
CFLAGS	+= -g3
CFLAGS	+= -Wall
CFLAGS	+= -Werror
CFLAGS	+= -MMD

OBJS	 = site.o
OBJS	+= macro.o
OBJS	+= fanctrl.o
OBJS	+= fanform.o
OBJS	+= post.o
OBJS	+= util.o

TESTS	+= test_post

default: install

site: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

install: site
	sudo cp $< /var/www/html/index.cgi
	sudo cp template.html /var/www/html

clean:
	$(RM) site *.o *.d $(TESTS)

-include $(OBJS:.o=.d)

######################################################################
#
# TESTS
#
######################################################################

.SECONDARY: $(TESTS)
.PHONY: test tests run_tests

test_%: test_%.c
	$(CC) $(CFLAGS) -o $@ $<

run_test_%: test_%
	./$<

run_tests: $(addprefix run_,$(TESTS))

test tests: $(TESTS)
	$(MAKE) run_tests
