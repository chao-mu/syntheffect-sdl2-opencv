#ifndef SYNTHEFFECT_APP_H
#define SYNTHEFFECT_APP_H

#include <SDL.h>
#include <opencv2/videoio.hpp>
#include <vector>

#include "syntheffect/synth/history_explorer_synth.h"
#include "syntheffect/synth/cat_explorer_synth.h"
#include "syntheffect/synth/derivative_synth.h"
#include "syntheffect/midi.h"

namespace syntheffect {
    class App {
        public:
            int setup(int argc, char **argv);
            void close();
            void update();
            void stop();
            bool stopped;
            void handleEvent(SDL_Event event);
            void handleMidiEvent(MidiMessage& msg);

        private:
            //The window we'll be rendering to
            SDL_Window* window;

            //The surface contained by the window
            SDL_Surface* screenSurface;

            //The image we will load and show on the screen
            SDL_Surface* frameSurface;

            cv::VideoCapture vcap;

            unsigned int msDelay;
            unsigned int lastUpdate;
            int vidWidth;
            int vidHeight;
            bool isNextFrameReady();
            void snapshot();

            DerivativeSynth* derivative_;
            HistoryExplorerSynth* historical_;
            CatExplorerSynth* cats_;
            unsigned int frameIdx;
            cv::Mat frame; 
    };
};
#endif
