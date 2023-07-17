#ifndef TERRAIN_H
#define TERRAIN_H

#include <string>
#include <SDL2/SDL.h>

class AssetManager;

class Terrain {
public:
    Terrain(bool solid,
            const std::string& a_id,
            SDL_Rect a_rect,
            AssetManager* asset_manager);
    ~Terrain()=default;

    [[nodiscard]] bool is_solid() const;
    //SDL_Texture* get_texture() const;
    void render(SDL_Rect dest_rect, int camera_x, int camera_y) const;

private:
    bool solid;
    const std::string asset_id;
    SDL_Rect src_rect;
    AssetManager* asset_manager;




};


#endif //TERRAIN_H
