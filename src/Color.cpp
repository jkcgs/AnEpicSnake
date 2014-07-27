/*
* File:   Color.cpp
* Author: Jonathan Gutiérrez
*
* AnEpicSnake v0.5-dev
*
* This file is part of AnEpicSnake, licensed under the GPLv3 license.
* See the NOTICE.txt file for more information.
*/
#include "Color.h"

const SDL_Color c_black = { 0, 0, 0, 255 };
const SDL_Color c_white = { 255, 255, 255, 255 };
const SDL_Color c_alpha(SDL_Color color, int alpha) {
    color.a = alpha;
    return color;
}