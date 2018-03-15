#ifndef SYNTHEFFECT_APP_H
#define SYNTHEFFECT_APP_H

#include "RtMidi.h"
#include <SDL.h>
#include <opencv2/videoio.hpp>
#include <vector>

#include "syntheffect/module/writer.h"
#include "syntheffect/midi.h"
#include "syntheffect/synth/cmd_micro.h"

namespace syntheffect {
    class App {
        public:
            App(cv::VideoCapture& vcap, cv::VideoCapture& vcap_secondary, SDL_Window* window);
            void loop(RtMidiIn* midi_in);

        private:
            void close();
            void update();
            void stop();

            void handleEvent(SDL_Event event);
            void handleMidiEvent(MidiMessage msg);

            //The window we'll be rendering to
            SDL_Window* window_;

            //The surface contained by the window
            SDL_Surface* screen_surface_;

            // The surface onto which we'll display our frame
            SDL_Surface* frame_surface_;

            //The image we will load and show on the screen

            // Whether or not we should exit the main loop.
            bool stopped_;

            // snapshot takes a picture and writes it to an external file.
            void snapshot();

            // writer_ is a module for outputing a recording of the magic synth_ will create
            module::Writer* writer_;

            // synth_ provides main effect processing and midi event handling.
            synth::CmdMicro* synth_;

            // frame_idx_ is the number of the current frame.
            unsigned int frame_idx_;

            // frame_ is the last read frame from our video.
            cv::Mat frame_; 
    };
};
#endif
