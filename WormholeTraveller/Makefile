CC=g++
CFLAGS=-I. -I/usr/include -ggdb
LIBS=-lGLEW -lGL -lglut -lSOIL -ggdb

DEPS=Camera.h CubeMapTexture.h GeometryCommon.h LightingShader.h opstatus.h prefdefs.h SceneObject.h ShaderProgram.h SkyboxShader.h SphereObject.h Texture.h World.h WormholeShader.h WormholeTraveller.h

ODIR=obj
BUILDDIR=build

_OBJ=Camera.o CubeMapTexture.o LightingShader.o main.o SceneObject.o ShaderProgram.o SkyboxShader.o SphereObject.o Texture.o World.o WormholeShader.o WormholeTraveller.o
OBJ=$(patsubst %,$(ODIR)/%,$(_OBJ))

all: traveller copy_shaders copy_models copy_textures

dirs:
	mkdir -p $(ODIR)
	mkdir -p $(BUILDDIR)
	mkdir -p $(BUILDDIR)/renderprogs
	mkdir -p $(BUILDDIR)/models
	mkdir -p $(BUILDDIR)/textures

$(ODIR)/%.o: %.cpp $(DEPS) dirs
	$(CC) -c -o $@ $< $(CFLAGS)

traveller: $(OBJ)
	$(CC) -o $(BUILDDIR)/$@ $^ $(CFLAGS) $(LIBS)

copy_shaders:
	cp ./Shaders/*.vert $(BUILDDIR)/renderprogs/
	cp ./Shaders/*.frag $(BUILDDIR)/renderprogs/

copy_models:
	cp ../Models/*.obj $(BUILDDIR)/models/

copy_textures:
	cp -R ../Textures/skybox_cubemap $(BUILDDIR)/textures/
	cp ../Textures/dds/* $(BUILDDIR)/textures/
	
.PHONY: clean

clean:
	rm -rf $(ODIR)
	rm -rf $(BUILDDIR)

