

/*
#include "include/SDL.h"
#include <iostream>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGH = 480;

void createWindow() {
	SDL_Window* window = NULL;

	SDL_Surface* screenSurface = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		std::cout << "No se puede inicializar SDL" << std::endl;
		std::cout << "Error: " << SDL_GetError() << std::endl;
	}
	else
	{
		std::cout << "Game is loading" << std::endl;
		window = SDL_CreateWindow("WorldWar Sim", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGH, SDL_WINDOW_SHOWN);
		if (window == NULL) {
			std::cout << "No se ha podido crear la ventana!" << std::endl;
			std::cout << "Error: " << SDL_GetError() << std::endl;
		}
		else
		{
			screenSurface = SDL_GetWindowSurface(window);
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF));
			SDL_UpdateWindowSurface(window);
			SDL_Delay(20000);
		}
	}
	SDL_DestroyWindow(window);
	SDL_Quit();
}

*/