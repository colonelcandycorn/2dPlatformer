//
// Created by sarah on 7/11/23.
//

#include "Tile.h"
#include "Terrain.h"

Tile::Tile(SDL_Rect dest, Terrain* terrain) : dest_rect(dest), terrain(terrain)
{
}

void Tile::render() const
{
    terrain->render(dest_rect);
}

bool Tile::is_solid() const
{
    return terrain->is_solid();
}

SDL_Rect Tile::get_dest_rect()
{
    return dest_rect;
}

