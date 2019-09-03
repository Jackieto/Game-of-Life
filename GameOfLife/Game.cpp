#include "Game.h"
#include <ctime>
#include <stdlib.h>
#include <cstdlib>

SDL_Point mouse_position;
constexpr static int WINH = 800;
constexpr static int WINW = 800;

constexpr static int up = - (WINH / 10);
constexpr static int down = WINH / 10;
constexpr static int numCells = (WINH / 10) * (WINW / 10);
constexpr static int top_left_corner = 0;
constexpr static int top_right_corner = (WINW / 10) - 1;
constexpr static int bottom_right_corner = ((WINH / 10) * (WINW / 10)) - 1;
constexpr static int bottom_left_corner = bottom_right_corner - ((WINW / 10) - 1);


Game::Game()
	:mWindow(nullptr)
	, mRenderer(nullptr)
	, mTicksCount(0)
	, mIsRunning(false)
	, mPause(true)
	, mExit(false)
{
}

bool Game::Initialize()
{
	// Initialize SDL
	int sdlResult = SDL_Init(SDL_INIT_VIDEO);
	if (sdlResult != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}
	SDL_Point window_position = {
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED
	};


	// Create an SDL Window
	mWindow = SDL_CreateWindow(
		"Game of Life BY JACKIETO", 
		window_position.x,	// Top left x-coordinate of window
		window_position.y,	// Top left y-coordinate of window
		WINW,	// Width of window
		WINH,	// Height of window
		0		// Flags (0 for no flags set)
	);

	if (!mWindow)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	//// Create SDL renderer
	mRenderer = SDL_CreateRenderer(
		mWindow, // Window to create renderer for
		-1,		 // Usually -1
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);

	if (!mRenderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}
	SDL_SetRenderDrawColor(mRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(mRenderer);

	/*
	TTF_Font* Font = TTF_OpenFont("Fonts\FONT1.ttf", 24);
	SDL_Color Red = { 255,0,0 };
	SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Font, "put your text here", Red);
	SDL_Texture* Message = SDL_CreateTextureFromSurface(mRenderer, surfaceMessage);
	SDL_Rect Message_rect;
	Message_rect.x = 0;  //controls the rect's x coordinate 
	Message_rect.y = 0; // controls the rect's y coordinte
	Message_rect.w = 100; // controls the width of the rect
	Message_rect.h = 100;
	SDL_RenderCopy(mRenderer, Message, NULL, &Message_rect);
	*/




	int x = 0;
	int y = 0;
	int tempsize = 0;
	int cont = 0;
		for (auto j = 0; j < numCells; j++) {
			std::vector<std::unique_ptr<Cell>> temp;
			auto c = std::make_unique<Cell>();
			c->setPos(x, y, Grid.size(),0);
			c->getX();
			c->getvx();
			c->createRect(x, y, 10, 10);
			SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 1);
			SDL_RenderDrawRect(mRenderer, c->getRect());
			SDL_RenderFillRect(mRenderer, c->getRect());
			temp.push_back((std::move(c)));
			if(cont > 79){
				cont = 0;
				tempsize++;
			}
			x += 10;
			if (x >= 800) {
				x = 0;
				y += 10;
			}
			Grid.push_back(std::move(temp));
			cont++;
		}
	SDL_RenderPresent(mRenderer);






	return true;
}

void Game::RunLoop()
{
	while (mPause && !mIsRunning) {
		ProcessInput();
		//getMousePos();
		const Uint8* state = SDL_GetKeyboardState(NULL);
		// PAUSE
		if (state[SDL_SCANCODE_SPACE])
		{
			if (!mPause) {
				mPause = true;
			}
			else {
				mPause = false;
				mIsRunning = true;
			}
		}
	}
	while (mIsRunning && !mPause) {
		GenerateOutput();
		PauseExit();
	}
}

void Game::PauseExit() {
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			// If we get an SDL_QUIT event, end loop
		case SDL_QUIT:
			mExit = true;
			break;
		}
	}

	const Uint8* state = SDL_GetKeyboardState(NULL);
	// PAUSE
	if (state[SDL_SCANCODE_P]) {
		mPause = true;
		mIsRunning = false;
	}

	if (state[SDL_SCANCODE_ESCAPE]) {
		mExit = true;
	}

}

void Game::ProcessInput()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			// If we get an SDL_QUIT event, end loop
		case SDL_QUIT:
			mExit = true;
			break;
		}
	}
	//Get State of mouse
	SDL_Point *b = new SDL_Point;
	std::vector<std::vector<std::unique_ptr<Cell>>>::iterator row;
	std::vector<std::unique_ptr<Cell>>::iterator col;
	Uint32 stateM = SDL_GetMouseState(&mouse_position.x, &mouse_position.y);

	int vx = 0;
	int vy = 0;

	//Create cells with left click
	if (event.button.button == SDL_BUTTON_LEFT) {
		b->x = mouse_position.x;
		b->y = mouse_position.y;

		for (row = Grid.begin(); row != Grid.end(); row++) {
			for (col = row->begin(); col != row->end(); col++) {
				if (SDL_PointInRect(b, (*col)->getRect())) {
					if ((*col)->getState() == false){
						(*col)->setState(true);
						vx = (*col)->getvx();
						vy = (*col)->getvy();
						SDL_Log("Vector X position: x=%i", (*col)->getvx());
						SDL_Log("Vector Y position: y=%i",(*col)->getvy());
						SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 1);
						SDL_RenderDrawRect(mRenderer, (*col)->getRect());
						SDL_RenderFillRect(mRenderer, (*col)->getRect());
						SDL_Log("Cell created at: x=%i, y=%i", (*col)->getX(), (*col)->getY());
						//numAdy(vx);
					}
				}
			}
		}
	}
	//Kill cells with right click
	if (event.button.button == SDL_BUTTON_RIGHT) {
		b->x = mouse_position.x;
		b->y = mouse_position.y;

		for (row = Grid.begin(); row != Grid.end(); row++) {
			for (col = row->begin(); col != row->end(); col++) {
				if (SDL_PointInRect(b, (*col)->getRect())) {
					if ((*col)->getState() == true) {
						(*col)->setState(false);
						SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 1);
						SDL_RenderDrawRect(mRenderer, (*col)->getRect());
						SDL_RenderFillRect(mRenderer, (*col)->getRect());
						SDL_Log("Cell died at: x=%i, y=%i", (*col)->getX(), (*col)->getY());
					}
				}
			}
		}
	}
	delete b;
	drawGrid();
}

void Game::GenerateOutput()
{
	std::vector<std::vector<std::unique_ptr<Cell>>>::iterator row;
	std::vector<std::unique_ptr<Cell>>::iterator col;

	int vx = 0;
	for (row = Grid.begin(); row != Grid.end(); row++) {
		for (col = row->begin(); col != row->end(); col++) {
			vx = (*col)->getvx();
			(*col)->setNumAdy(numAdy(vx));
			if ((*col)->getState() == true) {
				if ((*col)->getNumAdy() < 2) {
					(*col)->setCheck(false);
				}
				if ((*col)->getNumAdy() > 3) {
					(*col)->setCheck(false);
				}
				if (((*col)->getNumAdy() == 3)) {
					(*col)->setCheck(true);
				}
				if (((*col)->getNumAdy() == 2)) {
					(*col)->setCheck(true);
				}	
			}
			if ((*col)->getState() == false) {
				if ((*col)->getNumAdy() == 3) {
					(*col)->setCheck(true);
				}
			}
		}
	}
	for (row = Grid.begin(); row != Grid.end(); row++) {
		for (col = row->begin(); col != row->end(); col++) {
			vx = (*col)->getvx();
			if ((*col)->getCheck() == true) {
				(*col)->setState(true);
				drawCell(vx);
				
			}
			if ((*col)->getCheck() == false) {
				(*col)->setState(false);
				eraseCell(vx);
			}
		}
	}
	drawGrid();
}

void Game::drawCell(int x) {
	SDL_SetRenderDrawColor(mRenderer, 0, 0, 0, 1);
	SDL_RenderFillRect(mRenderer, Grid[x][0]->getRect());
}

void Game::eraseCell(int x) {
	SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 1);
	SDL_RenderFillRect(mRenderer, Grid[x][0]->getRect());
}


void Game::drawGrid() {

	//DRAW THE GRID

	SDL_SetRenderDrawColor(mRenderer, 110, 110, 110, 1);

	//HORIZONTAL LINES
	for (int i = 0; i < WINH; i += 10) {
		SDL_RenderDrawLine(mRenderer, 0, WINH / 2 + i, WINW, WINH / 2 + i);
	}

	for (int j = 0; j < WINH; j += 10) {
		SDL_RenderDrawLine(mRenderer, 0, WINH / 2 - j, WINW, WINH / 2 - j);
	}
	//VERTICAL LINES
	for (int i = 0; i < WINW; i += 10) {
		SDL_RenderDrawLine(mRenderer, WINW / 2 + i, 0, WINW / 2 + i, WINH);
	}

	for (int j = 0; j < WINH; j += 10) {
		SDL_RenderDrawLine(mRenderer, WINW / 2 - j, 0, WINW / 2 - j, WINH);
	}
	SDL_RenderPresent(mRenderer);
}

int Game::numAdy(int x) {
	int numADY = 0;
	int tempx = 0;
	int pos = 0;
	int ele = 0;

	bool existsUpperRight = false;
	bool existsbottomrow = false;
	bool existstoprow = false;
	bool existsdownleft = false;
	bool existsdownright = false;
	bool existsUpperLeft = false;
	bool existsRightColumn = false;
	bool elsewhere = false;

	int toprow[(WINH / 10) - 2];
	tempx = top_left_corner + 1;
	int bottomrow[(WINH / 10) - 2];

	for (int i = 0; i < (WINH / 10) - 2; i++) {
		toprow[i] = tempx;
		tempx++;
	}
	for (int i = 0; i < (WINH / 10) - 2; i++) {
		if (toprow[i] == x) {
			existstoprow = true;
			pos = i;
		}
	}
	tempx = bottom_left_corner + 1;

	for (int i = 0; i < (WINH / 10) - 2; i++) {
		bottomrow[i] = tempx;
		tempx++;
	}
	for (int i = 0; i < (WINH / 10) - 2; i++) {
		if (bottomrow[i] == x) {
			existsbottomrow = true;
			pos = i;
		}
	}
	int leftColumn[(WINH / 10) - 2];
	tempx = 0;
	for (int i = 80; i <= 6240; i += 80) {
		leftColumn[tempx] = i;
		tempx++;
	}
	bool existsLeftColumn = false;
	for (int i = 0; i < (WINH / 10) - 2; i++) {
		if (leftColumn[i] == x) {
			existsLeftColumn = true;
			pos = i;
		}
	}
	int rightColumn[(WINH / 10) - 2];
	tempx = 0;
	for (int i = 159; i <= 6319; i += 80) {
		rightColumn[tempx] = i;
		tempx++;
	}

	for (int i = 0; i < 78; i++) {
		if (rightColumn[i] == x) {
			existsRightColumn = true;
			pos = i;
		}
	}
	//UpperLeft corner cell, can only check for down, right and diagonaldownright cells
	if (x == top_left_corner){
		existsUpperLeft = true;
		tempx = x + 1;
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		tempx = x + down;
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		tempx = x + (down + 1);
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}

		tempx = bottom_right_corner;
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		tempx = bottom_right_corner - 1;
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}


	}
	//UpperRight corner cell, can only check for down, left and diagonaldownleft cells

	if (x == top_right_corner) {
		existsUpperRight = true;

		tempx = x - 1;
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		tempx = x + down;
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		tempx = x + (down - 1);
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}

		tempx = bottom_left_corner;
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}

		tempx = bottom_left_corner + 1;
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
	}
	//top row cells, can only check for down, right, left, diagonaldownright and diagonaldownleft cells

	if (existstoprow) {
		tempx = x + down;
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		tempx = x + 1;
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		tempx = x - 1;
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		tempx = x + (down + 81);
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		tempx = x + (down - 1);
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		tempx = bottomrow[pos];
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		if (Grid[tempx + 1][0].get()->getState() == true) {
			numADY++;
		}
		if (Grid[tempx - 1][0].get()->getState() == true) {
			numADY++;
		}
	}

	//bottom row cells, can only check for up, right, left, diagonalupright and diagonalupleft cells

	if (existsbottomrow) {
		tempx = x + up;
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		tempx = x + 1;
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		tempx = x - 1;
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		tempx = x + (up - 1);
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		tempx = x + (up + 1);
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		tempx = toprow[pos];
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		if (Grid[tempx + 1][0].get()->getState() == true) {
			numADY++;
		}
		if (Grid[tempx - 1][0].get()->getState() == true) {
			numADY++;
		}
	}
	//left column cells, can only check for right, up, down, diagonalupright, diagonaldownright cells
	if (existsLeftColumn == true) {
		tempx = x + 1;
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		tempx = x + up;
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		tempx = x + down;
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		tempx = x +(up + 1);
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		tempx = x + (down + 1);
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		tempx = rightColumn[pos];
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		if (Grid[tempx + up][0].get()->getState() == true) {
			numADY++;
		}
		if (Grid[tempx + down][0].get()->getState() == true) {
			numADY++;
		}

	}
	//downLeft corner cell, can only check for up, right and diagonalupright cells
	if (x == bottom_left_corner) {
		existsdownleft = true;

		tempx = top_left_corner;

		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}

		if (Grid[tempx + 1][0].get()->getState() == true) {
			numADY++;
		}
		tempx = x + up;
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		tempx = x + 1;
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		tempx = x + (up + 1);
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
	}
	//downRight corner cell, can only check for up, left and diagonalupleft cells

	if (x == bottom_right_corner) {
		existsdownright = true;

		tempx = top_right_corner;

		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		if (Grid[tempx - 1][0].get()->getState() == true) {
			numADY++;
		}
		tempx = x + up;
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		tempx = x - 1;
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		tempx = x + (up - 1);
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
	}
	//right column cells, can only check for left, up, down, diagonalupleft, diagonaldownleft cells
	
	if (existsRightColumn == true) {
		
		//cheks for adyacents elements in the opposite colums
		
		tempx = x - 1;
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		tempx = x + up;
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		tempx = x + down;
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		tempx = x + (up - 1);
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		tempx = x + (down - 1);
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		//cheks for adyacents elements in the opposite colums
		tempx = leftColumn[pos];
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		if (Grid[tempx + up][0].get()->getState() == true) {
			numADY++;
		}
		if (Grid[tempx + down][0].get()->getState() == true) {
			numADY++;
		}
	}

	if (!existsUpperRight && !existsUpperLeft && !existsRightColumn && !existsLeftColumn && !existsdownright && !existsdownleft && !existstoprow && !existsbottomrow) {
		elsewhere = true;
	}
	if (elsewhere) {
		tempx = x + 1;
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		tempx = x - 1;
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}

		tempx = x + up;
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		tempx = x + down;
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
	
		tempx = x + (up - 1);
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		tempx = x + (down + 1);
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}

		tempx = x + (down - 1);
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
		tempx = x + (up + 1);
		if (Grid[tempx][0].get()->getState() == true) {
			numADY++;
		}
	}
		return numADY;
	}

bool Game::getExit() const {
	return mExit;
}

void Game::getMousePos() {

	Uint32 stateM = SDL_GetMouseState(&mouse_position.x, &mouse_position.y);
	
	SDL_Log("Mouse pos at: x=%i, y=%i", mouse_position.x, mouse_position.y);

}

void Game::Shutdown()
{
	Grid.clear();
	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	//SDL_DestroyTexture(Message);
	SDL_Quit();
}