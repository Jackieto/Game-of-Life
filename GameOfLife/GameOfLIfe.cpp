

#include "pch.h"
#include "Game.h"
#include <cstdlib>
#include <iostream>



int main(int argc, char** argv)
{
	std::cout << "HELLO ";
	Game game;
	bool success = game.Initialize();
	if (success)
	{
		while (game.getExit() == false) {
			game.RunLoop();
		}
	}
	game.Shutdown();
	     
	return 0;

	}

