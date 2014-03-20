check_PROGRAMS = add1 sub1
add1_SOURCES = add1.cpp
sub1_SOURCES = sub1.cpp

AM_CPPFLAGS = -std=c++11 -I$(top_builddir)/lib -I/opt/local/include
AM_LDFLAGS = -L$(top_builddir)/lib -L/opt/local/lib -lavl -lglut -lGL

TESTS = \
		add1.sh \
		sub1.sh
