MIR.EDU
=======

Written and maintained by: *Justin Salamon <justin.salamon@nyu.edu>*

MIR.EDU is an open source vamp plug-in library written in C++ which implements a basic set of 
descriptors useful for teaching MIR. The idea is to provide a simple library with clear and well 
documented code for learning about audio descriptors (RMS, log attack-time, spectral flux, etc.).

MIR.EDU is:
- open source
- simple to understand
- simple to build / install
- easy to visualise using Sonic Visualizer
- east to use for batch processing using Sonic Annotator

MIR.EDU is not:
- highly optimized
- comprehensive
- state of the art

Build
=====

Currently only instructions buildin under OSX are available. Linux and windows instructions will follow.

1. Clone the repository using git:
justin$ git clone https://github.com/justinsalamon/miredu.git

2. Enter the miredu folder:
justin$ cd miredu

3. Build the library using make
justin$ make


Installation
============

1. Move the compiled library to your Vamp plug-in folder (you may have to create the Vamp folder is it doesn't exist yet):
justin$ mv mir-edu.dylib /Library/Audio/Plug-ins/Vamp/mir-edu.dylib