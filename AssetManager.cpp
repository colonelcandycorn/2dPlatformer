#include "AssetManager.h"
#include "GraphicsManager.h"
#include <SDL2/SDL_image.h>
#include <stdexcept>

void AssetManager::clear_assets() {
    for (auto& texture : textures) {
        SDL_DestroyTexture(texture.second);
    }
    textures.clear();
}

void AssetManager::add_texture(const std::string &asset_id, const std::string &file_path)
{
    SDL_Surface* surface = IMG_Load(file_path.c_str());

    if (!surface)
    {
        throw std::runtime_error("Failed to load image: " + file_path + " " + IMG_GetError());
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(GraphicsManager::get_renderer(), surface);

    if (!texture)
    {
        throw std::runtime_error("Failed to create texture: " + file_path + " " + SDL_GetError());
    }

    SDL_FreeSurface(surface);

    textures.emplace(asset_id, texture);

    return;
}

SDL_Texture* AssetManager::get_texture(const std::string& asset_id)
{
    return textures[asset_id];
}