
include $(builddir)/../addins.mk

@INTLTOOL_DESKTOP_RULE@

desktop_in_files = tableofcontents.desktop.in
desktop_files    = $(desktop_in_files:.desktop.in=.desktop)

addinsdir = $(ADDINSDIR)
addins_LTLIBRARIES = tableofcontents.la
addins_DATA = $(desktop_files)


tableofcontents_la_SOURCES = \
	tableofcontents.hpp          \
	tableofcontentsnoteaddin.hpp \
	tableofcontentsnoteaddin.cpp \
	tableofcontentsmenuitem.hpp  \
	tableofcontentsmenuitem.cpp  \
	tableofcontentsaction.hpp    \
	tableofcontentsaction.cpp    \
	$(NULL)

EXTRA_DIST     = $(desktop_in_files)
DISTCLEANFILES = $(desktop_files)
