CXXFLAGS=-O3 -Ofast
CXX=g++ $(CXXFLAGS)
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
