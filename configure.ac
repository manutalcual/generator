dnl Standard configure.ac
AC_PREREQ(2.53)
AC_INIT([generator], [0.1.0], [manuelcano@by.com.es])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE([dist-bzip2])
AM_CONFIG_HEADER([config.h:config.in])
AC_CONFIG_SRCDIR([src/main.cc])

dnl Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_LIBTOOL

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([sys/types.h sys/socket.h stdlib.h unistd.h string fstream])

AC_ARG_WITH([log],
 	AC_HELP_STRING([--with-log],
 	[Build with log. @<:@default=no@:>@]),
 	[with_log=$withval])

AS_IF([test "x$with_log" == xyes],
      [AC_DEFINE([WITH_LOG], [1],
                     [Defined if you want output log.])])

AC_ARG_WITH([syslog],
 	AC_HELP_STRING([--with-syslog],
 	[Output log and debugging traces to syslog.]),
 	[with_syslog=$withval])

AS_IF([test "x$with_syslog" == xyes],
      [AC_DEFINE([USE_SYSLOG], [1],
                     [Defined to output debbuging traces and log to syslog.])])

AC_ARG_WITH([log-domain],
 	AC_HELP_STRING([--with-log-domain],
 	[If built with syslog, this will be the domain.]),
 	[log_domain=$withval])

AS_IF([test "x$log_domain" != x],
      [AC_DEFINE([LOG_DOMAIN], [$withlog],
                     [Syslog domain log.])])

CFLAGS="-Wall -g $CFLAGS"
CXXFLAGS="-Wall -g -std=gnu++11"

AC_SUBST(CFLAGS)
AC_SUBST(CXXFLAGS)
AC_CONFIG_FILES([Makefile
	etc/Makefile
	src/Makefile])
AC_OUTPUT
