#include "syntheffect/module/cat_explorer.h"

#include <SDL.h>
#include <opencv2/opencv.hpp>
#include <boost/format.hpp>

namespace syntheffect {
    namespace module {
        // Maybe more performant with videos with b-frames
        // https://github.com/opencv/opencv/issues/4890
        CatExplorer::CatExplorer(cv::VideoCapture& vcap, cv::Size target_size) {
            double fps = vcap.get(cv::CAP_PROP_FPS);
            ms_delay_ = int((1 / fps) * 1000);

            current_pos_frames_ = 0;
            vcap_ = vcap;
            last_seek_ = -1;
            target_size_ = target_size;
            cv::Mat last_frame_;
        }

        void CatExplorer::seek(int32_t step_frames) {
            uint32_t max_frames = vcap_.get(cv::CAP_PROP_FRAME_COUNT);
            current_pos_frames_ = (current_pos_frames_ + step_frames) % max_frames;
            last_seek_ = SDL_GetTicks();
        }

        bool CatExplorer::shouldSeek() {
            if (last_seek_ < 0) {
                return false;
            }

            // Pause for 200ms before starting to play again.
            return SDL_GetTicks() - last_seek_ < 200;
        }

        bool CatExplorer::isNextFrameReady() {
            if (!active_) {
                return false;
            }

            unsigned int transpired = SDL_GetTicks() - last_update_;
            if (transpired > ms_delay_) {
                SDL_Log("transpired=%u, ms_delay_=%u", transpired, ms_delay_);
            }

            return last_update_ == 0 || transpired >= ms_delay_;
        }

        void CatExplorer::read(cv::Mat& out) {
            if (!active_) {
                last_frame_.copyTo(out);
                return;
            }

            last_update_ = SDL_GetTicks();

            if (shouldSeek()) {
                vcap_.set(CV_CAP_PROP_POS_FRAMES, current_pos_frames_);
            }

            if (!vcap_.read(out)) {
                vcap_.set(CV_CAP_PROP_POS_FRAMES, 0);
                if (!vcap_.read(out)) {
                    SDL_Log("ERROR: For some reason the video can not be read on frame 0");
                    // App may crash since out is empty.
                    return;
                }
            }

            cv::resize(out, out, target_size_, 0, 0, cv::INTER_CUBIC);
            out.copyTo(last_frame_);

            current_pos_frames_ = vcap_.get(CV_CAP_PROP_POS_FRAMES);
        }
    };
};
