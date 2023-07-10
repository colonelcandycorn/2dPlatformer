//
// Created by sarah on 7/4/23.
//

#ifndef PINKMAN_H
#define PINKMAN_H
#include <glm/glm.hpp>
#include <SDL2/SDL.h>

enum texture_type {
    IDLE,
    RUNNING,
    JUMPING,
    FALLING
};

const int TEXT_COUNT = 4;
const int GRAVITY = 83;
const int JUMP_VELOCITY = -1000 ;

const int IDLE_FRAMES = 11;
const int RUNNING_FRAMES = 12;
const int PINKMAN_WIDTH = 32;
const int PINKMAN_HEIGHT = 32;
const int JUMPING_FRAMES = 1;
const int FALLING_FRAMES = 1;


class AssetManager;
class PinkManState;

class PinkMan {
public:
    explicit PinkMan(glm::vec2 position = glm::vec2(0, 0), glm::vec2 scale = glm::vec2(1, 1));
    ~PinkMan();

    void init(AssetManager* asset_manager);
    void update(Uint64 deltaTime);
    void render();

    void process_input(SDL_Event event);
    void update_position(Uint64 deltaTime);

    SDL_Rect& get_dest_rect();
    SDL_Rect& get_src_rect();
    glm::vec2& get_velocity();
    void set_flip_flag(SDL_RendererFlip flag);
    void update_texture(texture_type type);
    void update_state(PinkManState* state);

private:


    glm::vec2 position;
    glm::vec2 scale;
    glm::vec2 velocity;
    SDL_Rect srcRect, destRect;

    SDL_Texture* textures[TEXT_COUNT];
    texture_type current_texture;
    SDL_RendererFlip flip_flag;
    PinkManState* state;

};


#endif //PINKMAN_H
