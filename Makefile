CXXFLAGS=-Wall -O3 -Ofast -std=c++98
CXX=g++
DEBUG=

.SUFFIXES: .cpp .o

OFILES= makemake.o
makemake: $(OFILES)
	$(CXX) $(DEBUG) $(OFILES) -o makemake

clean:
	-rm *.o makemake

prefix=/usr

install: makemake
	install -m 0755 makemake $(prefix)/bin

uninstall: makemake
	rm $(prefix)/bin/makemake

makemake.o: makemake.cpp 
