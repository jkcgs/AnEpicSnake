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
    effect = EFFECT_NONE;
    visible = true;
}

Food::~Food() { }

void Food::generate(int max_x, int max_y, Snake* snake, Food* otherFood) {
    do {
        this->rect.x = (rand() % ((max_x + 1) / getSize())) * getSize();
        this->rect.y = (rand() % ((max_y + 1) / getSize())) * getSize();
    } while (snake->collides(&this->rect) || (otherFood != NULL && snake->collides(&otherFood->getRect())));
}

void Food::draw(SDL_Renderer* renderer) {
    if (visible) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &rect);
    }
}

int Food::getSize() {
    return rect.w;
}

void Food::setSize(int size) {
    this->rect.w = size;
    this->rect.h = size;
}

void Food::setRandomEffect()
{
    effect = (Effect) ((rand() % (EFFECT_TOTAL - 1)) + 1);
}
