/* 
 * File:   Texture.cpp
 * Author: Jonathan Gutiérrez
 * 
 * Created on 13 de julio de 2014, 05:11 PM
 * 
 * AnEpicSnake v0.5-dev
 * 
 * This file is part of AnEpicSnake, licensed under the GPLv3 license.
 * See the NOTICE.txt file for more information.
 */

#include "stdafx.h"
#include "Texture.h"

Texture::Texture() {
    texture = NULL;
    
    rect.x = 0;
    rect.y = 0;
    
    clip.x = 0;
    clip.y = 0;
    clip.w = 0;
    clip.h = 0;
}

Texture::~Texture() {
    free();
}

void Texture::free() {
    SDL_DestroyTexture(texture);
}

bool Texture::loadFromFile(SDL_Renderer* renderer, std::string path) {
    free();
    
    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    
    if(loadedSurface == NULL) {
        printf("Unable to load title image. Error: %s", IMG_GetError());
        return false;
    } else {
        texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if(texture == NULL) {
            printf("Unable to create texture from '%s'. Error: %s", path.c_str(), SDL_GetError());
            return false;
        } else {
            rect.w = loadedSurface->w;
            rect.h = loadedSurface->h;
            
            // Load default values for clip rectangle
            clip.w = loadedSurface->w;
            clip.h = loadedSurface->h;
        }
        
        SDL_FreeSurface(loadedSurface);
    }
    
    return true;
}

void Texture::draw(SDL_Renderer* renderer) {
    if(texture != NULL && renderer != NULL) {
        SDL_RenderCopy(renderer, texture, &clip, &rect);
    }
}

void Texture::setPos(int x, int y) {
    rect.x = x;
    rect.y = y;
}

void Texture::setClip(SDL_Rect clip) {
    this->clip = clip;
}

SDL_Rect Texture::getClip() {
    return clip;
}

SDL_Rect Texture::getRect() {
    return rect;
}

SDL_Texture* Texture::getTexture() {
    return texture;
}

void Texture::setTexture(SDL_Texture* texture) {
    this->texture = texture;
}

void Texture::setSize(int w, int h) {
    this->rect.w = w;
    this->rect.h = h;
}