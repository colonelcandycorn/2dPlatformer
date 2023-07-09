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

    assetManager = new AssetManager();
    assetManager->add_texture("pink_idle",
                              "assets/Free/characters/pink_man/idle.png");
    assetManager->add_texture("pink_running","assets/Free/characters/pink_man/running.png");

    pinkMan = new PinkMan({windowWidth / 2, windowHeight / 2 }, {1, 1});
    pinkMan->init(assetManager);

    isRunning = true;
}

/*
 * run()
 *
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
 * Processes input from the user.
 */

void Game::process_input()
{
    SDL_Event e;

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
 * Updates the game world.
 */
void Game::update()
{
    Uint64 time_to_wait = MILLISECONDS_PER_FRAME - (SDL_GetTicks64() - millisecondsPreviousFrame);

    if (time_to_wait > 0 && time_to_wait <= MILLISECONDS_PER_FRAME)
    {
        SDL_Delay(time_to_wait);
    }

    Uint64 delta_time = (SDL_GetTicks64() - millisecondsPreviousFrame);

    millisecondsPreviousFrame = SDL_GetTicks64();


    pinkMan->update(delta_time);


    return;
}

/*
 * render()
 *
 * Renders the game world.
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
 */
void Game::take_down()
{
    delete pinkMan;
    delete assetManager;
    GraphicsManager::release();
    SDL_Quit();
}