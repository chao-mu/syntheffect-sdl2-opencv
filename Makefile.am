AUTOMAKE_OPTIONS = foreign
bin_PROGRAMS = syntheffect
syntheffect_SOURCES=\
		    src/main.cc \
		    src/midi.cc \
		    src/app.cc \
		    src/module/history_explorer_module.cc \
		    src/module/derivative_module.cc \
		    src/module/cat_explorer_module.cc \
		    src/module/color_tweak_module.cc \
		    src/module/writer_module.cc \
		    src/module.cc
syntheffect_CPPFLAGS=$(RTMIDI_CFLAGS) $(BOOST_CPPFLAGS) $(OPENCV_CFLAGS) $(SDL_CFLAGS) -I$(top_srcdir)/include
syntheffect_LDFLAGS=$(RTMIDI_LIBS) $(BOOST_LDFLAGS) $(OPENCV_LIBS) $(SDL_LIBS)
