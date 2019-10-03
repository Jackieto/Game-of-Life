#pragma once
#include "SDL.h"

struct Vector2
{
	int x;
	int y;
	int v_pos_x;
	int v_pos_y;
};

class Cell
{
public:
	Cell();
	Cell(const Cell& orig);
	~Cell();

	Cell operator=(Cell const &c);

	int getX() const;
	int getY() const;
	int getvx() const;
	int getvy() const;

	void setNumAdy(int x);
	int getNumAdy() const;

	void createRect(int x, int y, int w, int h);
	SDL_Rect* getRect() const;

	void setState(bool state);
	bool getState() const ;

	void setCheck(bool check);
	bool getCheck() const;

	void setPos(int x, int y, int vx, int vy);

	


private:
	SDL_Rect* _Rect;
	SDL_Rect temp;
	bool _state;
	bool _check;
	Vector2 CellPos;
	int _numAdy;
};

