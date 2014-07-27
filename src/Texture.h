/* 
 * File:   Texture.h
 * Author: Jonathan Gutiérrez
 *
 * Created on 13 de julio de 2014, 05:11 PM
 * 
 * AnEpicSnake v0.5-dev
 * 
 * This file is part of AnEpicSnake, licensed under the GPLv3 license.
 * See the NOTICE.txt file for more information.
 */

#ifndef TEXTURE_H
#define	TEXTURE_H

#include "stdafx.h"

class Texture {
public:
    Texture();
     ~Texture();
     
     bool loadFromFile(SDL_Renderer* renderer, std::string path);
     void free();
     void draw(SDL_Renderer* renderer);
     void setPos(int x, int y);
     void setSize(int w, int h);
     SDL_Rect getClip();
     void setClip(SDL_Rect clip);
     SDL_Rect getRect();
     SDL_Texture* getTexture();
     void setTexture(SDL_Texture* texture);
private:
    SDL_Texture* texture;
    SDL_Rect rect;
    SDL_Rect clip;
};

#endif	/* TEXTURE_H */

