#include "syntheffect/synth/cmd_micro.h"

#include <SDL.h>

namespace syntheffect {
    namespace synth {
        CmdMicro::CmdMicro(cv::VideoCapture& second_vcap) {
            current_state_ = CMD_MICRO_STATE_DERIVATIVE;

            derivative_ = new module::Derivative();
            cats_ = new module::CatExplorer(second_vcap);
            color_tweak_ = new module::ColorTweak();
        }

        void CmdMicro::handleCatExplorer(CmdMicroEvent event, uint8_t value) {
            switch(event) {
                case CMD_MICRO_LEFT_PLAY_PAUSE:
                    cats_->toggle();
                    break;
                case CMD_MICRO_LEFT_TURN_TABLE_SPIN:
                    cats_->fadeWeight(value > 64);
                    break;
                case CMD_MICRO_RIGHT_TURN_TABLE_SPIN:
                    cats_->seek(value > 64 ? 2 : -2);
                    break;
                default:
                    break;
            }
        }

        void CmdMicro::handleDerivative(CmdMicroEvent event, uint8_t value) {
            switch(event) {
                case CMD_MICRO_LEFT_PLAY_PAUSE:
                    derivative_->toggle();
                    break;
                default:
                    break;
            }
        }

        void CmdMicro::update(const cv::Mat& in, cv::Mat& out) {
            color_tweak_->update(in, out);
            derivative_->update(out, out);
            cats_->update(out, out);
        }

        void CmdMicro::handleMidiEvent(MidiMessage& msg) {
            CmdMicroEvent event = convertEvent(msg);
            uint8_t value = msg.getValue();

            switch(event) {
                case CMD_MICRO_LEFT_ONE:
                    current_state_ = CMD_MICRO_STATE_DERIVATIVE;
                    break;
                case CMD_MICRO_LEFT_TWO:
                    current_state_ = CMD_MICRO_STATE_CAT_EXPLORER;
                    break;
                default:
                    break;
            }

            switch(current_state_) {
                case CMD_MICRO_STATE_CAT_EXPLORER:
                    handleCatExplorer(event, value);
                    return;
                case CMD_MICRO_STATE_DERIVATIVE:
                    handleDerivative(event, value);
                    return;
                default:
                    break;
            }
        } 

        CmdMicroEvent CmdMicro::convertEvent(MidiMessage& msg) {
            switch(msg.getType()) {
                case MIDI_TYPE_NOTE_ON:
                    switch (msg.getNote()) {
                        case 18:
                            return CMD_MICRO_LEFT_ONE;
                        case 19:
                            return CMD_MICRO_LEFT_TWO;
                        case 23:
                            return CMD_MICRO_LEFT_PLAY_PAUSE;
                        default:
                            SDL_Log("Unknow NOTE_ON %d", msg.getNote());
                            return CMD_MICRO_UNKNOWN;
                    }
                case MIDI_TYPE_CONTROL:
                    switch (msg.getFunction()) {
                        case 17:
                            return CMD_MICRO_LEFT_TURN_TABLE_SPIN;
                        case 33:
                            return CMD_MICRO_RIGHT_TURN_TABLE_SPIN;
                        default:
                            SDL_Log("Unknow CONTROL %d", msg.getNote());
                            return CMD_MICRO_UNKNOWN;
                    }
                default:
                    return CMD_MICRO_UNKNOWN;
            }
        }

    }
}
