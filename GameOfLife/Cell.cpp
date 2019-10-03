#include "Cell.h"



Cell::Cell(): _state(false), _numAdy(0), _check(false)
{
	CellPos.x = -1;
	CellPos.y = -1;
	CellPos.v_pos_x = -1;
	CellPos.v_pos_y = -1;
	
}


Cell::Cell(const Cell& orig) {
	CellPos = orig.CellPos;
	_state = orig._state;
}


void Cell::setState(bool state) {
	this->_state = state;
}

bool Cell::getState() const {
	return _state;
}

void Cell::setPos(int x, int y, int vx, int vy) {
	this->CellPos.x = x;
	this->CellPos.y = y;
	this->CellPos.v_pos_x = vx;
	this->CellPos.v_pos_y = vy;
}

int Cell::getvx() const {
	return this->CellPos.v_pos_x;
}
int Cell::getvy() const {
	return this->CellPos.v_pos_y;
}

int Cell::getX() const {
	return this->CellPos.x;
}

int Cell::getY() const {
	return this->CellPos.y;
}

Cell Cell::operator=(Cell const &c1)   {

	this->CellPos = c1.CellPos;
	this->_state = c1._state;
	
	return *this;

}

void Cell::createRect(int x, int y, int w, int h) {
	temp = { x, y, w, h };
	_Rect = &temp;
}

SDL_Rect* Cell::getRect() const {
	return this->_Rect;
}

void Cell::setNumAdy(int x) {
	this->_numAdy = x;
}

int Cell::getNumAdy() const {
	return _numAdy;
}

void Cell::setCheck(bool check) {
	this->_check = check;
}

bool Cell::getCheck() const {
	return _check;
}

Cell::~Cell()
{
}


