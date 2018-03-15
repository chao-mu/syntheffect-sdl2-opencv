#ifndef SYNTHEFFECT_SYNTH_CMD_MICRO_H
#define SYNTHEFFECT_SYNTH_CMD_MICRO_H

#include "opencv2/core/mat.hpp"
#include "opencv2/videoio.hpp"

#include "syntheffect/midi.h"
#include "syntheffect/module/cat_explorer.h"
#include "syntheffect/module/derivative.h"
#include "syntheffect/module/feedback.h"
#include "syntheffect/module/color_tweak.h"
#include "syntheffect/module/tracker.h"
#include "syntheffect/module/merger.h"

namespace syntheffect {
    namespace synth {
        enum CmdMicroState {
            CMD_MICRO_STATE_SECONDARY_VIDEO,
            CMD_MICRO_STATE_PRIMARY_VIDEO,
            CMD_MICRO_STATE_PRIMARY_EFFECTS,
            CMD_MICRO_STATE_SECONDARY_EFFECTS,
        };

        enum CmdMicroEvent {
            CMD_MICRO_UNKNOWN,
            CMD_MICRO_LEFT_TURN_TABLE_SPIN,
            CMD_MICRO_RIGHT_TURN_TABLE_SPIN,
            CMD_MICRO_LEFT_PLAY_PAUSE,
            CMD_MICRO_LEFT_ONE,
            CMD_MICRO_LEFT_TWO,
            CMD_MICRO_RIGHT_ONE,
            CMD_MICRO_RIGHT_TWO,
            CMD_MICRO_LEFT_LEFT_FADER,
        };

        class CmdMicro {
            public:
                CmdMicro(cv::VideoCapture& primary_vid, cv::VideoCapture& secondary_vid, cv::Size target_size);
                void handleMidiEvent(MidiMessage& msg);
                bool read(cv::Mat& out);

            private:
                void handlePrimaryVideo(CmdMicroEvent event, uint8_t value);
                void handleSecondaryVideo(CmdMicroEvent event, uint8_t value);
                void handlePrimaryEffects(CmdMicroEvent event, uint8_t value);
                void handleSecondaryEffects(CmdMicroEvent event, uint8_t value);

                CmdMicroEvent convertEvent(MidiMessage& msg);

                CmdMicroState current_state_;

                module::CatExplorer* primary_vid_;
                module::CatExplorer* secondary_vid_;

                module::Derivative* derivative_;
                module::ColorTweak* color_tweak_;
                module::Feedback* feedback_;
                module::Tracker* tracker_;
                module::Merger* merger_;
        };
    }
}
#endif
