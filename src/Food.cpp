#include "Food.h"
#include <stdlib.h>

Food::Food()
{
	SDL_Rect r = {0, 0, 10, 10};
	this->rect = r;
	special = false;
}

Food::~Food()
{
}

void Food::generate(Snake* snake, int max_x, int max_y) {
	do {
		this->rect.x = (rand() % ((max_x + 1) / getSize())) * getSize();
		this->rect.y = (rand() % ((max_y + 1) / getSize())) * getSize();
	} while (snake->collides(&this->rect));
}

void Food::draw(SDL_Renderer* renderer) {
	SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
	SDL_RenderFillRect(renderer, &rect);
}

SDL_Rect Food::getRect() {
	return rect;
}

int Food::getSize() {
	return rect.w;
}

void Food::setSize(int size) {
	this->rect.w = size;
	this->rect.h = size;
}

bool Food::isSpecial() {
	return special;
}

void Food::setSpecial(bool special) {
	this->special = special;
}

SDL_Color Food::getColor() {
	return color;
}
void Food::setColor(SDL_Color color) {
	this->color = color;
}