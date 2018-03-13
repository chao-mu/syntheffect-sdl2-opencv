#!/bin/sh

ffmpeg -i "$1" "$2" -x265-params b-frames=0 
