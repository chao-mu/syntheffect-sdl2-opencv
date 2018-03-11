#include <opencv2/core/mat.hpp>

#include "syntheffect/synth.h"

#ifndef SYNTHEFFECT_SYNTH_DERIVATIVE_SYNTH
#define SYNTHEFFECT_SYNTH_DERIVATIVE_SYNTH
namespace syntheffect {
    class DerivativeSynth : Synth {
        public:
           DerivativeSynth();
           void update(const cv::Mat& in, cv::Mat& out);
           void setBlurSize(double param);
           std::string stringify();

        private:
           cv::Size blurSize;
    };
};
#endif
