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

default: install

site: $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) $(LIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

install: site
	sudo cp $< /var/www/html/index.cgi
	sudo cp template.html /var/www/html

clean:
	$(RM) site *.o *.d

-include $(OBJS:.o=.d)
