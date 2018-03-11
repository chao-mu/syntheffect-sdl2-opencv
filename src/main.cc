#include "RtMidi.h"
#include <iostream>

#include "syntheffect/app.h"
#include "syntheffect/midi.h"

const char* window_name = "window";

int main(int argc, char **argv)
{
    syntheffect::App* myApp = new syntheffect::App();
    int status = myApp->setup(argc, argv);
    if (status != 0) {
        return status;
    }

    // http://www.music.mcgill.ca/~gary/rtmidi/
    RtMidiIn *midiin = new RtMidiIn();
    // Check available ports.
    unsigned int nPorts = midiin->getPortCount();
    if (nPorts == 0) {
	std::cout << "No MIDI ports available!\n";
	delete midiin;
	return 1;
    }
    midiin->openPort(0);

    std::vector<unsigned char> rawMessage;
    SDL_Event event;
    while (true) {
        myApp->update();
        if (myApp->stopped) {
            break;
        }

        while (SDL_PollEvent(&event) != 0) {
            myApp->handleEvent(event);
        }
        if (myApp->stopped) {
            break;
        }

	while (midiin->getMessage(&rawMessage) > 0) {
            syntheffect::MidiMessage msg(rawMessage);
            myApp->handleMidiEvent(msg);
	}
        if (myApp->stopped) {
            break;
        }
    }

    delete midiin;
    myApp->close();

    return 0;
}
