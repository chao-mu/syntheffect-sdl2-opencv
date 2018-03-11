#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>

#include "syntheffect/synth.h"
#include <cstdint>

namespace syntheffect {
    class CatExplorerSynth : Synth {
        public:
           CatExplorerSynth(cv::VideoCapture& vcap);
           void update(const cv::Mat& in, cv::Mat& out);
           void start();
           void stop();
           void setWeight(double param);
           void fadeWeight(bool up);
           void seek(uint32_t step_frames);
           std::string stringify();

        private:
           bool active_;
           bool shouldSeek();
           int64_t last_seek_;
           double weight_;
           uint32_t current_pos_frames_;
           cv::VideoCapture vcap_;
    };
};
