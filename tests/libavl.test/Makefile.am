check_PROGRAMS = AvlInt_assign1
AvlInt_assign1_SOURCES = AvlInt_assign1.cpp

AM_CPPFLAGS = -std=c++11 -I$(top_builddir)/lib -I/opt/local/include
AM_LDFLAGS = -L$(top_builddir)/lib -L/opt/local/lib -lavl -lglut -lGL

AM_TESTS_ENVIRONMENT = . $(srcdir)/init_test.sh;
AM_TESTS_FD_REDIRECT = 9>&2

TESTS = \
		AvlInt_assign1.sh
