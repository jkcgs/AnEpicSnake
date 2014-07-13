/* 
 * File:   Texture.h
 * Author: Jonathan Gutiérrez
 *
 * Created on 13 de julio de 2014, 05:11 PM
 */

#ifndef TEXTURE_H
#define	TEXTURE_H

#include <stdio.h>
#include <string>
#include <SDL2/SDL_image.h>

class Texture {
public:
    Texture();
     ~Texture();
     
     bool loadFromFile(SDL_Renderer* renderer, std::string path);
     void free();
     void draw(SDL_Renderer* renderer);
     void setPos(int x, int y);
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

