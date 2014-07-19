#pragma once
#include <SDL.h>
#include "Snake.h"

class Food
{
public:
	Food();
	~Food();

	// Generates a new position for food on the specified range
	// (window size)
	void generate(Snake* snake, int max_x, int max_y);
	void draw(SDL_Renderer* renderer);

	SDL_Rect getRect();
	int getSize();
	void setSize(int size);
	bool isSpecial();
	void setSpecial(bool special);
	SDL_Color getColor();
	void setColor(SDL_Color color);
private:
	bool special;
	SDL_Rect rect;
	SDL_Color color;
};

