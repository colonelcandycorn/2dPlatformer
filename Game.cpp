#include "Game.h"
#include <iostream>


Game::Game() :
    isRunning(false),
    millisecondsPreviousFrame(0),
    pinkMan(nullptr),
    assetManager(nullptr),
    camera({0, 0, WINDOW_WIDTH, WINDOW_HEIGHT})
{
}

/*
 * init()
 *
 * Initializes SDL and creates the window and renderer. Throws a runtime_error
 * if SDL fails to initialize or if the window or renderer fail to be created.
 */
void Game::init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING))
    {
        throw runtime_error("Error initializing SDL");
    }

    //SDL_DisplayMode display_mode; // leave for now

    if (!GraphicsManager::get_window())
    {
        throw runtime_error("Error creating SDL window");
    }

    if (!GraphicsManager::get_renderer())
    {
        throw runtime_error("Error creating SDL renderer");
    }


    assetManager = new AssetManager();
    load_assets();

    load_level("assets/tiled/test.tmx");

    // TODO: Separate this into a function or read from a file to load all assets

    //TODO: Make PinkMan and AssetManager into smart pointers
    //TODO: Make PinkMan an entity and abstract it into a class
    init_game_objects();
    isRunning = true;
}

void Game::load_assets()
{
    assetManager->add_texture("pink_idle",
                              "assets/Free/characters/pink_man/idle.png");
    assetManager->add_texture("pink_running","assets/Free/characters/pink_man/running.png");
    assetManager->add_texture("pink_jumping", "assets/Free/characters/pink_man/jump.png");
    assetManager->add_texture("pink_falling", "assets/Free/characters/pink_man/fall.png");

}

void Game::init_game_objects()
{
    pinkMan = new PinkMan({64, 64}, {1, 1});
    pinkMan->init(assetManager);

}


/*
 * run()
 *
 * Runs the game loop.
 *
 */

void Game::run()
{
    while (isRunning)
    {
        process_input();
        update();
        render();
    }
}

/*
 * process_input()
 *
 * Processes input from the user. Right now it just checks for the escape key/quit,
 * otherwise it passes the event to PinkMan to handle. This will change as more
 * entities are added to the game.
 */

void Game::process_input()
{
    SDL_Event e;

    //TODO: Make this less reliant on PinkMan resolving input
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
            case SDL_QUIT:
                isRunning = false;
                break;
            case SDL_KEYDOWN:
                if (e.key.keysym.sym == SDLK_ESCAPE)
                {
                    isRunning = false;
                }
                else
                {
                    pinkMan->process_input(e);
                }
                break;
            case SDL_KEYUP:
                pinkMan->process_input(e);
                break;
            default:
                break;
        }
    }
}

/*
 * update()
 *
 * Updates the game world. Most of my knowledge of this comes from
 * "Game Programming Patterns" by Robert Nystrom. In his section on the
 * "Game Loop" he describes the "Fixed Timestep" pattern. This is the
 * pattern I am using here.
 */
void Game::update()
{
    static Uint64 lag = 0;
    Uint64 time_to_wait = MILLISECONDS_PER_FRAME - (SDL_GetTicks64() - millisecondsPreviousFrame);

    if (time_to_wait > 0 && time_to_wait <= MILLISECONDS_PER_FRAME)
    {
        SDL_Delay(time_to_wait);
    }

    Uint64 delta_time = (SDL_GetTicks64() - millisecondsPreviousFrame);
    lag += delta_time;

    millisecondsPreviousFrame = SDL_GetTicks64();

    while (lag >= MILLISECONDS_PER_FRAME)
    {

        pinkMan->update(MILLISECONDS_PER_FRAME, level_tiles);
        update_camera();
        lag -= MILLISECONDS_PER_FRAME;
    }


    return;
}

/*
 * render()
 *
 * Renders the game world. Renderer is cleared, game objects are drawn, and
 * the renderer is presented. Currently, the renderer is a Singleton, but
 * I may change this in the future.
 */

void Game::render()
{
    SDL_SetRenderDrawColor(GraphicsManager::get_renderer(), 21, 21, 21, 255);
    SDL_RenderClear(GraphicsManager::get_renderer());



    for (auto& tile_row : background_tiles)
    {
        for (auto& tile : tile_row)
        {
            tile.render(camera.x, camera.y);
        }
    }

    for (auto& tile_row : level_tiles)
    {
        for (auto& tile : tile_row)
        {
            tile.render(camera.x, camera.y);
        }
    }

    pinkMan->render(camera.x, camera.y);


    SDL_RenderPresent(GraphicsManager::get_renderer());
}

/*
 * take_down()
 *
 * Destroys the window and renderer and quits SDL.
 *
 * Also deletes the PinkMan and AssetManager. Probably will change this in the
 * future to use smart pointers.
 */
void Game::take_down()
{
    delete pinkMan;
    pinkMan = nullptr;

    level_tiles.clear();
    assetManager->clear_assets();
    delete assetManager;
    assetManager = nullptr;


    GraphicsManager::release();
    SDL_Quit();
}

void Game::update_camera()
{
    camera.x = pinkMan->get_dest_rect().x - (WINDOW_WIDTH / 2);
    camera.y = pinkMan->get_dest_rect().y - (WINDOW_HEIGHT / 2);

    if (camera.x < 0)
    {
        camera.x = 0;
    }

    if (camera.y < 0)
    {
        camera.y = 0;
    }

    // if camera is greater than the width of the map minus the width of the window
    if (camera.y + WINDOW_HEIGHT > LEVEL_HEIGHT)
    {
        camera.y = LEVEL_HEIGHT - WINDOW_HEIGHT;
    }

    if (camera.x + WINDOW_WIDTH > LEVEL_WIDTH)
    {
        camera.x = LEVEL_WIDTH - WINDOW_WIDTH;
    }


}

void Game::load_level(string level_file_path)
{
    if (!map.load(level_file_path))
    {
        throw runtime_error("Failed to load level file");
    }

    for (auto& tileset : map.getTilesets())
    {
        load_tilesets(tileset);
    }

    level_tiles.resize(map.getTileCount().y);
    background_tiles.resize(map.getTileCount().y);

    for (auto& layer : map.getLayers())
    {
        if (layer->getName() == "Background")
        {
            load_layer(*layer, background_tiles);
        }
        else if (layer->getName() == "Level")
        {
            load_layer(*layer, level_tiles);
        }
    }
}

void Game::load_tilesets(const tmx::Tileset &tileset)
{
    string path = (string) tileset.getImagePath();
    string name = (string) tileset.getName();
    assetManager->add_texture(name, path);

    for (auto tile : tileset.getTiles())
    {
        tmx::Vector2 a_pos = tile.imagePosition;
        tmx::Vector2 a_size = tile.imageSize;
        SDL_Rect src_rect = {(int) a_pos.x,
                             (int) a_pos.y,
                             (int) a_size.x,
                             (int) a_size.y};

        if (!tile.properties.empty())
        {
            bool solid = (bool) tile.properties.front().getBoolValue();
            terrains.emplace_back(solid,
                                  name,
                                  src_rect,
                                  assetManager);
        }
    }
}

void Game::load_layer(tmx::Layer &layer, vector<vector<My_Tile>> &tiles)
{
    if (layer.getType() == tmx::Layer::Type::Tile) {
        auto &tileLayer = layer.getLayerAs<tmx::TileLayer>();
        auto &some_tiles = tileLayer.getTiles();
        int tile_size_x = map.getTileSize().x;
        int tile_size_y = map.getTileSize().y;

        for (int i = 0; i < (int) some_tiles.size(); i++) {
            if (some_tiles[i].ID != 0) {
                int x_offset = (int) i % map.getTileCount().x * tile_size_x;
                int y_offset = (int) i / map.getTileCount().x * tile_size_y;


                SDL_Rect dest_rect = {x_offset,
                                      y_offset,
                                      tile_size_x,
                                      tile_size_y};

                tiles[i / map.getTileCount().x].emplace_back(dest_rect, &terrains[some_tiles[i].ID - 1]);
            }
        }
    }
}