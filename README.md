# BeepPlayer

BeepPlayer is a frequency generator which converts text files to sounds.
It is written in C++ and uses only the Win32 API and STL.

## Getting Started

These instructions will get you a copy of the project up and running on your 
local machine for development and testing purposes.

### Prerequisites
* Git
* a text editor of your choice
* MinGW (per example [this distro](https://nuwen.net/mingw.html))

### How to start the project
* clone this repo
* navigate your terminal to the scripts folder
* run the build script

## How to create music

To create your own tracks you have to understand how the file format works.
This little tutorial will explain you everything you need to know.

* a music track is a simple text file
* every part of your track needs a title
* every tone you want to create consists of three parts: 
note, duration, sleep time

### What number is which note

Here is the tricky part of the process. Notes are encoded as numbers.

* the base note is A'', which is encoded as 0
* if you go the scale one step up you have up  an A#'' or Bb'' 
(B decreased by a halftone)
    - this note is encoded as 1
* if you go the scale one step down you have an G#' or Ab' 
(A decreased by a halftone)
    - this note is encoded as -1

According to this little explanation the scale has the following scheme:

    A-A#-B-C-C#-D-D#-E-F-F#-G-G#

### So how does a complete sound file looks out

The application itself contains some examples in the bin/examples folder.
Since these are relatively complex examples, here's a simpler one: 
the scale itself.

    ## Scale
     0 100 100
     1 100 100
     2 100 100
     3 100 100
     4 100 100
     5 100 100
     6 100 100
     7 100 100
     8 100 100
     9 100 100
    10 100 100
    11 100 100
