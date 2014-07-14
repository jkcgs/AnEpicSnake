/* 
 * File:   Button.cpp
 * Author: Jonathan Gutiérrez
 * 
 * Created on 11 de julio de 2014, 11:12 PM
 * 
 * AnEpicSnake v0.3
 * 
 * This file is part of AnEpicSnake, licenced under the GPLv3 licence.
 * See the NOTICE.txt file for more information.
 */

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>

#include "Button.h"

Button::Button() {
    this->state = BTN_STATE_NORMAL;
    this->displayed = true;
}

Button::~Button() {
    free();
}

void Button::free() {
    texture.free();
}


void Button::draw(SDL_Renderer* renderer) {
    if(!displayed || this->texture.getTexture() == NULL) {
        return;
    }
    
    this->texture.setClip(this->stateTextureClips[state]);
    this->texture.draw(renderer);
}

void Button::handleEvent(SDL_Event* event) {
    // If it isn't displayed, or event is not compatible, GG
    if(!displayed || 
            event->type != SDL_MOUSEBUTTONDOWN && 
            event->type != SDL_MOUSEBUTTONUP &&
            event->type != SDL_MOUSEMOTION) {
        return;
    }
    
    // Check if mouse is hovering button. If not, button is normal
    if(event->motion.x >= texture.getRect().x && 
            event->motion.x < (texture.getRect().x + texture.getRect().w) && 
            event->motion.y >= texture.getRect().y && 
            event->motion.y < (texture.getRect().y + texture.getRect().h)) {
        if(event->type == SDL_MOUSEBUTTONDOWN) {
            state = BTN_STATE_DOWN;
        } else if(event->type == SDL_MOUSEBUTTONUP) {
            state = BTN_STATE_UP;
        } else {
            state = BTN_STATE_HOVER;
        }
    } else {
        state = BTN_STATE_NORMAL;
    }
}

bool Button::loadImage(SDL_Renderer* renderer, std::string path) {
    if(!texture.loadFromFile(renderer, path)) {
        return false;
    }
    
    SDL_Rect r = {0, 0, texture.getRect().w, texture.getRect().h};
    for(int i = 0; i < BTN_STATE_TOTAL; i++) {
        stateTextureClips[i] = r;
    }
    
    return true;
}

void Button::setTextureClip(State state, SDL_Rect clip) {
    if(state < BTN_STATE_TOTAL) {
        this->stateTextureClips[state] = clip;
    }
}

bool Button::isDisplayed() {
    return this->displayed;
}

Button::State Button::getState() {
    return state;
}

void Button::setState(State state) {
    this->state = state;
}

void Button::setDisplayed(bool displayed) {
    this->displayed = displayed;
}

void Button::setPos(int x, int y) {
    texture.setPos(x, y);
}

SDL_Rect Button::getRect() {
    return texture.getRect();
}

void Button::setTexture(Texture texture) {
    this->texture = texture;
}

void Button::setSize(int w, int h) {
    this->texture.setSize(w, h);
}
