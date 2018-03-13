#include "RtMidi.h"
#include <iostream>

#include "syntheffect/app.h"
#include "syntheffect/midi.h"

const char* window_name = "window";

int main(int argc, char **argv)
{
    if (argc != 3) {
        printf("Usage: %s syntheffect <path to video> <path to cat related video>\n", argv[0]);
        return 1;
    }
    const char* filename = argv[1];
    const char* filename_cats = argv[2];

    cv::VideoCapture vcap_cats = cv::VideoCapture(filename_cats);

    auto vcap = cv::VideoCapture(filename);

    // http://www.music.mcgill.ca/~gary/rtmidi/
    RtMidiIn *midi_in = new RtMidiIn();
    // Check available ports.
    unsigned int nPorts = midi_in->getPortCount();
    if (nPorts == 0) {
	std::cout << "No MIDI ports available!\n";
	delete midi_in;
	return 1;
    }
    midi_in->openPort(0);


    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    int vid_width = int(round(vcap.get(cv::CAP_PROP_FRAME_WIDTH)));
    int vid_height = int(round(vcap.get(cv::CAP_PROP_FRAME_HEIGHT))); 

    //Create window
    SDL_Window* window = SDL_CreateWindow("syntheffect", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, vid_width, vid_height, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    syntheffect::App* app = new syntheffect::App(vcap, vcap_cats, window);
    app->loop(midi_in);

    delete midi_in;

    return 0;
}
