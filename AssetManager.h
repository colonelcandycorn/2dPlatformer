
#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include <map>
#include <string>
#include <SDL2/SDL.h>


class AssetManager {
public:
    AssetManager()=default;
    ~AssetManager()=default;
    AssetManager(const AssetManager& other)=delete;
    AssetManager& operator=(const AssetManager& other)=delete;

    void clear_assets();
    void add_texture(const std::string& asset_id, const std::string& file_path);

    SDL_Texture* get_texture(const std::string& asset_id);

private:
    std::map<std::string, SDL_Texture*> textures;
};


#endif //ASSETMANAGER_H
