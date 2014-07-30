/*
* File:   Food.h
* Author: Jonathan Gutiérrez
*
* Created on 26 de julio de 2014, 04:21 PM
*
* AnEpicSnake v0.5-dev
*
* This file is part of AnEpicSnake, licensed under the GPLv3 license.
* See the NOTICE.txt file for more information.
*/

#pragma once
#ifndef _FOOD_H_
#define _FOOD_H_

enum Effect {
    EFFECT_NONE,
    EFFECT_5POINTS,
    EFFECT_SPEED_UP,
    EFFECT_SPEED_DOWN,
    EFFECT_TOTAL
};

#include "stdafx.h"
#include "Snake.h"

class Food {
public:
    Food();
    ~Food();

    // Generates a new position for food on the specified range
    // (window size)
    void generate(int max_x, int max_y, Snake* snake, Food* otherFood = NULL);

    // Draws the food into the renderer
    void draw(SDL_Renderer* renderer);

    // Sets a random effect to the food, not including EFFECT_NONE
    void setRandomEffect();

    SDL_Rect getRect();
    int getSize();
    void setSize(int size);
    SDL_Color getColor();
    void setColor(SDL_Color color);
    Effect getEffect() const { return effect; }
    void setEffect(Effect val) { effect = val; }
    bool isVisible() const { return visible; }
    void setVisible(bool val) { visible = val; }
private:
    SDL_Rect rect;
    SDL_Color color;

    Effect effect;
    bool visible;
};

#endif // _FOOD_H_