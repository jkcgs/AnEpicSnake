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
     
     //************************************
     // Loads a image file to the texture
     // Parameter: SDL_Renderer* renderer The render to process the image
     // Parameter: std::string path The image path
     // Return bool The success of the process
     //************************************
     bool loadFromFile(SDL_Renderer* renderer, std::string path);

     //************************************
     // Draws the texture to the desired renderer
     // Parameter: SDL_Renderer* renderer The renderer to draw the texture on
     //************************************
     void draw(SDL_Renderer* renderer);

     //************************************
     // Frees the memory used by the texture
     //************************************
     void free();

     //************************************
     // Sets the position to draw the texture
     // Parameter: int x The X position for the texture
     // Parameter: int y The Y position for the texture
     //************************************
     void setPos(int x, int y);

     //************************************
     // Sets the size of the texture
     // Parameter: int w The width of the texture
     // Parameter: int h The height of the texture
     //************************************
     void setSize(int w, int h);

     SDL_Texture* getTexture() const { return texture; }
     void setTexture(SDL_Texture* val) { texture = val; }
     SDL_Rect getRect() const { return rect; }
     void setRect(SDL_Rect val) { rect = val; }
     SDL_Rect getClip() const { return clip; }
     void setClip(SDL_Rect val) { clip = val; }
private:
    SDL_Texture* texture;
    SDL_Rect rect;
    SDL_Rect clip;
};

#endif	/* TEXTURE_H */

