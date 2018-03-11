#ifndef SYNTHEFFECT_SYNTH_DUEL_COLOR_MAP_SYNTH
#define SYNTHEFFECT_SYNTH_DUEL_COLOR_MAP_SYNTH

#include <opencv2/core/mat.hpp>

#include "syntheffect/synth.h"

namespace syntheffect {
    class DuelColorMapSynth : Synth {
        public:
           DuelColorMapSynth();
           void update(const cv::Mat& in, cv::Mat& out);
    };
}
#endif
