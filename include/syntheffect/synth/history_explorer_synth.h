#include <opencv2/core/mat.hpp>

#include "syntheffect/synth.h"

namespace syntheffect {
    class HistoryExplorerSynth : Synth {
        public:
           HistoryExplorerSynth();
           void update(const cv::Mat& in, cv::Mat& out);
           void start();
           void stop();
           void setHistoryWeight(double param);
           void fadeHistoryWeight(bool up);
           std::string stringify();

        private:
           cv::Mat saved_frame_;
           cv::Mat last_frame_;
           bool active_;
           double history_weight_;
    };
};
