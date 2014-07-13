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
    
    for(int i = 0; i < BTN_STATE_TOTAL; i++) {
        this->stateTextureClips[i] = NULL;
    }
    
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
        SDL_RenderCopy(renderer, this->stateTexture, this->stateTextureClips[state], rect);
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
    int x, y;
    SDL_GetMouseState(&x, &y);
    
    // Check if mouse is hovering button. If not, button is normal
    if(x >= rect.x && x < (rect.x + rect.w) && y >= rect.y && y < (rect.y + rect.h)) {
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

void Button::setDisplayed(bool displayed) {
    this->displayed = displayed;
}