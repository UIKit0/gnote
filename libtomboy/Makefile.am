
AM_CPPFLAGS = 				\
	-I$(top_srcdir)			\
	-DG_LOG_DOMAIN=\"libtomboy\"	\
	-DEGG_COMPILATION 		\
	-DGTK_DISABLE_DEPRECATED 	\
	-DGDK_DISABLE_DEPRECATED 	\
	-DG_DISABLE_DEPRECATED		\
	$(GTK_CFLAGS)

tomboylibdir = $(pkglibdir)
noinst_LTLIBRARIES = libtomboy.la

libtomboy_la_SOURCES =		\
	tomboykeybinder.h	\
	tomboykeybinder.c	\
	tomboyutil.h		\
	tomboyutil.c		\
	eggaccelerators.h	\
	eggaccelerators.c

libtomboy_la_CFLAGS = $(X_CFLAGS)
libtomboy_la_LDFLAGS = -export-dynamic -module -avoid-version
libtomboy_la_LIBADD = $(GTK_LIBS) $(X_LIBS)

maintainer-clean-local:
	rm -f Makefile.in
