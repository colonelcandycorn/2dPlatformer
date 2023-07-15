//
// Created by sarah on 7/4/23.
//

#ifndef PINKMANSTATE_H
#define PINKMANSTATE_H
#include <SDL2/SDL.h>
#include <vector>
#include <algorithm>

class PinkMan;
class Tile;

using std::vector;
using std::sort;

class PinkManState {
public:
    PinkManState();
    explicit PinkManState(bool is_keyup_event);
    virtual ~PinkManState()= default;
    virtual PinkManState* process_input(PinkMan& hero, SDL_Event event)=0;
    virtual void update(PinkMan& hero, Uint64 deltaTime, vector<Tile> &tiles)=0;
    virtual void enter(PinkMan& hero)=0;

protected:
    void update_(PinkMan& hero, Uint64 deltaTime, int frames);
    void collide_(PinkMan& hero, Uint64 deltaTime, std::vector<Tile> &tiles);

    bool is_keyup_event;
};

class PinkManIdleState : public PinkManState {
public:
    PinkManIdleState()=default;
    ~PinkManIdleState() override;
    PinkManState* process_input(PinkMan& hero, SDL_Event event) override;
    void update(PinkMan& hero, Uint64 deltaTime, vector<Tile> &tiles) override;
    void enter(PinkMan& hero) override;
};


class PinkManRightRunningState : public PinkManState {
public:
    PinkManRightRunningState()=default;
    ~PinkManRightRunningState() override;
    PinkManState* process_input(PinkMan& hero, SDL_Event event) override;
    void update(PinkMan& hero, Uint64 deltaTime, vector<Tile> &tiles) override;
    void enter(PinkMan& hero) override;

};

class PinkManLeftRunningState : public PinkManState {
public:
    PinkManLeftRunningState()=default;
    ~PinkManLeftRunningState() override;
    PinkManState* process_input(PinkMan& hero, SDL_Event event) override;
    void update(PinkMan& hero, Uint64 deltaTime, vector<Tile> &tiles) override;
    void enter(PinkMan& hero) override;
};

class PinkManJumpingState : public PinkManState {
public:
    PinkManJumpingState()=default;
    ~PinkManJumpingState() override;
    PinkManState* process_input(PinkMan& hero, SDL_Event event) override;
    void update(PinkMan& hero, Uint64 deltaTime, vector<Tile> &tiles) override;
    void enter(PinkMan& hero) override;
};

class PinkManFallingState : public PinkManState
{
public:
    PinkManFallingState()=default;
    PinkManFallingState(bool is_keyup_event);
    ~PinkManFallingState() override;
    PinkManState* process_input(PinkMan& hero, SDL_Event event) override;
    void update(PinkMan& hero, Uint64 deltaTime, vector<Tile> &tiles) override;
    void enter(PinkMan& hero) override;
};

#endif //PINKMANSTATE_H
