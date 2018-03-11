#include "syntheffect/app.h"

#include <math.h>
#include <iostream>

#include <boost/format.hpp>
#include <SDL.h>
#include <opencv2/opencv.hpp>

#include "syntheffect/midi.h"

namespace syntheffect {
    int App::setup(int argc, char **argv) {
        if (argc != 3) {
            printf("Usage: %s syntheffect <path to video> <path to cat related video>\n", argv[0]);
            return 1;
        }
        const char* fileName = argv[1];
        const char* fileNameCats = argv[2];

        cv::VideoCapture vcapCats = cv::VideoCapture(fileNameCats);

        vcap = cv::VideoCapture(fileName);
        vidWidth = int(round(vcap.get(cv::CAP_PROP_FRAME_WIDTH)));
        vidHeight = int(round(vcap.get(cv::CAP_PROP_FRAME_HEIGHT))); 
        msDelay = int((1 / vcap.get(cv::CAP_PROP_FPS)) * 1000);

        if(SDL_Init(SDL_INIT_VIDEO) < 0) {
            printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
            return 1;
        }

        //Create window
        window = SDL_CreateWindow("SynthEffect", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, vidWidth, vidHeight, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            return 1;
        }

        //Get window surface
        screenSurface = SDL_GetWindowSurface(window);

        lastUpdate = 0;
        stopped = false;
        frameIdx = 0;

        derivative_ = new DerivativeSynth();
        historical_ = new HistoryExplorerSynth();
        cats_ = new CatExplorerSynth(vcapCats);


        return 0;
    }

    void App::stop() {
        stopped = true;
    }

    void App::snapshot() {
        std::string filename = boost::str(boost::format("out-%d.jpg") % frameIdx);
        cv::imwrite(filename, frame);
    }

    void App::handleMidiEvent(MidiMessage& msg) {
        switch(msg.getType()) {
            case MIDI_TYPE_NOTE_ON:
                SDL_Log("Note on! note=%d vel=%d", msg.getNote(), msg.getVelocity());
                switch (msg.getNote()) {
                    // Left turn table touch start
                    case 1:
                        cats_->setWeight(0.5);
                        cats_->start();
                    default:
                        return;
                }
            case MIDI_TYPE_NOTE_OFF:
                switch (msg.getNote()) {
                    // Left turn table touch stop
                    case 1:
                        cats_->stop();
                    default:
                        return;
                }
            case MIDI_TYPE_CONTROL:
                switch (msg.getNote()) {
                    // Left turn table
                    case 17:
                        cats_->fadeWeight(msg.getVelocity() > 64);
                        return;
                    // Right turn table
                    case 33:
                        cats_->seek(msg.getVelocity() > 64 ? 5 : -5);
                        return;
                }

                SDL_Log("Control! %d, %d", msg.getFunction(), msg.getValue());
                return;
            case MIDI_TYPE_UNKNOWN:
                SDL_Log("Midi unknown!");
                return;
        }
    } 

    void App::handleEvent(SDL_Event event) {
        switch(event.type) {
            case SDL_QUIT:
                stop();
                return;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym) {
                    case SDLK_s:
                        snapshot();
                        return;
                    case SDLK_h:
                        cats_->start();
                        return;
                }
            case SDL_KEYUP:
                switch(event.key.keysym.sym) {
                    case SDLK_h:
                        cats_->stop();
                        return;
                }
        }
    }

    bool App::isNextFrameReady() {
        unsigned int transpired = SDL_GetTicks() - lastUpdate;
        if (transpired > msDelay) {
            SDL_Log("transpired=%u, msDelay=%u", transpired, msDelay);
        }
        return lastUpdate == 0 || transpired >= msDelay;
    }

    void App::update() {
        if (isNextFrameReady()) {
            lastUpdate = SDL_GetTicks();

            bool readSuccess = vcap.read(frame);
            if (!readSuccess) {
                SDL_Log("Video finished");
                stop();
                return;
            }

            if (frameSurface != NULL) {
                SDL_FreeSurface(frameSurface);
            }

            frameIdx++;

            //historical_->update(frame, frame);
            cats_->update(frame, frame);
            derivative_->update(frame, frame);

            frameSurface = SDL_CreateRGBSurfaceFrom(
                    frame.data,
                    frame.cols,
                    frame.rows,
                    24,
                    frame.cols * 3,
                    0xff0000, 0x00ff00, 0x0000ff, 0);

            if (frameSurface == NULL) {
                SDL_Log("Couldn't convert Mat to Surface. %s\n", SDL_GetError());
            }

            // Calculate upper left of middle of the scrreen
            SDL_Rect targetRect;
            double xcenter = screenSurface->w / 2;
            double ycenter = screenSurface->h / 2; 
            targetRect.w = frameSurface->w;
            targetRect.h = frameSurface->h;
            targetRect.x = int(round(xcenter - (frameSurface->w/2)));
            targetRect.y = int(round(ycenter - (frameSurface->h/2)));

            SDL_BlitSurface(frameSurface, NULL, screenSurface, &targetRect);

            SDL_UpdateWindowSurface(window);
        }
    }

    void App::close() {
        if (frameSurface != NULL) {
            SDL_FreeSurface(frameSurface);
        }

        //Destroy window
        SDL_DestroyWindow(window);

        //Quit SDL subsystems
        SDL_Quit();

        vcap.release();
    }
};
