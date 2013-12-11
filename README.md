#RPCam (Raspberry Pi Camere)

[Issues](https://github.com/arabyoru/RPCam/issues)

[dgkim5212@gmail.com](dgkim5212@gmail.com)

## License
This program follow the GPLv2 license

## Version
This documatation is for the unreleased development branch. The current stable version is not yet.

## Description
This program is the Raspberry Pi using a V4L2 camera control program.

Implementation of goals

 1. Save JPG images using v4l2
 
 2. JPG data network transmission
 
 3. External event detection and storage and file management

 In development now..

## Development Environment
Raspberry Pi 2.0 Board 
Linux version 3.10.19
gcc (Debian 4.6.3-14+rpi1) 4.6.3
V4L2 Library

## Feature
In order to use a camera device RPCam enable the operation must be preceded.
Also to be installed V4L2(Video4Linux2)

V4L2 Install

	pi@rasplay~$ sudo apt-get install uv4l uv4l-raspicam

V4L2 Device Enable

	pi@rasplay~$ uv4l --driver raspicam --auto-video_nr --width 640 --height 480 --encoding jpeg







