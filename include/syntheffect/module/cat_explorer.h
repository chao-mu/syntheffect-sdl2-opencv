#ifndef SYNTHEFFECT_MODULE_CAT_EXPLORER_H
#define SYNTHEFFECT_MODULE_CAT_EXPLORER_H

#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>

#include "syntheffect/module/module.h"
#include <cstdint>

namespace syntheffect {
    namespace module {
        class CatExplorer : public Module {
            public:
                CatExplorer(cv::VideoCapture& vcap, cv::Size target_size_);

                // Advance the video one frame or to saught frame.
                void read(cv::Mat& out);

                void seek(int32_t step_frames);
  
                // Whether or not we are ready for a read.
                bool isNextFrameReady();

            private:
                bool shouldSeek();
                int64_t last_seek_;
                double weight_;
                uint32_t current_pos_frames_;
                cv::Size target_size_;
                
                // The amount to wait between frames, at ideal maximum
                unsigned int ms_delay_;
                
                // The video we're reading
                cv::VideoCapture vcap_;

                // The last time we drew a frame
                unsigned int last_update_;

                // The last frame read, used for when read while paused
                cv::Mat last_frame_;
        };
    }
}
#endif
