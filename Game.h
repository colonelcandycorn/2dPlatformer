#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <memory>
#include <stdexcept>
#include <glm/glm.hpp>
#include "GraphicsManager.h"
#include "PinkMan.h"
#include "AssetManager.h"
#include "Terrain.h"
#include <string>
#include <vector>
#include "Tile.h"

using namespace std;

const int FPS = 30;
const Uint64 MILLISECONDS_PER_FRAME = 1000 / FPS;
const int TILE_SIZE = 32;
const int TERRAIN_TYPE_COUNT = 1;

enum TerrainType
{
    GRASS,
    EMPTY
};

class Game
{
private:
    bool isRunning;
    Uint64 millisecondsPreviousFrame;
    PinkMan* pinkMan;
    AssetManager* assetManager;
    Terrain* terrain[TERRAIN_TYPE_COUNT];

    //TODO: Move Map to its own class
    vector<Tile> tiles;

public:
    //Constructors, Destructors, and Assignment Operators
    Game();
    ~Game()=default;
    Game& operator=(const Game& other) = delete;
    Game(const Game& other) = delete;

    //Member Functions
    void init();
    void run();
    void take_down();
    void load_assets();
    void init_game_objects();

    //inside of run()
    void process_input();
    void update();
    void render();
};












#endif // GAME_H