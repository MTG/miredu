MIR.EDU
=======

A vamp plug-in library for teaching MIR.
Copyright (C) 2013 Justin Salamon.

MIR.EDU is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
at your option) any later version.

MIR.EDU is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

CONTACT
=======

MIR.EDU is written and maintained by:  
Justin Salamon  
email: <justin.salamon@nyu.edu>  
web: www.justinsalamon.com

DESCRIPTION
===========

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

BUILD
=====

Currently only instructions for building under OSX are available. Linux and windows instructions will follow.

OSX

First you need to download and build the Vamp plugin SDK, by following the instructions provided here:
<http://code.soundsoftware.ac.uk/projects/vamp-plugin-sdk/wiki/mtp1#1-Download-and-build-the-SDK> (remember to rename the SDK folder to 'vamp-plugin-sdk', i.e. remove the version number from the folder name). Then, from the same folder where you placed the Vamp SDK (i.e. if the SDK is at ```~/Documents/dev/vamp-plugin-sdk```, you should start at ```~/Documents/dev/```) follow the instructions below:

1. Clone the MIR.EDU repository using git:  
```
justin$ git clone https://github.com/justinsalamon/miredu.git  
```
2. Enter the miredu folder:  
```
justin$ cd miredu
```
3. Build the library using make  
```
justin$ make
```


INSTALLATION
============

OSX

If you built the library yourself:

1. Copy the compiled library (mir-edu.dylib) and metadata files (mir-edu.cat and mir-edu.n3) to your 'Vamp' plug-ins folder (you may have to create the Vamp folder is it doesn't exist yet):

```
justin$ cd miredu
justin$ cp mir-edu.dylib mir-edu.cat mir-edu.n3 /Library/Audio/Plug-Ins/Vamp/
```

If you're using the pre-compiled library:

1. Copy the pre-compiled library (builds/osx/mir-edu.dylib) and metadata files (mir-edu.cat and mir-edu.n3) to your 'Vamp' plug-ins folder (you may have to create the Vamp folder is it doesn't exist yet):

```
justin$ cd miredu  
justin$ cp builds/osx/mir-edu.dylib mir-edu.cat mir-edu.n3 /Library/Audio/Plug-Ins/Vamp/mir-edu.dylib
```

Windows

If you're using the pre-compiled library:

1. Copy the pre-compiled library (miredu\builds\windows) and metadata files (mir-edu.cat and mir-edu.n3) to your 'Vamp Plugins' folder (you may have to create the Vamp Plugins folder is it doesn't exist yet):

* 32 bit: C:\Program Files\Vamp Plugins\
* 64 bit: C:\Program Files (x86)\Vamp Plugins\
