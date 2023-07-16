#ifndef TILE_H
#define TILE_H

#include <SDL2/SDL_rect.h>

class Terrain;

class Tile {
public:
    Tile(SDL_Rect dest, Terrain* terrain);
    ~Tile()=default;

    void render(int camera_x, int camera_y) const;
    [[nodiscard]] bool is_solid() const;
    [[nodiscard]] SDL_Rect get_dest_rect();

private:
    SDL_Rect dest_rect;
    Terrain* terrain;

};


#endif //TILE_H
