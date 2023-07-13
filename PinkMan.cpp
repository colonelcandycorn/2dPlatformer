#include "PinkMan.h"
#include "AssetManager.h"
#include "GraphicsManager.h"
#include "PinkManState.h"
#include <iostream>
#include "ColliderManager.h"
#include "Tile.h"
#include <map>
#include <algorithm>

PinkMan::PinkMan(glm::vec2 position, glm::vec2 scale) :
    position(position),
    scale(scale),
    velocity(glm::vec2(0, 0)),
    srcRect({0, 0, PINKMAN_WIDTH, PINKMAN_HEIGHT}),
    destRect({(int)position.x,
              (int)position.y,
              static_cast<int>(PINKMAN_WIDTH * scale.x),
              static_cast<int>(PINKMAN_HEIGHT * scale.y)}),
    current_texture(IDLE),
    flip_flag(SDL_FLIP_NONE),
    state(nullptr)
{
}

PinkMan::~PinkMan() {
    delete state;
}


void PinkMan::init(AssetManager* asset_manager) {
    textures[IDLE] = asset_manager->get_texture("pink_idle");
    textures[RUNNING] = asset_manager->get_texture("pink_running");
    textures[JUMPING] = asset_manager->get_texture("pink_jumping");
    textures[FALLING] = asset_manager->get_texture("pink_falling");

    state = new PinkManIdleState();

}

void PinkMan::update(Uint64 deltaTime, std::vector<Tile> tiles) {

    state->update(*this, deltaTime);

    //TODO: Move to separate function

    std::vector<std::pair<float, Tile*>> colliders;
    float time_to_collision = 0;
    glm::vec2 normal = glm::vec2(0, 0);

    for (auto& tile : tiles)
    {
        normal = glm::vec2(0, 0);

        if (tile.is_solid())
        {
            if (ColliderManager::rect_collision(destRect, velocity, tile.get_dest_rect(), deltaTime, time_to_collision, normal))
            {
                colliders.emplace_back(std::make_pair(time_to_collision, &tile));
            }
        }
    }

    normal = glm::vec2(0, 0);

    std::sort(colliders.begin(), colliders.end(), [](std::pair<float, Tile*> a, std::pair<float, Tile*> b) {
        return a.first < b.first;
    });

    for (auto& collider : colliders)
    {
        if (ColliderManager::rect_collision(destRect, velocity, collider.second->get_dest_rect(), deltaTime, time_to_collision, normal))
        {
            ColliderManager::resolve_collision(velocity, normal, time_to_collision);
            normal = glm::vec2(0, 0);
        }
        else
        {
            std::cout << "No collision" << std::endl;
        }
    }

    if (this->velocity.y == 0 && current_texture == FALLING)
    {
        if (this->velocity.x == 0)
        {
            update_state(new PinkManIdleState());
        }
    }

    update_position(deltaTime);

}

void PinkMan::render() {
    SDL_RenderCopyEx(GraphicsManager::get_renderer(),
                     textures[current_texture],
                     &srcRect,
                     &destRect,
                     0,
                     nullptr,
                     flip_flag);
}

SDL_Rect& PinkMan::get_dest_rect() {
    return destRect;
}

SDL_Rect& PinkMan::get_src_rect() {
    return srcRect;
}

void PinkMan::process_input(SDL_Event event)
{
    PinkManState* temp = state->process_input(*this, event);
    if (temp != nullptr)
    {
        delete state;
        state = temp;

        state->enter(*this);
    }
}

void PinkMan::update_state(PinkManState *a_state)
{
    if (state != nullptr)
    {
        delete this->state;
        state = a_state;

        state->enter(*this);
    }
    return;
}

void PinkMan::update_position(Uint64 deltaTime)
{

    position.x += velocity.x * deltaTime / 1000;
    //std::cout << velocity.x << std::endl;
    position.y += velocity.y * deltaTime / 1000;
    //std::cout << velocity.y << std::endl;

    destRect.x = (int)position.x;
    destRect.y = (int)position.y;

}

glm::vec2& PinkMan::get_velocity()
{
    return velocity;
}

void PinkMan::set_flip_flag(SDL_RendererFlip flag)
{
    flip_flag = flag;
}

void PinkMan::update_texture(texture_type type)
{
    current_texture = type;
}