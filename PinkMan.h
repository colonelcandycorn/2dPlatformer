
#ifndef PINKMAN_H
#define PINKMAN_H
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <vector>

/*
 * texture_type is an enum that represents the different textures that PinkMan can have.
 * The various constants below represent the number of frames for each texture, and other
 * constants that are used in the PinkMan class.
 */
enum texture_type {
    IDLE,
    RUNNING,
    JUMPING,
    FALLING
};

const int TEXT_COUNT = 4;
const int GRAVITY = 64;
const int JUMP_VELOCITY = -800;
const int JUMP_VELOCITY_X = 100;
const int RUNNING_VELOCITY = 170;


const int IDLE_FRAMES = 11;
const int RUNNING_FRAMES = 12;
const int PINKMAN_WIDTH = 32;
const int PINKMAN_HEIGHT = 32;
const int JUMPING_FRAMES = 1;
const int FALLING_FRAMES = 1;


class AssetManager;
class PinkManState;
class Tile;

class PinkMan {
public:
    explicit PinkMan(glm::vec2 position = glm::vec2(0, 0), glm::vec2 scale = glm::vec2(1, 1));
    ~PinkMan();

    void init(AssetManager* asset_manager);
    void update(Uint64 deltaTime, std::vector<Tile> collision_tiles);
    void render(int camera_x, int camera_y);

    void process_input(SDL_Event event);
    void update_position(Uint64 deltaTime);

    SDL_Rect& get_dest_rect();
    SDL_Rect& get_src_rect();
    glm::vec2& get_velocity();
    void set_flip_flag(SDL_RendererFlip flag);
    void update_texture(texture_type type);
    void update_state(PinkManState* state);
    SDL_RendererFlip& get_flip_flag();

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
