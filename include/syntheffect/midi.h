#ifndef SYNTHEFFECT_MIDI_H
#define SYNTHEFFECT_MIDI_H

#include <vector>

namespace syntheffect {
    enum MidiType {
        MIDI_TYPE_UNKNOWN,
        MIDI_TYPE_NOTE_ON,
        MIDI_TYPE_NOTE_OFF,
        MIDI_TYPE_CONTROL
    };

    class MidiMessage {
        public:
            MidiMessage(std::vector<unsigned char> message);
            MidiType getType();
            unsigned char getVelocity();
            unsigned char getFunction();
            unsigned char getNote();
            unsigned char getValue();

        private:
            std::vector<unsigned char> rawMessage;
    };
};
#endif
