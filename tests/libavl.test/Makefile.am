check_PROGRAMS = avlint_assign1 avlint_add1
avlint_assign1_SOURCES = avlint_assign1.cpp
avlint_add1_SOURCES = avlint_add1.cpp

AM_CPPFLAGS = -std=c++11 -I$(top_builddir)/lib -I/opt/local/include
AM_LDFLAGS = -L$(top_builddir)/lib -L/opt/local/lib -lavl -lglut -lGL

AM_TESTS_ENVIRONMENT = . $(srcdir)/init_test.sh;
AM_TESTS_FD_REDIRECT = 9>&2

TESTS = \
		avlint_assign1.sh \
		avlint_add1.sh
