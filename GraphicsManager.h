#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H

#include <SDL2/SDL.h>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

class GraphicsManager {
public:
    static SDL_Window* get_window();
    static SDL_Renderer* get_renderer();

    static void release();

private:
    GraphicsManager();
};


#endif //GRAPHICSMANAGER_H
