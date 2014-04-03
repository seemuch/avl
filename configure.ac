AC_INIT([avl], [0.1], [avl-plt@googlegroups.com], [],
	[https://github.com/wqfish/avl])

AM_INIT_AUTOMAKE([-Wall foreign])

AC_PROG_CC
AC_PROG_CXX
AC_PROG_LEX
AC_PROG_YACC

AC_CONFIG_MACRO_DIR([m4])
AM_PROG_AR
AC_PROG_LIBTOOL

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
		 Makefile
		 src/Makefile
		 lib/Makefile
		 sample_code/Makefile
		 tests/Makefile
		 tests/libavl.test/Makefile
		 tests/avl.test/Makefile
		 ])

AC_OUTPUT
