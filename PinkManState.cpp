//
// Created by sarah on 7/4/23.
//

#include "PinkManState.h"
#include "PinkMan.h"
#include <iostream>

PinkManIdleState::~PinkManIdleState() = default;

void PinkManIdleState::update(PinkMan& hero, Uint64 deltaTime)
{
    static int time_since_last_frame = 0;

    time_since_last_frame += deltaTime;

    if (time_since_last_frame > 48 ) {
        time_since_last_frame = 0;
        hero.get_src_rect().x += PINKMAN_WIDTH;
        if (hero.get_src_rect().x >= PINKMAN_WIDTH * IDLE_FRAMES) {
            hero.get_src_rect().x = 0;
        }
    }
}

PinkManState* PinkManIdleState::process_input(PinkMan &hero, SDL_Event event)
{
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                hero.set_flip_flag(SDL_FLIP_HORIZONTAL);
                hero.update_texture(RUNNING);
                hero.get_src_rect().x = 0;
                hero.get_velocity().x = -170;
                return new PinkManLeftRunningState();
                break;
            case SDLK_RIGHT:
                hero.set_flip_flag(SDL_FLIP_NONE);
                hero.update_texture(RUNNING);
                hero.get_src_rect().x = 0;
                hero.get_velocity().x = 170;
                return new PinkManRightRunningState();
                break;
            default:
                break;
        }
    }
    return nullptr;
}


PinkManRightRunningState::~PinkManRightRunningState() = default;

void PinkManRightRunningState::update(PinkMan &hero, Uint64 deltaTime)
{
    static int time_since_last_frame = 0;

    time_since_last_frame += deltaTime;

    if (time_since_last_frame > 48 ) {
        time_since_last_frame = 0;
        hero.get_src_rect().x += PINKMAN_WIDTH;
        if (hero.get_src_rect().x >= PINKMAN_WIDTH * RUNNING_FRAMES) {
            hero.get_src_rect().x = 0;
        }
    }

    hero.update_position(deltaTime);
}

PinkManState* PinkManRightRunningState::process_input(PinkMan& hero, SDL_Event event)
{
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                hero.set_flip_flag(SDL_FLIP_HORIZONTAL);
                hero.get_velocity().x = -170;
                return new PinkManLeftRunningState();
                break;
            case SDLK_RIGHT:
                hero.get_velocity().x = 170;
                break;
            default:
                break;
        }
    }
    else if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
            case SDLK_RIGHT:
                hero.get_velocity().x = 0;
                hero.update_texture(IDLE);
                hero.get_src_rect().x = 0;
                return new PinkManIdleState();
                break;
            default:
                break;
        }
    }
    return nullptr;
}

PinkManLeftRunningState::~PinkManLeftRunningState() = default;

PinkManState* PinkManLeftRunningState::process_input(PinkMan &hero, SDL_Event event)
{
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                hero.get_velocity().x = -170;
                break;
            case SDLK_RIGHT:
                hero.set_flip_flag(SDL_FLIP_NONE);
                hero.get_velocity().x = 170;
                return new PinkManRightRunningState();
                break;
            default:
                break;
        }
    }
    else if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                hero.get_velocity().x = 0;
                hero.update_texture(IDLE);
                hero.get_src_rect().x = 0;
                return new PinkManIdleState();
                break;
            default:
                break;
        }
    }
    return nullptr;
}

void PinkManLeftRunningState::update(PinkMan &hero, Uint64 deltaTime)
{
    static int time_since_last_frame = 0;

    time_since_last_frame += deltaTime;

    if (time_since_last_frame > 48 ) {
        time_since_last_frame = 0;
        hero.get_src_rect().x += PINKMAN_WIDTH;
        if (hero.get_src_rect().x >= PINKMAN_WIDTH * RUNNING_FRAMES) {
            hero.get_src_rect().x = 0;
        }
    }

    hero.update_position(deltaTime);
}