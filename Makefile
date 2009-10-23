CXX = g++
CXXFLAGS = -g -Wall -I/usr/include/SDL -Ilibsquish -DNO_DXT_COMPRESSION
LIBS = -lz -lSDL -lGL -lGLU -Llibsquish -lsquish

OBJFILES = \
	out/imGeometry3.o \
	out/imKeyList.o \
	out/imLog.o \
	out/imString.o \
	out/imAnimBehavior.o \
	out/imController.o \
	out/imKeyFrame.o \
	out/imLeafController.o \
	out/imSceneDatabase.o \
	out/imSceneIndex.o \
	out/imAnimLayer.o \
	out/imLayer.o \
	out/imMaterial.o \
	out/imMipmap.o \
	out/imTexture.o \
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

out/imGeometry3.o: src/imGeometry3.h src/imGeometry3.cpp
	$(CXX) $(CXXFLAGS) -c src/imGeometry3.cpp -o $@

out/imKeyList.o: src/imKeyList.h src/imKeyList.cpp
	$(CXX) $(CXXFLAGS) -c src/imKeyList.cpp -o $@

out/imLog.o: src/imLog.cpp
	$(CXX) $(CXXFLAGS) -c src/imLog.cpp -o $@

out/imString.o: src/imString.h src/imString.cpp
	$(CXX) $(CXXFLAGS) -c src/imString.cpp -o $@

out/imAnimBehavior.o: src/anim/imAnimBehavior.h src/anim/imAnimBehavior.cpp
	$(CXX) $(CXXFLAGS) -c src/anim/imAnimBehavior.cpp -o $@

out/imController.o: src/anim/imController.h src/anim/imController.cpp
	$(CXX) $(CXXFLAGS) -c src/anim/imController.cpp -o $@

out/imKeyFrame.o: src/anim/imKeyFrame.h src/anim/imKeyFrame.cpp
	$(CXX) $(CXXFLAGS) -c src/anim/imKeyFrame.cpp -o $@

out/imLeafController.o: src/anim/imLeafController.h src/anim/imLeafController.cpp
	$(CXX) $(CXXFLAGS) -c src/anim/imLeafController.cpp -o $@

out/imSceneDatabase.o: src/scene/imSceneDatabase.h src/scene/imSceneDatabase.cpp
	$(CXX) $(CXXFLAGS) -c src/scene/imSceneDatabase.cpp -o $@

out/imSceneIndex.o: src/scene/imSceneIndex.h src/scene/imSceneIndex.cpp
	$(CXX) $(CXXFLAGS) -c src/scene/imSceneIndex.cpp -o $@

out/imAnimLayer.o: src/surface/imAnimLayer.h src/surface/imAnimLayer.cpp
	$(CXX) $(CXXFLAGS) -c src/surface/imAnimLayer.cpp -o $@

out/imLayer.o: src/surface/imLayer.h src/surface/imLayer.cpp
	$(CXX) $(CXXFLAGS) -c src/surface/imLayer.cpp -o $@

out/imMaterial.o: src/surface/imMaterial.h src/surface/imMaterial.cpp
	$(CXX) $(CXXFLAGS) -c src/surface/imMaterial.cpp -o $@

out/imMipmap.o: src/surface/imMipmap.h src/surface/imMipmap.cpp
	$(CXX) $(CXXFLAGS) -c src/surface/imMipmap.cpp -o $@

out/imTexture.o: src/surface/imTexture.h src/surface/imTexture.cpp
	$(CXX) $(CXXFLAGS) -c src/surface/imTexture.cpp -o $@

out/dnifile.o: src/vfs/dnifile.cpp
	$(CXX) $(CXXFLAGS) -c src/vfs/dnifile.cpp -o $@

out/imStream.o: src/vfs/imStream.h src/vfs/imStream.cpp
	$(CXX) $(CXXFLAGS) -c src/vfs/imStream.cpp -o $@

out/imVfs.o: src/vfs/imVfs.h src/vfs/imVfs.cpp
	$(CXX) $(CXXFLAGS) -c src/vfs/imVfs.cpp -o $@
