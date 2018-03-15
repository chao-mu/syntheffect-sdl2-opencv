#include "syntheffect/app.h"

#include <math.h>
#include <iostream>

#include <boost/format.hpp>
#include <SDL.h>
#include <opencv2/opencv.hpp>

#include "syntheffect/midi.h"

namespace syntheffect {
    App::App(cv::VideoCapture& vcap_primary, cv::VideoCapture& vcap_secondary, SDL_Window* window) {
        //Get window surface
        screen_surface_ = SDL_GetWindowSurface(window);
        
        window_ = window;

        stopped_ = false;
        frame_idx_ = 0;

        frame_surface_ = NULL;

        int vid_width = int(round(vcap_primary.get(cv::CAP_PROP_FRAME_WIDTH)));
        int vid_height = int(round(vcap_primary.get(cv::CAP_PROP_FRAME_HEIGHT))); 
        double fps = vcap_primary.get(cv::CAP_PROP_FPS);

        writer_ = new module::Writer("out.avi", vid_width, vid_height, fps);
        synth_ = new synth::CmdMicro(vcap_primary, vcap_secondary, cv::Size(vid_width, vid_height));
    }

    void App::loop(RtMidiIn* midi_in) {
        std::vector<unsigned char> raw_message;
        SDL_Event event;

        while (true) {
            update();
            if (stopped_) {
                break;
            }

            while (SDL_PollEvent(&event) != 0) {
                handleEvent(event);
            }
            if (stopped_) {
                break;
            }

            while (midi_in->getMessage(&raw_message) > 0) {
                MidiMessage msg(raw_message);
                synth_->handleMidiEvent(msg);
            }
            if (stopped_) {
                break;
            }
        }

        close();
    }

    void App::stop() {
        stopped_ = true;
    }

    void App::snapshot() {
        std::string filename = boost::str(boost::format("out-%d.jpg") % frame_idx_);
        cv::imwrite(filename, frame_);
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
                }
        }
    }

    void App::update() {
        bool ready = synth_->read(frame_);
        if (!ready) {
            return;
        }

        writer_->update(frame_, frame_);

        if (frame_surface_ != NULL) {
            SDL_FreeSurface(frame_surface_);
        }

        frame_idx_++;

        frame_surface_ = SDL_CreateRGBSurfaceFrom(
                frame_.data,
                frame_.cols,
                frame_.rows,
                24,
                frame_.cols * 3,
                0xff0000, 0x00ff00, 0x0000ff, 0);

        if (frame_surface_ == NULL) {
            SDL_Log("Couldn't convert Mat to Surface. %s\n", SDL_GetError());
        }

        // Calculate upper left of middle of the scrreen
        SDL_Rect target_rect;
        double xcenter = screen_surface_->w / 2;
        double ycenter = screen_surface_->h / 2; 
        target_rect.w = frame_surface_->w;
        target_rect.h = frame_surface_->h;
        target_rect.x = int(round(xcenter - (frame_surface_->w/2)));
        target_rect.y = int(round(ycenter - (frame_surface_->h/2)));

        SDL_BlitSurface(frame_surface_, NULL, screen_surface_, &target_rect);

        SDL_UpdateWindowSurface(window_);
    }

    void App::close() {
        if (frame_surface_ != NULL) {
            SDL_FreeSurface(frame_surface_);
        }

        //Destroy window
        SDL_DestroyWindow(window_);

        //Quit SDL subsystems
        SDL_Quit();

        writer_->release();
    }
};
