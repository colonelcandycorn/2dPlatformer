//
// Created by sarah on 7/11/23.
//

#include "My_Tile.h"
#include "Terrain.h"

My_Tile::My_Tile(SDL_Rect dest, Terrain* terrain) : dest_rect(dest), terrain(terrain)
{
}

void My_Tile::render(int camera_x, int camera_y) const
{
    terrain->render(dest_rect, camera_x, camera_y);
}

bool My_Tile::is_solid() const
{
    return terrain->is_solid();
}

SDL_Rect My_Tile::get_dest_rect()
{
    return dest_rect;
}

