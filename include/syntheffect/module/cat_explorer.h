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
                CatExplorer(cv::VideoCapture& vcap);
                void update(const cv::Mat& in, cv::Mat& out);
                void setWeight(double param);
                void fadeWeight(bool up);
                void seek(uint32_t step_frames);
                std::string stringify();

            private:
                bool shouldSeek();
                int64_t last_seek_;
                double weight_;
                uint32_t current_pos_frames_;
                cv::VideoCapture vcap_;
        };
    }
}
#endif
