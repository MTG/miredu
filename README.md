MIR.EDU
=======

Written and maintained by: Justin Salamon *<justin.salamon@nyu.edu>*
*<www.justinsalamon.com>*

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

Currently only instructions for building under OSX are available. Linux and windows instructions will follow.

OSX

1. Clone the repository using git:
justin$ git clone https://github.com/justinsalamon/miredu.git

2. Enter the miredu folder:
justin$ cd miredu

3. Build the library using make
justin$ make


Installation
============

OSX

If you built the library yourself:

1. Move the compiled library to your 'Vamp' plug-ins folder (you may have to create the Vamp folder is it doesn't exist yet):
justin$ mv mir-edu.dylib /Library/Audio/Plug-Ins/Vamp/mir-edu.dylib

If you're using the pre-compiled library:

1. Move the pre-compiled library to your 'Vamp' plug-ins folder (you may have to create the Vamp folder is it doesn't exist yet):
justin$ mv builds/osx/mir-edu.dylib /Library/Audio/Plug-Ins/Vamp/mir-edu.dylib

Windows

If you're using the pre-compiled library:

1. Move the pre-compiled library to your 'Vamp Plugins' folder (you may have to create the Vamp folder is it doesn't exist yet):
32 bit: C:\Program Files\Vamp Plugins\mir-edu.dll
64 bit: C:\Program Files (x86)\Vamp Plugins\mir-edu.dll
