#include "Game.h"

int main(int argc, char** argv)
{
	Game game;

    game.init();
    game.run();
    game.take_down();

	return 0;
}
