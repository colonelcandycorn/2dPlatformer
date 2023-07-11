#ifndef GRAPHICSMANAGER_H
#define GRAPHICSMANAGER_H

#include <SDL2/SDL.h>

const int WINDOW_WIDTH = 640;
const int WINDOW_HEIGHT = 480;

class GraphicsManager {
public:
    static SDL_Window* get_window();
    static SDL_Renderer* get_renderer();

    static void release();

private:
    GraphicsManager()=default;
};


#endif //GRAPHICSMANAGER_H
