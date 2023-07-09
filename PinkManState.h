//
// Created by sarah on 7/4/23.
//

#ifndef PINKMANSTATE_H
#define PINKMANSTATE_H
#include <SDL2/SDL.h>
class PinkMan;

class PinkManState {
public:
    virtual ~PinkManState()= default;
    virtual PinkManState* process_input(PinkMan& hero, SDL_Event event)=0;
    virtual void update(PinkMan& hero, Uint64 deltaTime)=0;

private:

};

class PinkManIdleState : public PinkManState {
public:
    PinkManIdleState()=default;
    ~PinkManIdleState() override;
    PinkManState* process_input(PinkMan& hero, SDL_Event event) override;
    void update(PinkMan& hero, Uint64 deltaTime) override;
};


class PinkManRightRunningState : public PinkManState {
public:
    PinkManRightRunningState()=default;
    ~PinkManRightRunningState() override;
    PinkManState* process_input(PinkMan& hero, SDL_Event event) override;
    void update(PinkMan& hero, Uint64 deltaTime) override;
};

class PinkManLeftRunningState : public PinkManState {
public:
    PinkManLeftRunningState()=default;
    ~PinkManLeftRunningState() override;
    PinkManState* process_input(PinkMan& hero, SDL_Event event) override;
    void update(PinkMan& hero, Uint64 deltaTime) override;
};

class PinkManRightJumpingState : public PinkManState {
public:
    PinkManRightJumpingState()=default;
    ~PinkManRightJumpingState() override;
    PinkManState* process_input(PinkMan& hero, SDL_Event event) override;
    void update(PinkMan& hero, Uint64 deltaTime) override;
};


#endif //PINKMANSTATE_H
