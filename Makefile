all:
	swig -python -c++ -module PicoScope4824 PicoScopeClass4824.swg
	g++ -std=c++11 PicoScopeClass4824.cxx PicoScopeClass4824_wrap.cxx -fPIC -I/opt/picoscope/include -I/usr/include/python3.5/ -L/opt/picoscope/lib -lps4000a -shared -o _PicoScope4824.so
clean:
	rm -f *.so
	rm -f *.py
	rm -f *.pyc
	rm -f PicoScopeClass4824_wrap.cxx
	rm -Rf html
	rm -Rf latex
doc:
	doxygen
