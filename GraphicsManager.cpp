#include "GraphicsManager.h"

SDL_Window* GraphicsManager::get_window()
{
    /*
     * SDL_CreateWindow takes a title, x position, y position, width, height,
     * and flags. The x and y positions are for the window's position on the
     * screen. SDL_WINDOWPOS_CENTERED and SDL_WINDOWPOS_UNDEFINED can be used
     * to center or leave undefined the window's position. The width and height
     * are for the window's size.
     */

    static SDL_Window* window = SDL_CreateWindow(
            "Game",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WINDOW_WIDTH,
            WINDOW_HEIGHT,
            SDL_WINDOW_SHOWN);

    return window;
}

SDL_Renderer* GraphicsManager::get_renderer()
{
    /*
     * SDL_CreateRenderer takes a window and an index for the rendering driver
     * to be used. The index is useful when a system has multiple graphics
     * drivers. The index is -1 if we want to use the first driver that
     * supports the requested flags.
     *
     * The flags are for enabling hardware acceleration and vsync. Hardware
     * acceleration is enabled by default, but we want to enable vsync.
     */
    static SDL_Renderer* renderer = SDL_CreateRenderer(
            get_window(),
            -1,
            SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    return renderer;
}

void GraphicsManager::release()
{
    SDL_DestroyRenderer(get_renderer());
    SDL_DestroyWindow(get_window());
}
