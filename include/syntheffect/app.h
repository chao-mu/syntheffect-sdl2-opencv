#ifndef SYNTHEFFECT_APP_H
#define SYNTHEFFECT_APP_H

#include <SDL.h>
#include <opencv2/videoio.hpp>
#include <vector>

#include "syntheffect/module/history_explorer_module.h"
#include "syntheffect/module/cat_explorer_module.h"
#include "syntheffect/module/derivative_module.h"
#include "syntheffect/module/color_tweak_module.h"
#include "syntheffect/module/writer_module.h"
#include "syntheffect/midi.h"

namespace syntheffect {
    enum AppState {
        APP_STATE_CAT_EXPLORER_ACTIVE,
        APP_STATE_DERIVATIVE_ACTIVE
    };

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
            void handleCatExplorerActive(MidiMessage& msg);

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

            HistoryExplorerModule* historical_;
            DerivativeModule* derivative_;
            CatExplorerModule* cats_;
            ColorTweakModule* color_tweak_;
            WriterModule* writer_;

            unsigned int frameIdx;
            cv::Mat frame_; 
            AppState current_state_;
    };
};
#endif
