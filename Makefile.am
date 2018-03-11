AUTOMAKE_OPTIONS = foreign
bin_PROGRAMS = syntheffect
syntheffect_SOURCES=\
		    src/main.cc \
		    src/midi.cc \
		    src/app.cc \
		    src/synth/history_explorer_synth.cc \
		    src/synth/derivative_synth.cc \
		    src/synth/cat_explorer_synth.cc \
		    src/synth.cc
syntheffect_CPPFLAGS=$(RTMIDI_CFLAGS) $(OPENCV_CFLAGS) $(SDL_CFLAGS) -I$(top_srcdir)/include
syntheffect_LDFLAGS=$(RTMIDI_LIBS) $(OPENCV_LIBS) $(SDL_LIBS)
