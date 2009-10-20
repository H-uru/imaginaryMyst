CXX = g++
CXXFLAGS = -g -Wall -I/usr/include/SDL -Ilibsquish -DNO_DXT_COMPRESSION
LIBS = -lz -lSDL -lGL -lGLU -Llibsquish -lsquish

OBJFILES = \
	out/imLog.o \
	out/imString.o \
	out/imSceneDatabase.o \
	out/imMipmap.o \
	out/dnifile.o \
	out/imStream.o \
	out/imVfs.o

all: libsquish/libsquish.a bin/imaginaryMyst

clean:
	rm -f out/*.o
	rm -f bin/imaginaryMyst

libsquish/libsquish.a:
	( cd libsquish && make )

bin/imaginaryMyst: $(OBJFILES) src/Main.cpp
	$(CXX) $(CXXFLAGS) $(OBJFILES) $(LIBS) src/Main.cpp -o $@

out/imLog.o: src/imLog.cpp
	$(CXX) $(CXXFLAGS) -c src/imLog.cpp -o $@

out/imString.o: src/imString.h src/imString.cpp
	$(CXX) $(CXXFLAGS) -c src/imString.cpp -o $@

out/imSceneDatabase.o: src/scene/imSceneDatabase.h src/scene/imSceneDatabase.cpp
	$(CXX) $(CXXFLAGS) -c src/scene/imSceneDatabase.cpp -o $@

out/imMipmap.o: src/surface/imMipmap.h src/surface/imMipmap.cpp
	$(CXX) $(CXXFLAGS) -c src/surface/imMipmap.cpp -o $@

out/dnifile.o: src/vfs/dnifile.cpp
	$(CXX) $(CXXFLAGS) -c src/vfs/dnifile.cpp -o $@

out/imStream.o: src/vfs/imStream.h src/vfs/imStream.cpp
	$(CXX) $(CXXFLAGS) -c src/vfs/imStream.cpp -o $@

out/imVfs.o: src/vfs/imVfs.h src/vfs/imVfs.cpp
	$(CXX) $(CXXFLAGS) -c src/vfs/imVfs.cpp -o $@
