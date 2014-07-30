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

    //************************************
    // Generates a new position for food on the specified range
    // (window size)
    //************************************
    void generate(int max_x, int max_y, Snake* snake, Food* otherFood = NULL);

    //************************************
    // Draws the food into the renderer
    //************************************
    void draw(SDL_Renderer* renderer);

    //************************************
    // Sets a random effect to the food, not including EFFECT_NONE
    //************************************
    void setRandomEffect();

    //************************************
    // Gets the actual size of the food
    // Returns: int The size of the food (it's a square, it returns the width)
    //************************************
    int getSize();

    //************************************
    // Sets the size of the square
    // Parameter: int size The width an height value for the food's square
    //************************************
    void setSize(int size);

    SDL_Rect getRect() const { return rect; }
    void setRect(SDL_Rect val) { rect = val; }
    SDL_Color getColor() const { return color; }
    void setColor(SDL_Color val) { color = val; }
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