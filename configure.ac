AC_INIT(gnote, 3.11.2)
AC_PREREQ(2.59)
AC_CONFIG_SRCDIR(README)
AM_INIT_AUTOMAKE([1.11 foreign dist-bzip2])
AM_SILENT_RULES([yes])
AM_MAINTAINER_MODE

YELP_HELP_INIT

AC_CONFIG_HEADER(config.h)
AC_CONFIG_MACRO_DIR(m4)

m4_pattern_allow([^BOOST_])

GNOTE_VERSION="$PACKAGE_VERSION"
AC_SUBST(GNOTE_VERSION)
LIBGNOTE_RELEASE=${PACKAGE_VERSION%.*}
AC_SUBST(LIBGNOTE_RELEASE)
LIBGNOTE_VERSION_INFO=1:0:0
AC_SUBST(LIBGNOTE_VERSION_INFO)


dnl all the library version.
dnl if one is harcoded elsewhere, it is a bug

GTK_VERSION=3.10
LIBGLIBMM_VERSION=2.32
LIBGTKMM_VERSION=3.10
GTKSPELL_VERSION=3.0.0
BOOST_VERSION=1.34
LIBSECRET_VERSION=0.8

AC_PROG_CXX
AC_GNU_SOURCE

AC_ARG_ENABLE(debug,[  --enable-debug    Turn on debugging],[
        case "${enableval}" in
           yes) debug=true ;
                DEBUG_CFLAGS="-DDEBUG -g" ;
                OPTIMIZE_CFLAGS="" ;;
            no) debug=false ;
                DEBUG_CFLAGS="-DNDEBUG" ;;
             *) AC_MSG_ERROR(bad value ${enableval} for --enable-debug) ;;
        esac
],[     debug=false
        DEBUG_CFLAGS="-DNDEBUG"
])
AM_CONDITIONAL(DEBUG, test x$debug = xtrue)


CPPFLAGS="$CPPFLAGS $DEBUG_CFLAGS -Wall -Wcast-align -Wcast-qual -Wformat-security -Wpointer-arith -Wreturn-type"
CFLAGS="$CFLAGS $DEBUG_CFLAGS $OPTIMIZE_CFLAGS"
CXXFLAGS="$CXXFLAGS $DEBUG_CFLAGS $OPTIMIZE_CFLAGS"
dnl CFLAGS=""
LDFLAGS="$LDFLAGS"


dnl AC_PROG_INSTALL
LT_INIT([disable-static])

AC_LANG_CPLUSPLUS
AC_LANG_COMPILER_REQUIRE
AX_CXX_COMPILE_STDCXX_11([], [optional])
AC_CHECK_PROG(HAVE_DESKTOP_FILE_VALIDATE, [desktop-file-validate], [yes], [no])
if test $HAVE_DESKTOP_FILE_VALIDATE == "no"; then
  AC_MSG_ERROR([desktop-file-validate not found])
fi

PKG_CHECK_MODULES(LIBGLIBMM, [glibmm-2.4 >= $LIBGLIBMM_VERSION])
PKG_CHECK_MODULES(GTK, [gtk+-3.0 >= $GTK_VERSION])
PKG_CHECK_MODULES(X, [x11])
PKG_CHECK_MODULES(LIBGTKMM, [gtkmm-3.0 >= $LIBGTKMM_VERSION])
PKG_CHECK_MODULES(LIBXML, [libxml-2.0])
PKG_CHECK_MODULES(LIBXSLT, [libxslt])
PKG_CHECK_MODULES(UUID, [uuid])

PKG_CHECK_MODULES(GTKSPELL, gtkspell3-3.0 >= $GTKSPELL_VERSION,
			    [FIXED_GTKSPELL="yes";AC_DEFINE([FIXED_GTKSPELL], [1], [Set if the GtkSpell library is recent enough.])
			    ], FIXED_GTKSPELL="no")
AC_SUBST(GTKSPELL_CFLAGS)
AC_SUBST(GTKSPELL_LIBS)
AM_CONDITIONAL(FIXED_GTKSPELL, test "$FIXED_GTKSPELL" = "yes")


PKG_CHECK_MODULES(LIBSECRET, [libsecret-1 >= $LIBSECRET_VERSION])



BOOST_REQUIRE([$BOOST_VERSION])
BOOST_BIND
BOOST_FORMAT
BOOST_TEST([s])

if test $HAVE_CXX11 == 1; then
  cxx11_support="yes";
else
  cxx11_support="no";
  BOOST_CONVERSION
  BOOST_FOREACH
  AC_CHECK_HEADER(tr1/memory,,[
	CXXFLAGS="$CXXFLAGS -I/usr/include/boost/tr1";
	AC_MSG_NOTICE([using boost tr1 implementation.])
])
fi


AC_LANG_PUSH(C++)
if test "$GCC" = "yes"; then
        GNOTE_BUILD_CONFIG="$GNOTE_BUILD_CONFIG gcc-options="
        for option in -Wall -Wextra -Wsign-compare -Wpointer-arith \
                      -Wchar-subscripts -Wwrite-strings \
                      -Wunused -Wpointer-arith -Wshadow -fshow-column ; do
                SAVE_CXXFLAGS="$CXXFLAGS"
                CXXFLAGS="$CXXFLAGS $option"
		GNOTE_BUILD_CONFIG="$GNOTE_BUILD_CONFIG$option "
                AC_MSG_CHECKING([whether gcc understands $option])
                AC_TRY_COMPILE([], [],
                        has_option=yes,
                        has_option=no,)
                if test $has_option = no; then
                  CXXFLAGS="$SAVE_CXXFLAGS"
                fi
                AC_MSG_RESULT($has_option)
                unset has_option
                unset SAVE_CXXFLAGS
        done
        unset option
fi
AC_LANG_POP


AC_ARG_WITH(dbus_service_dir, [  --with-dbus-service-dir=DIR            Where to install Gnote's DBus service file.])
AM_CONDITIONAL(WITH_DBUS_SERVICE_DIR, test "x$with_dbus_service_dir" != "x")
if test "x$with_dbus_service_dir" != "x"; then
	DBUS_SERVICE_DIR=$with_dbus_service_dir
else
	DBUS_SERVICE_DIR=${datadir}/dbus-1/services
fi
AC_SUBST(DBUS_SERVICE_DIR)

AC_ARG_WITH(x11_support, [  --with-x11-support      Compile with X11 support.],
            [with_x11_support=yes;AC_DEFINE(HAVE_X11_SUPPORT, [1], [Define to 1 to include X11 support.])],
            [with_x11_support=no])
AM_CONDITIONAL(HAVE_X11_SUPPORT, test "x$with_x11_support" != "x")


IT_PROG_INTLTOOL([0.35.0])

GETTEXT_PACKAGE=gnote
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED([GETTEXT_PACKAGE], ["$GETTEXT_PACKAGE"],
                   [The domain to use with gettext])
AM_GLIB_GNU_GETTEXT

GLIB_GSETTINGS

GNOTE_LOCALEDIR=[${datadir}/locale]
AC_SUBST(GNOTE_LOCALEDIR)

AC_DEFINE_UNQUOTED([GNOTE_BUILD_CONFIG], ["$GNOTE_BUILD_CONFIG"], [The string used to hardcode the build config.])

AC_CONFIG_FILES([
Makefile
data/Makefile
data/gnote.desktop.in
data/icons/Makefile
data/appdata/Makefile
libtomboy/Makefile
src/Makefile
src/addins/Makefile
src/addins/addins.mk
src/addins/backlinks/Makefile
src/addins/bugzilla/Makefile
src/addins/exporttogtg/Makefile
src/addins/exporttohtml/Makefile
src/addins/filesystemsyncservice/Makefile
src/addins/fixedwidth/Makefile
src/addins/inserttimestamp/Makefile
src/addins/notedirectorywatcher/Makefile
src/addins/noteoftheday/Makefile
src/addins/printnotes/Makefile
src/addins/readonly/Makefile
src/addins/replacetitle/Makefile
src/addins/specialnotes/Makefile
src/addins/statistics/Makefile
src/addins/stickynoteimport/Makefile
src/addins/tableofcontents/Makefile
src/addins/todo/Makefile
src/addins/tomboyimport/Makefile
src/addins/underline/Makefile
src/addins/webdavsyncservice/Makefile
src/dbus/Makefile
po/Makefile.in
po/Makefile
help/Makefile
])


AC_OUTPUT


echo "
Options:

  C++11 support:               ${cxx11_support}
  Spellchecking support:       ${FIXED_GTKSPELL}
  X11 support:                 ${with_x11_support}

"
