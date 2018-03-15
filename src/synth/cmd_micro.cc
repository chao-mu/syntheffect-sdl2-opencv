#include "syntheffect/synth/cmd_micro.h"

#include <SDL.h>

namespace syntheffect {
    namespace synth {
        CmdMicro::CmdMicro(cv::VideoCapture& primary_vid, cv::VideoCapture& secondary_vid, cv::Size target_size) {
            current_state_ = CMD_MICRO_STATE_PRIMARY_VIDEO;

            primary_vid_ = new module::CatExplorer(primary_vid, target_size);
            secondary_vid_ = new module::CatExplorer(secondary_vid, target_size);

            derivative_ = new module::Derivative();
            color_tweak_ = new module::ColorTweak();
            feedback_ = new module::Feedback();
            tracker_ = new module::Tracker();
            merger_ = new module::Merger();

            feedback_->start();
            //color_tweak_->start();
            //tracker_->start();
        }

        void CmdMicro::handlePrimaryVideo(CmdMicroEvent event, uint8_t value) {
            switch(event) {
                case CMD_MICRO_LEFT_PLAY_PAUSE:
                    primary_vid_->toggle();
                    break;
                case CMD_MICRO_LEFT_TURN_TABLE_SPIN:
                    merger_->fadeWeight(value < 64);
                    break;
                case CMD_MICRO_RIGHT_TURN_TABLE_SPIN:
                    primary_vid_->seek(value > 64 ? 2 : -2);
                    break;
                default:
                    break;
            }
        }

        void CmdMicro::handleSecondaryVideo(CmdMicroEvent event, uint8_t value) {
            switch(event) {
                case CMD_MICRO_LEFT_PLAY_PAUSE:
                    secondary_vid_->toggle();
                    break;
                case CMD_MICRO_LEFT_TURN_TABLE_SPIN:
                    merger_->fadeWeight(value > 64);
                    break;
                case CMD_MICRO_RIGHT_TURN_TABLE_SPIN:
                    secondary_vid_->seek(value > 64 ? 2 : -2);
                    break;
                default:
                    break;
            }
        }

        void CmdMicro::handlePrimaryEffects(CmdMicroEvent event, uint8_t value) {
            switch(event) {
                case CMD_MICRO_LEFT_PLAY_PAUSE:
                    color_tweak_->toggle();
                    break;
                default:
                    break;
            }
        }

        void CmdMicro::handleSecondaryEffects(CmdMicroEvent event, uint8_t value) {
            switch(event) {
                case CMD_MICRO_LEFT_PLAY_PAUSE:
                    derivative_->toggle();
                    break;
                case CMD_MICRO_LEFT_LEFT_FADER:
                    derivative_->setBlurSize(value, 0, 127);
                default:
                    break;
            }
        }

        bool CmdMicro::read(cv::Mat& out) {
            if (!primary_vid_->isNextFrameReady() && !secondary_vid_->isNextFrameReady()) {
                return false;
            }

            cv::Mat primary_frame;
            primary_vid_->read(primary_frame);

            cv::Mat secondary_frame;
            secondary_vid_->read(secondary_frame);

            if (!primary_frame.empty() && !secondary_frame.empty()) {
                merger_->update(primary_frame, secondary_frame, out);
            } else if (!primary_frame.empty()) {
                primary_frame.copyTo(out);
            } else if (!secondary_frame.empty()) {
                secondary_frame.copyTo(out);
            } else {
                SDL_Log("primary or seondary was ready, but only empty frames were read");
                return false;
            }

            feedback_->update(out, out);
            color_tweak_->update(out, out);
            derivative_->update(out, out);

            return true;
        }

        void CmdMicro::handleMidiEvent(MidiMessage& msg) {
            CmdMicroEvent event = convertEvent(msg);
            uint8_t value = msg.getValue();

            switch(event) {
                case CMD_MICRO_LEFT_ONE:
                    current_state_ = CMD_MICRO_STATE_PRIMARY_VIDEO;
                    break;
                case CMD_MICRO_LEFT_TWO:
                    current_state_ = CMD_MICRO_STATE_SECONDARY_VIDEO;
                    break;
                case CMD_MICRO_RIGHT_ONE:
                    current_state_ = CMD_MICRO_STATE_PRIMARY_EFFECTS;
                    break;
                case CMD_MICRO_RIGHT_TWO:
                    current_state_ = CMD_MICRO_STATE_SECONDARY_EFFECTS;
                    break;
                default:
                    break;
            }

            switch(current_state_) {
                case CMD_MICRO_STATE_PRIMARY_VIDEO:
                    handlePrimaryVideo(event, value);
                    return;
                case CMD_MICRO_STATE_SECONDARY_VIDEO:
                    handleSecondaryVideo(event, value);
                    return;
                case CMD_MICRO_STATE_PRIMARY_EFFECTS:
                    handlePrimaryEffects(event, value);
                    return;
                case CMD_MICRO_STATE_SECONDARY_EFFECTS:
                    handleSecondaryEffects(event, value);
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
                        case 34:
                            return CMD_MICRO_RIGHT_ONE;
                        case 36:
                            return CMD_MICRO_RIGHT_TWO;
                        default:
                            SDL_Log("Unknow NOTE_ON %d", msg.getNote());
                            return CMD_MICRO_UNKNOWN;
                    }
                case MIDI_TYPE_CONTROL:
                    switch (msg.getFunction()) {
                        case 16:
                            return CMD_MICRO_LEFT_LEFT_FADER;
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
