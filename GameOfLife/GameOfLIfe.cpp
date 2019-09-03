// WorldWar Sim.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//


#include "pch.h"
#include "Game.h"
#include <stdio.h>
#include <iostream>

#include "Game.h"

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

