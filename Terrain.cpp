#include "Terrain.h"
#include "AssetManager.h"
#include "GraphicsManager.h"

Terrain::Terrain(bool solid,
                 const std::string& a_id,
                 SDL_Rect a_rect,
                 AssetManager* asset_manager) :
    solid(solid),
    asset_id(a_id),
    src_rect(a_rect),
    asset_manager(asset_manager)
{
}

bool Terrain::is_solid() const {
    return solid;
}

void Terrain::render(SDL_Rect dest_rect, int camera_x, int camera_y) const
{
    SDL_Rect temp = dest_rect;
    temp.x -= camera_x;
    temp.y -= camera_y;

    SDL_RenderCopyEx(GraphicsManager::get_renderer(),
                     asset_manager->get_texture(asset_id),
                     &src_rect,
                     &temp,
                     0,
                     nullptr,
                     SDL_FLIP_NONE);
}
