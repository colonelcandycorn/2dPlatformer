#include "Game.h"
#include <iostream>

Game::Game() :
    isRunning(false),
    millisecondsPreviousFrame(0),
    pinkMan(nullptr),
    assetManager(nullptr)
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

    int windowWidth, windowHeight;
    SDL_GetWindowSize(GraphicsManager::get_window(), &windowWidth, &windowHeight);

    // TODO: Separate this into a function or read from a file to load all assets
    assetManager = new AssetManager();
    assetManager->add_texture("pink_idle",
                              "assets/Free/characters/pink_man/idle.png");
    assetManager->add_texture("pink_running","assets/Free/characters/pink_man/running.png");
    assetManager->add_texture("pink_jumping", "assets/Free/characters/pink_man/jump.png");
    assetManager->add_texture("pink_falling", "assets/Free/characters/pink_man/fall.png");


    //TODO: Make PinkMan and AssetManager into smart pointers
    //TODO: Make PinkMan an entity and abstract it into a class
    pinkMan = new PinkMan({windowWidth / 2, windowHeight / 2 }, {1, 1});
    pinkMan->init(assetManager);

    isRunning = true;
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
        pinkMan->update(MILLISECONDS_PER_FRAME);
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


    pinkMan->render();

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
    delete assetManager;
    GraphicsManager::release();
    SDL_Quit();
}