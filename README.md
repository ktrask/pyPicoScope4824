# pyPicoScope4824
Python and C++ Wrapper for the PicoScope 4824 API.

The Makefile assumes python 3.5 to work, and you need swig installed. Also you need the picoscope API installed, you can get it from the picotech.com website.
Then you can run make to build the library.
There is an example.py to show you the usage. The example starts streaming to a file called stream.csv.
This python library is at a very early state, and things like simultaneously streaming don't work at the moment.
