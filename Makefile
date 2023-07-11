#Variables
CC = g++
CFLAGS = -Wall -Werror -g -std=c++17
COBJS = main.o \
		Game.o \
		GraphicsManager.o \
		AssetManager.o \
		PinkMan.o \
		PinkManState.o \
		Terrain.o \
		Tile.o \
		ColliderManager.o \
INCLUDE_PATH = -I"./libs/"
TARGETS = main
LFLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

#Default

all: $(TARGETS)

#Executables

main: $(COBJS)
	$(CC) $(^) -o $(@) $(LFLAGS)

main.o: main.cpp
	$(CC) $(CFLAGS) -c $(<) -o $(@)

Game.o: Game.cpp Game.h
	$(CC) $(CFLAGS) -c $(<) -o $(@)

GraphicsManager.o: GraphicsManager.cpp GraphicsManager.h
	$(CC) $(CFLAGS) -c $(<) -o $(@)

AssetManager.o: AssetManager.cpp AssetManager.h
	$(CC) $(CFLAGS) -c $(<) -o $(@)

PinkMan.o: PinkMan.cpp PinkMan.h
	$(CC) $(CFLAGS) -c $(<) -o $(@)

PinkManState.o: PinkManState.cpp PinkManState.h
	$(CC) $(CFLAGS) -c $(<) -o $(@)

Terrain.o: Terrain.cpp Terrain.h
	$(CC) $(CFLAGS) -c $(<) -o $(@)

Tile.o: Tile.cpp Tile.h
	$(CC) $(CFLAGS) -c $(<) -o $(@)

ColliderManager.o: ColliderManager.cpp ColliderManager.h
	$(CC) $(CFLAGS) -c $(<) -o $(@)


#CLEAN

clean:
	rm -f $(TARGETS) $(COBJS)
