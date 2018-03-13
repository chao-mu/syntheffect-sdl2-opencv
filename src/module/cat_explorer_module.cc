#include "syntheffect/module/cat_explorer_module.h"

#include <SDL.h>
#include <opencv2/opencv.hpp>
#include <boost/format.hpp>

namespace syntheffect {
    // Maybe more performant with videos with b-frames
    // https://github.com/opencv/opencv/issues/4890
    CatExplorerModule::CatExplorerModule(cv::VideoCapture& vcap) {
        weight_ = 0.5;
        current_pos_frames_ = 0;
        vcap_ = vcap;
        last_seek_ = -1;
    }

    void CatExplorerModule::setWeight(double param) {
        weight_ = param;
    }

    void CatExplorerModule::seek(uint32_t step_frames) {
        uint32_t max_frames = vcap_.get(cv::CAP_PROP_FRAME_COUNT);
        current_pos_frames_ = (current_pos_frames_ + step_frames) % (max_frames + 1);
        last_seek_ = SDL_GetTicks();
    }

    bool CatExplorerModule::shouldSeek() {
        if (last_seek_ < 0) {
            return false;
        }

        // Pause for 200ms before starting to play again.
        return SDL_GetTicks() - last_seek_ < 200;
    }

    void CatExplorerModule::update(const cv::Mat& in, cv::Mat& out) {
        if (active_) {
            cv::Mat frame;
            vcap_ >> frame;
            if (shouldSeek()) {
                vcap_.set(CV_CAP_PROP_POS_FRAMES, current_pos_frames_);
            } else {
                current_pos_frames_++;
            }

            if (current_pos_frames_ > int(round(vcap_.get(cv::CAP_PROP_FRAME_COUNT)))) {
                current_pos_frames_ = 0;
            }

            cv::resize(frame, frame, in.size(), 0, 0, cv::INTER_CUBIC);
            cv::addWeighted(in, (1 - weight_), frame, weight_, 0, out);
        } else {
            in.copyTo(out);
        }
    }

    void CatExplorerModule::fadeWeight(bool up) {
        weight_ = fadeParam(weight_, up, 0.05);
    }

    std::string CatExplorerModule::stringify() {
        return str(boost::format("CatExplorerModule: weight=%1%") % weight_);
    }
};
