# Fan Control

A simple c CGI and GPIO project for Raspberry PI by Mathias Thore 2016.

## Contents

The program is written in c. It controls a fan (on/off) on the
Raspberry PI GPIO pin 27 (GPIO.2). This happens in `fanctrl.c`.

It is written as a CGI program, so its output is html, and input is
taken from html POST data. This processing happens in `post.c`.

The program provides a template based macro system. The main function,
in `site.c`, opens `template.html`, and processes each line looking
for specified macro markers. When a macro marker is discovered, an
associated macro function is called. The output of this function is
used in place of the marker. The macro processing is implemented in
`macro.c`.

Most macros are implemented directly in `site.c`. But the fan control
form macro has its own file, `fanform.c`.

A function for dynamically allocated string appending with formatting,
and other utility functions, are implemented in `util.c`.

The CSS present in the template has been tuned to display well on a
cell phone.

## Instructions

Type `make` to build and install the program and its template. The
files will be placed under `/var/www/html`.

Apache must be running on the Raspberry PI, and the www-data user
needs access to the GPIO device. This can be achieved by putting the
following in a file called `/etc/udev/rules.d/99-gpio.rules`:
```
KERNEL=="gpio*", RUN="/bin/sh -c 'chgrp -R gpio /sys/%p /sys/class/gpio && chmod -R g+w /sys/%p /sys/class/gpio'"
```

And then adding www-data to the gpio group like this:
```
sudo useradd -g www-data gpio

```

The site configuration (`/etc/apache2/sites-enabled/000-default.conf`)
needs the following addition to execute CGI scripts in the site root:
```
	<Directory "/var/www/html">
		   DirectoryIndex index.cgi
		   AllowOverride None
		   Options +ExecCGI -MultiViews +SymLinksIfOwnerMatch
		   Require all granted
		   AddHandler cgi-script .cgi
	</Directory>
```

## Limitations

Only one macro per line in the template can be processed.
