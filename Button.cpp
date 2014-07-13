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
    
    SDL_Color black = {0, 0, 0, 255};
    SDL_Color white = {255, 255, 255, 255};
    
    this->colorStates[BTN_STATE_NORMAL] = black;
    this->colorStates[BTN_STATE_HOVER] = white;
    this->colorStates[BTN_STATE_DOWN] = black;
    this->colorStates[BTN_STATE_UP] = black;
    
    this->stateTexture = NULL;
    this->displayed = true;
}


Button::~Button() {
}

void Button::draw(SDL_Renderer* renderer) {
    if(!displayed) {
        return;
    }
    
    if(this->stateTexture == NULL) {
        SDL_Color color = this->colorStates[state];

        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &rect);
    } else {
        SDL_RenderCopy(renderer, this->stateTexture, &this->stateTextureClips[state], &rect);
    }
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
    if(event->motion.x >= rect.x && event->motion.x < (rect.x + rect.w) && event->motion.y >= rect.y && event->motion.y < (rect.y + rect.h)) {
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

SDL_Rect Button::getRect() {
    return rect;
}

void Button::setRect(SDL_Rect rect) {
    this->rect = rect;
}

bool Button::isDisplayed() {
    return this->displayed;
}


void Button::setDisplayed(bool displayed) {
    this->displayed = displayed;
}

void Button::setTexture(SDL_Texture* texture) {
    this->stateTexture = texture;
}

void Button::setTextureClip(State state, SDL_Rect clip) {
    if(state < BTN_STATE_TOTAL) {
        this->stateTextureClips[state] = clip;
    }
}

void Button::setColorState(State state, SDL_Color color) {
    if(state < BTN_STATE_TOTAL) {
        this->colorStates[state] = color;
    }
}
