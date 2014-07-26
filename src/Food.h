/*
* File:   Food.h
* Author: Jonathan Gutiérrez
*
* Created on 26 de julio de 2014, 04:21 PM
*
* AnEpicSnake v0.5-dev
*
* This file is part of AnEpicSnake, licenced under the GPLv3 licence.
* See the NOTICE.txt file for more information.
*/

#pragma once
#ifndef _FOOD_H_
#define _FOOD_H_

#include "stdafx.h"
#include "Snake.h"

class Food {
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

#endif // _FOOD_H_