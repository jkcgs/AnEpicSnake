/*
* File:   Food.cpp
* Author: Jonathan Gutiérrez
*
* Created on 26 de julio de 2014, 04:21 PM
*
* AnEpicSnake v0.5-dev
*
* This file is part of AnEpicSnake, licensed under the GPLv3 license.
* See the NOTICE.txt file for more information.
*/

#include "stdafx.h"
#include "Food.h"

Food::Food() {
    SDL_Rect r = {0, 0, 10, 10};
    this->rect = r;
    type = TYPE_NORMAL;
}

Food::~Food() { }

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

SDL_Color Food::getColor() {
    return color;
}
void Food::setColor(SDL_Color color) {
    this->color = color;
}

FoodType Food::getType()
{
    return type;
}

void Food::setType(FoodType type)
{
    this->type = type;
}
