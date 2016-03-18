ACLOCAL_AMFLAGS = -I m4

AM_CPPFLAGS = -I$(top_srcdir) -DEVOCOSM_VERSION=\"$(VERSION)\"

CPPFLAGS=-O3 -g -std=c++14 -Wall

h_sources = evocommon.h evocosm.h \
		evoreal.h roulette.h validator.h stats.h \
		state_machine.h machine_tools.h simple_machine.h fuzzy_machine.h \
		organism.h landscape.h \
		mutator.h scaler.h selector.h reproducer.h \
		analyzer.h listener.h \
		function_optimizer.h \
		command_line.h

cpp_sources = evocommon.cpp evoreal.cpp roulette.cpp function_optimizer.cpp  command_line.cpp

lib_LTLIBRARIES = libevocosm.la

libevocosm_la_SOURCES = $(h_sources) $(cpp_sources)
libevocosm_la_LDFLAGS= -version-info $(GENERIC_LIBRARY_VERSION) -release $(GENERIC_RELEASE)

library_includedir=$(includedir)/$(GENERIC_LIBRARY_NAME)
library_include_HEADERS = $(h_sources)

DEFS=-I. -I$(srcdir)
