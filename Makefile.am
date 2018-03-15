AUTOMAKE_OPTIONS = foreign
bin_PROGRAMS = syntheffect
syntheffect_SOURCES=\
		    src/main.cc \
		    src/midi.cc \
		    src/app.cc \
		    src/module/history_explorer.cc \
		    src/module/derivative.cc \
		    src/module/cat_explorer.cc \
		    src/module/color_tweak.cc \
		    src/module/writer.cc \
		    src/module/module.cc \
		    src/module/feedback.cc \
		    src/module/tracker.cc \
		    src/module/merger.cc \
		    src/synth/cmd_micro.cc
syntheffect_CPPFLAGS=$(RTMIDI_CFLAGS) $(BOOST_CPPFLAGS) $(OPENCV_CFLAGS) $(SDL_CFLAGS) -I$(top_srcdir)/include
syntheffect_LDFLAGS=$(RTMIDI_LIBS) $(BOOST_LDFLAGS) $(OPENCV_LIBS) $(SDL_LIBS)
