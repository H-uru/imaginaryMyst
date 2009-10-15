CXX = g++
CXXFLAGS = -g -Wall -I/usr/include/SDL
LIBS = -lz -lSDL

OBJFILES = \
	out/imLog.o \
	out/imString.o \
	out/imSceneDatabase.o \
	out/dnifile.o \
	out/imStream.o \
	out/imVfs.o

all: bin/imaginaryMyst

clean:
	rm -f out/*.o
	rm -f bin/imaginaryMyst

bin/imaginaryMyst: $(OBJFILES) src/Main.cpp
	$(CXX) $(CXXFLAGS) $(OBJFILES) $(LIBS) src/Main.cpp -o $@

out/imLog.o: src/imLog.cpp
	$(CXX) $(CXXFLAGS) -c src/imLog.cpp -o $@

out/imString.o: src/imString.h src/imString.cpp
	$(CXX) $(CXXFLAGS) -c src/imString.cpp -o $@

out/imSceneDatabase.o: src/scene/imSceneDatabase.h src/scene/imSceneDatabase.cpp
	$(CXX) $(CXXFLAGS) -c src/scene/imSceneDatabase.cpp -o $@

out/dnifile.o: src/vfs/dnifile.cpp
	$(CXX) $(CXXFLAGS) -c src/vfs/dnifile.cpp -o $@

out/imStream.o: src/vfs/imStream.h src/vfs/imStream.cpp
	$(CXX) $(CXXFLAGS) -c src/vfs/imStream.cpp -o $@

out/imVfs.o: src/vfs/imVfs.h src/vfs/imVfs.cpp
	$(CXX) $(CXXFLAGS) -c src/vfs/imVfs.cpp -o $@
