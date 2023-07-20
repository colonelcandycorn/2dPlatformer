//
// Created by sarah on 7/4/23.
//

#include "PinkManState.h"
#include "PinkMan.h"
#include "ColliderManager.h"
#include "My_Tile.h"
#include <iostream>
#include <algorithm>



/*
 ****************************************************************************************
 * PinkManState
 * **************************************************************************************
 */
PinkManState::PinkManState() : is_keyup_event(false) {}

PinkManState::PinkManState(bool is_keyup_event) : is_keyup_event(is_keyup_event) {}

void PinkManState::update_(PinkMan &hero, Uint64 deltaTime, int frames)
{
    hero.get_velocity().y += GRAVITY;

    static int time_since_last_frame = 0;

    time_since_last_frame += deltaTime;

    if (time_since_last_frame > 48 ) {
        time_since_last_frame = 0;
        hero.get_src_rect().x += PINKMAN_WIDTH;
        if (hero.get_src_rect().x >= PINKMAN_WIDTH * frames) {
            hero.get_src_rect().x = 0;
        }
    }


}


void PinkManState::collide_(PinkMan &hero, Uint64 deltaTime, vector<vector<My_Tile>> &tiles)
{
    std::vector<std::pair<float, My_Tile*>> colliders;
    float time_to_collision = 0;
    glm::vec2 normal = glm::vec2(0, 0);

    for (auto& tile_row : tiles) {
        for (auto &tile: tile_row) {
            normal = glm::vec2(0, 0);

            if (tile.is_solid()) {
                if (ColliderManager::rect_collision(hero.get_dest_rect(), hero.get_velocity(), tile.get_dest_rect(),
                                                    deltaTime, time_to_collision, normal)) {
                    colliders.emplace_back(std::make_pair(time_to_collision, &tile));
                }
            }
        }
    }

    normal = glm::vec2(0, 0);

    std::sort(colliders.begin(), colliders.end(), [](std::pair<float, My_Tile*> a, std::pair<float, My_Tile*> b) {
        return a.first < b.first;
    });

    for (auto& collider : colliders)
    {
        if (ColliderManager::rect_collision(hero.get_dest_rect(), hero.get_velocity(), collider.second->get_dest_rect(), deltaTime, time_to_collision, normal))
        {
            ColliderManager::resolve_collision(hero.get_velocity(), normal, time_to_collision);
            normal = glm::vec2(0, 0);
        }
    }

    if (hero.get_velocity().y > 0)
        hero.update_state(new PinkManFallingState(is_keyup_event));
}
/*
 ****************************************************************************************
 * PinkManIdleState
 * **************************************************************************************
 */
PinkManIdleState::~PinkManIdleState() = default;

void PinkManIdleState::update(PinkMan& hero, Uint64 deltaTime, vector<vector<My_Tile>> &tiles)
{
    PinkManState::update_(hero, deltaTime, IDLE_FRAMES);
    PinkManState::collide_(hero, deltaTime, tiles);
}

PinkManState* PinkManIdleState::process_input(PinkMan &hero, SDL_Event event)
{
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                return new PinkManLeftRunningState();
            case SDLK_RIGHT:
                return new PinkManRightRunningState();
            case SDLK_SPACE:
                return new PinkManJumpingState();
            default:
                break;
        }
    }
    return nullptr;
}

void PinkManIdleState::enter(PinkMan &hero)
{
    hero.get_velocity().x = 0;
    hero.get_velocity().y = 0;
    hero.update_texture(IDLE);
    hero.get_src_rect().x = 0;
}

/*
 ****************************************************************************************
 * PinkManRightRunningState
 * **************************************************************************************
 */

PinkManRightRunningState::~PinkManRightRunningState() = default;

void PinkManRightRunningState::update(PinkMan &hero, Uint64 deltaTime, vector<vector<My_Tile>> &tiles)
{
    PinkManState::update_(hero, deltaTime, RUNNING_FRAMES);
    PinkManState::collide_(hero, deltaTime, tiles);
}

PinkManState* PinkManRightRunningState::process_input(PinkMan& hero, SDL_Event event)
{
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                return new PinkManLeftRunningState();
            case SDLK_RIGHT:
                hero.get_velocity().x = RUNNING_VELOCITY;
                break;
            case SDLK_SPACE:
                return new PinkManJumpingState();
            default:
                break;
        }
    }
    else if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
            case SDLK_RIGHT:
                return new PinkManIdleState();
            default:
                break;
        }
    }
    return nullptr;
}

void PinkManRightRunningState::enter(PinkMan &hero)
{
    hero.update_texture(RUNNING);
    hero.set_flip_flag(SDL_FLIP_NONE);
    hero.get_src_rect().x = 0;
    hero.get_velocity().x = 170;
}

/*
 ****************************************************************************************
 * PinkManLeftRunningState
 * **************************************************************************************
 */

PinkManLeftRunningState::~PinkManLeftRunningState() = default;

PinkManState* PinkManLeftRunningState::process_input(PinkMan &hero, SDL_Event event)
{
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                hero.get_velocity().x = -RUNNING_VELOCITY;
                break;
            case SDLK_RIGHT:
                return new PinkManRightRunningState();
            case SDLK_SPACE:
                return new PinkManJumpingState();
            default:
                break;
        }
    }
    else if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                return new PinkManIdleState();
            default:
                break;
        }
    }
    return nullptr;
}

void PinkManLeftRunningState::update(PinkMan &hero, Uint64 deltaTime, vector<vector<My_Tile>> &tiles)
{
    PinkManState::update_(hero, deltaTime, RUNNING_FRAMES);
    PinkManState::collide_(hero, deltaTime, tiles);
}

void PinkManLeftRunningState::enter(PinkMan &hero)
{
    hero.update_texture(RUNNING);
    hero.set_flip_flag(SDL_FLIP_HORIZONTAL);
    hero.get_src_rect().x = 0;
    hero.get_velocity().x = -170;
}

/*
 ****************************************************************************************
 * PinkManJumpingState
 * **************************************************************************************
 */

PinkManJumpingState::~PinkManJumpingState() = default;

PinkManState* PinkManJumpingState::process_input(PinkMan &hero, SDL_Event event)
{
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_LEFT:
               hero.get_velocity().x = -JUMP_VELOCITY_X;
                hero.set_flip_flag(SDL_FLIP_HORIZONTAL);
                break;
            case SDLK_RIGHT:
                hero.get_velocity().x = JUMP_VELOCITY_X;
                hero.set_flip_flag(SDL_FLIP_NONE);
                break;
            default:
                break;
        }
    }
    else if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                hero.get_velocity().x = -JUMP_VELOCITY_X;
                is_keyup_event = true;
                break;
            case SDLK_RIGHT:
                hero.get_velocity().x = JUMP_VELOCITY_X;
                is_keyup_event = true;
                break;
            default:
                break;
        }
    }
    return nullptr;
}

void PinkManJumpingState::update(PinkMan &hero, Uint64 deltaTime, vector<vector<My_Tile>> &tiles)
{
    PinkManState::update_(hero, deltaTime, JUMPING_FRAMES);

    PinkManState::collide_(hero, deltaTime, tiles);


}

void PinkManJumpingState::enter(PinkMan &hero)
{
    hero.update_texture(JUMPING);
    hero.get_src_rect().x = 0;
    hero.get_velocity().y = JUMP_VELOCITY;
    is_keyup_event = false;
}

/*
 ****************************************************************************************
 * PinkManFallingState
 * **************************************************************************************
 */

PinkManFallingState::PinkManFallingState(bool is_keyup_event)
    : PinkManState(is_keyup_event)
{
}



PinkManFallingState::~PinkManFallingState() = default;

PinkManState* PinkManFallingState::process_input(PinkMan &hero, SDL_Event event)
{
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                hero.get_velocity().x = -JUMP_VELOCITY_X;
                hero.set_flip_flag(SDL_FLIP_HORIZONTAL);
                is_keyup_event = false;
                break;
            case SDLK_RIGHT:
                hero.get_velocity().x = JUMP_VELOCITY_X;
                hero.set_flip_flag(SDL_FLIP_NONE);
                is_keyup_event = false;
                break;
            default:
                break;
        }
    }
    else if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                hero.get_velocity().x = -JUMP_VELOCITY_X;
                is_keyup_event = true;
                break;
            case SDLK_RIGHT:
                hero.get_velocity().x = JUMP_VELOCITY_X;
                is_keyup_event = true;
                break;
            default:
                break;
        }
    }
    return nullptr;
}

void PinkManFallingState::update(PinkMan &hero, Uint64 deltaTime, vector<vector<My_Tile>> &tiles)
{
    PinkManState::update_(hero, deltaTime, FALLING_FRAMES);
    PinkManState::collide_(hero, deltaTime, tiles);

    if (hero.get_velocity().y == 0 && (hero.get_velocity().x == 0 || is_keyup_event)) {
        hero.update_state(new PinkManIdleState());
    }
    else if (hero.get_velocity().y == 0 && hero.get_flip_flag() == SDL_FLIP_NONE) {
        hero.update_state(new PinkManRightRunningState());
    }
    else if (hero.get_velocity().y == 0 && hero.get_flip_flag() == SDL_FLIP_HORIZONTAL) {
        hero.update_state(new PinkManLeftRunningState());
    }
}


void PinkManFallingState::enter(PinkMan &hero)
{
    hero.update_texture(FALLING);
    hero.get_src_rect().x = 0;
}