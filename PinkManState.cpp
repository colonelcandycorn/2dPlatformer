//
// Created by sarah on 7/4/23.
//

#include "PinkManState.h"
#include "PinkMan.h"
#include <iostream>

/*
 ****************************************************************************************
 * PinkManState
 * **************************************************************************************
 */
void PinkManState::update_(PinkMan &hero, Uint64 deltaTime, int frames)
{
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

/*
 ****************************************************************************************
 * PinkManIdleState
 * **************************************************************************************
 */
PinkManIdleState::~PinkManIdleState() = default;

void PinkManIdleState::update(PinkMan& hero, Uint64 deltaTime)
{
    PinkManState::update_(hero, deltaTime, IDLE_FRAMES);
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

void PinkManRightRunningState::update(PinkMan &hero, Uint64 deltaTime)
{
    PinkManState::update_(hero, deltaTime, RUNNING_FRAMES);

}

PinkManState* PinkManRightRunningState::process_input(PinkMan& hero, SDL_Event event)
{
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                return new PinkManLeftRunningState();
            case SDLK_RIGHT:
                hero.get_velocity().x = 170;
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
                hero.get_velocity().x = -170;
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

void PinkManLeftRunningState::update(PinkMan &hero, Uint64 deltaTime)
{
    PinkManState::update_(hero, deltaTime, RUNNING_FRAMES);

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
               // hero.get_velocity().x = -85;
                hero.set_flip_flag(SDL_FLIP_HORIZONTAL);
                break;
            case SDLK_RIGHT:
                //hero.get_velocity().x = 85;
                hero.set_flip_flag(SDL_FLIP_NONE);
                break;
            default:
                break;
        }
    }
    return nullptr;
}

void PinkManJumpingState::update(PinkMan &hero, Uint64 deltaTime)
{
    hero.get_velocity().y += GRAVITY;

    if (hero.get_velocity().y > 0)
        hero.update_state(new PinkManFallingState());

}

void PinkManJumpingState::enter(PinkMan &hero)
{
    hero.update_texture(JUMPING);
    hero.get_src_rect().x = 0;
    hero.get_velocity().y = JUMP_VELOCITY;
}

/*
 ****************************************************************************************
 * PinkManFallingState
 * **************************************************************************************
 */

PinkManFallingState::~PinkManFallingState() = default;

PinkManState* PinkManFallingState::process_input(PinkMan &hero, SDL_Event event)
{
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                //hero.get_velocity().x = -85;
                hero.set_flip_flag(SDL_FLIP_HORIZONTAL);
                break;
            case SDLK_RIGHT:
                //hero.get_velocity().x = 85;
                hero.set_flip_flag(SDL_FLIP_NONE);
                break;
            default:
                break;
        }
    }
    return nullptr;
}

void PinkManFallingState::update(PinkMan &hero, Uint64 deltaTime)
{
    hero.get_velocity().y += GRAVITY;

}

void PinkManFallingState::enter(PinkMan &hero)
{
    hero.update_texture(FALLING);
    hero.get_src_rect().x = 0;
}