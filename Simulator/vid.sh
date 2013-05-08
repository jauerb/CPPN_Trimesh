#!/bin/bash
if [ $1 ]; then
	ffmpeg -i /tmp/JoshA/frame/%04d.ppm -sameq $1
fi
