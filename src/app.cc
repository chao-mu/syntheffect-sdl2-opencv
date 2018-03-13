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
        double fps = vcap.get(cv::CAP_PROP_FPS);
        msDelay = int((1 / fps) * 1000);

        if(SDL_Init(SDL_INIT_VIDEO) < 0) {
            printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
            return 1;
        }

        //Create window
        window = SDL_CreateWindow("syntheffect", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, vidWidth, vidHeight, SDL_WINDOW_SHOWN);
        if (window == NULL) {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            return 1;
        }

        //Get window surface
        screenSurface = SDL_GetWindowSurface(window);

        lastUpdate = 0;
        stopped = false;
        frameIdx = 0;

        current_state_ = APP_STATE_DERIVATIVE_ACTIVE;
        derivative_ = new module::Derivative();
        historical_ = new module::HistoryExplorer();
        cats_ = new module::CatExplorer(vcapCats);
        color_tweak_ = new module::ColorTweak();
        writer_ = new module::Writer("out.avi", vidWidth, vidHeight, fps);

        color_tweak_->start();
        derivative_->start();
        writer_->start();

        return 0;
    }

    void App::stop() {
        stopped = true;
    }

    void App::snapshot() {
        std::string filename = boost::str(boost::format("out-%d.jpg") % frameIdx);
        cv::imwrite(filename, frame_);
    }

    void App::handleCatExplorerActive(MidiMessage& msg) {
        switch(msg.getType()) {
            case MIDI_TYPE_NOTE_ON:
                switch (msg.getNote()) {
                    // Play/pause
                    case 23:
                        cats_->toggle();
                        return;
                    default:
                        return;
                }
            case MIDI_TYPE_CONTROL:
                switch (msg.getFunction()) {
                    // Left turn table
                    case 17:
                        cats_->fadeWeight(msg.getValue() > 64);
                        return;
                    // Right turn table
                    case 33:
                        cats_->seek(msg.getValue() > 64 ? 5 : -5);
                        return;
                    default:
                        return;
                }
            default:
                return;
        }
    }

    void App::handleMidiEvent(MidiMessage& msg) {
        switch(msg.getType()) {
            case MIDI_TYPE_NOTE_ON:
                switch (msg.getNote()) {
                    case 18:
                        current_state_ = APP_STATE_DERIVATIVE_ACTIVE;
                        return;
                    case 19:
                        current_state_ = APP_STATE_CAT_EXPLORER_ACTIVE;
                        return;
                    default:
                        break;
                }
            default:
                break;
        }

        switch(current_state_) {
            case APP_STATE_CAT_EXPLORER_ACTIVE:
                handleCatExplorerActive(msg);
                return;
            default:
                break;
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

            bool readSuccess = vcap.read(frame_);
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
            //cats_->update(frame, frame);
            color_tweak_->update(frame_, frame_);
            derivative_->update(frame_, frame_);
            writer_->update(frame_, frame_);

            frameSurface = SDL_CreateRGBSurfaceFrom(
                    frame_.data,
                    frame_.cols,
                    frame_.rows,
                    24,
                    frame_.cols * 3,
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
        writer_->release();
    }
};
