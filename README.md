MIR.EDU
=======

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


CONTACT
=======

MIR.EDU is written and maintained by:  
Justin Salamon  
email: <justin.salamon@nyu.edu>  
web: www.justinsalamon.com


BUILD
=====

Currently only instructions for building under OSX and Linux are available. Windows instructions will be added in the future.

OSX & Linux:

1. First you need to download and build the Vamp plugin SDK, by following the steps below (based on
<http://code.soundsoftware.ac.uk/projects/vamp-plugin-sdk/wiki/mtp1#1-Download-and-build-the-SDK>):

	1.1. Download the Vamp plugin SDK from the "development headers and source code" link on the developer page at <http://vamp-plugins.org/develop.html> -- the file you want is vamp-plugin-sdk-N.tar.gz where N is the latest version number of the SDK. Save it into your home directory, open a terminal window, and unpack it. We'll also rename its directory to vamp-plugin-sdk for easier reference later on.

	```
	mac:dev justin$ ls vamp*
	vamp-plugin-sdk-2.2.tar.gz
	mac:dev justin$ tar xvzf vamp-plugin-sdk-2.2.tar.gz
 	... lots of output ...
	mac:dev justin$ mv vamp-plugin-sdk-2.2 vamp-plugin-sdk
	mac:dev justin$
	```

	1.2. At this point you really ought to read the README file in the SDK directory, and the README.osx (or README.linux) file in the SDK's build subdirectory. Here we'll skip that and plunge in and build the SDK directly.

	OSX:

	```
	mac:dev justin$ cd vamp-plugin-sdk
	mac:vamp-plugin-sdk justin$
	mac:vamp-plugin-sdk justin$ make -f build/Makefile.osx sdk
 	... lots of output ...
	mac:vamp-plugin-sdk justin$
	```

	Linux:

	```
	linux:dev justin$ cd vamp-plugin-sdk
	linux:vamp-plugin-sdk justin$
	linux:vamp-plugin-sdk justin$ ./configure
	linux:vamp-plugin-sdk justin$ make
 	... lots of output ...
	linux:vamp-plugin-sdk justin$
	```

2. Then, from the same folder where you placed the Vamp SDK (i.e. if the SDK is at ```~/Documents/dev/vamp-plugin-sdk```, you should start at ```~/Documents/dev/```) follow the instructions below:

	2.1. Clone the MIR.EDU repository using git:  
	
	```
	mac:dev justin$ git clone https://github.com/justinsalamon/miredu.git  
	```

	2.2. Enter the miredu folder:  

	```
	mac:dev justin$ cd miredu
	mac:miredu justin$ 
	```

	2.3. Rename 'Makefile.osx' or 'Makefile.linux' to 'Make' and build the library:

	```
	mac:miredu justin$ mv Makefile.osx Makefile
	mac:miredu justin$ make
	```

INSTALLATION
============

OSX

If you built the library yourself:

1. Copy the compiled library (mir-edu.dylib) and metadata files (mir-edu.cat and mir-edu.n3) to your 'Vamp' plug-ins folder (you may have to create the Vamp folder is it doesn't exist yet):

	```
	mac:dev justin$ mkdir /Library/Audio/Plug-ins/Vamp
	mac:dev justin$ cd miredu
	mac:miredu justin$
	mac:miredu justin$ cp mir-edu.dylib mir-edu.cat mir-edu.n3 /Library/Audio/Plug-Ins/Vamp/
	```

If you're using the pre-compiled library:

1. Copy the pre-compiled library (builds/osx/mir-edu.dylib) and metadata files (mir-edu.cat and mir-edu.n3) to your 'Vamp' plug-ins folder (you may have to create the Vamp folder is it doesn't exist yet):

	```
	mac:dev justin$ mkdir /Library/Audio/Plug-ins/Vamp
	mac:dev justin$ cd miredu  
	mac:miredu justin$
	mac:miredu justin$ cp builds/osx/mir-edu.dylib mir-edu.cat mir-edu.n3 /Library/Audio/Plug-Ins/Vamp/
	```
	
Linux

If you built the library yourself:

1. Copy the compiled library (mir-edu.so) and metadata files (mir-edu.cat and mir-edu.n3) to your 'vamp' plug-ins folder (you may have to create the vamp folder is it doesn't exist yet):

	```
	linux:dev justin$ mkdir ~/vamp
	linux:dev justin$ cd miredu
	linux:miredu justin$ 
	linux:miredu justin$ cp mir-edu.so mir-edu.cat mir-edu.n3 ~/vamp/
	```

If you're using the pre-compiled library:

1. Copy the pre-compiled library (builds/osx/mir-edu.dylib) and metadata files (mir-edu.cat and mir-edu.n3) to your 'Vamp' plug-ins folder (you may have to create the Vamp folder is it doesn't exist yet):

	```
	linux:dev justin$ mkdir ~/vamp
	linux:dev justin$ cd miredu  
	linux:miredu justin$
	linux:miredu justin$ cp builds/linux64/mir-edu.so mir-edu.cat mir-edu.n3 ~/vamp/
	```

Windows

If you're using the pre-compiled library:

1. Copy the pre-compiled library (miredu\builds\windows\mir-edu.dll) and metadata files (mir-edu.cat and mir-edu.n3) to your 'Vamp Plugins' folder (you may have to create the Vamp Plugins folder is it doesn't exist yet):

* 32 bit: C:\Program Files\Vamp Plugins\
* 64 bit: C:\Program Files (x86)\Vamp Plugins\

[![githalytics.com alpha](https://cruel-carlota.pagodabox.com/a334029ff1b8a2cb72aa7794a44c3906 "githalytics.com")](http://githalytics.com/justinsalamon/miredu)


ACKNOWLEDGMENTS
===============
MIR.EDU is supported by:
- Universitat Pompeu Fabra
- New York University


LICENSE
=======

A vamp plug-in library for teaching MIR.
Copyright (C) 2013-2014 Justin Salamon.

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
