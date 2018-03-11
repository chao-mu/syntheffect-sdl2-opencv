![Alt Text](http://www.capriogroup.com/WebStuff/Images/Borders/Animated-Border-WelcomeOnClassyMarquee.gif)

# Synth Effect

Synth Effect is a visual synthesizer written for my own VJing purposes. It's a stack of SDL2, OpenCV, and RTMIDI.
At this point I am not expecting others to use it.

![Alt Text](https://cdn.business2community.com/wp-content/uploads/2016/04/image.gif.gif)

![Alt Text](http://www.capriogroup.com/WebStuff/Images/Borders/Animated-Border-Fire.gif)
## Installation

After installing dependencies (sdl2, opencv 3, and rtmidi):

```
$ ./configure
$ make 
$ make install
```

### Dependencies on Ubuntu

This project is developed and run on MacOS, but in setting up to work with Travis it had to also at least compile on Ubuntu:

```
$ sudo apt-get install -y librtmidi-dev libsdl2-dev
```

Installing opencv >= 3.4.1 is an exercise left to the reader.

![Alt Text](http://www.capriogroup.com/WebStuff/Images/Borders/Animated-Border-Fire.gif)
## Contributing

Please follow the Google styleguide https://google.github.io/styleguide/cppguide.html

If existing code does not follow that guide, then it should be considered a bug.

![Alt Text](http://www.capriogroup.com/WebStuff/Images/Borders/Animated-Border-Fire.gif)
## Personal Notes

To make on MacOS:

```
PKG_CONFIG_PATH=/usr/local/lib/pkgconfig make
```

Or just set that variable so you don't have to keep including it to make.

![Alt Text](http://www.capriogroup.com/WebStuff/Images/Borders/Animated-Border-Fire.gif)
## Status
[![Build Status](https://travis-ci.org/chao-mu/syntheffect.png)](https://travis-ci.org/chao-mu/syntheffect)

![Alt Text](http://www.capriogroup.com/WebStuff/Images/Borders/Animated-Border-GreenLineInBlack.gif)
