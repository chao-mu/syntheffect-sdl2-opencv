#ifndef SYNTHEFFECT_SYNTH_CMD_MICRO_H
#define SYNTHEFFECT_SYNTH_CMD_MICRO_H

#include "opencv2/core/mat.hpp"
#include "opencv2/videoio.hpp"

#include "syntheffect/midi.h"
#include "syntheffect/module/cat_explorer.h"
#include "syntheffect/module/derivative.h"
#include "syntheffect/module/color_tweak.h"

namespace syntheffect {
    namespace synth {
        enum CmdMicroState {
            CMD_MICRO_STATE_CAT_EXPLORER,
            CMD_MICRO_STATE_DERIVATIVE
        };

        enum CmdMicroEvent {
            CMD_MICRO_UNKNOWN,
            CMD_MICRO_LEFT_TURN_TABLE_SPIN,
            CMD_MICRO_RIGHT_TURN_TABLE_SPIN,
            CMD_MICRO_LEFT_PLAY_PAUSE,
            CMD_MICRO_LEFT_ONE,
            CMD_MICRO_LEFT_TWO
        };

        class CmdMicro {
            public:
                CmdMicro(cv::VideoCapture& second_vcap);
                void handleMidiEvent(MidiMessage& msg);
                void update(const cv::Mat& in, cv::Mat& out);

            private:
                void handleCatExplorer(CmdMicroEvent event, uint8_t value);
                CmdMicroEvent convertEvent(MidiMessage& msg);

                CmdMicroState current_state_;

                module::Derivative* derivative_;
                module::CatExplorer* cats_;
                module::ColorTweak* color_tweak_;
        };
    }
}
#endif
