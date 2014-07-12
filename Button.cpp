/* 
 * File:   Button.cpp
 * Author: Jonathan Gutiérrez
 * 
 * Created on 11 de julio de 2014, 11:12 PM
 */

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_events.h>

#include "Button.h"

Button::Button() {
    this->state = BTN_STATE_NORMAL;
    
    SDL_Color black = {0, 0, 0, 255};
    SDL_Color white = {255, 255, 255, 255};
    this->colorNormal = white;
    this->colorHover = black;
    this->colorDown = white;
    this->colorUp = black;
    
    this->displayed = true;
}


Button::~Button() {
}

void Button::draw(SDL_Renderer* renderer) {
    if(!displayed) {
        return;
    }
    
    SDL_Color color;
    switch(state) {
        case BTN_STATE_HOVER:
            color = colorHover;
            break;
        case BTN_STATE_UP:
            color = colorUp;
            break;
        case BTN_STATE_DOWN:
            color = colorDown;
            break;
        case BTN_STATE_NORMAL:
        default:
            color = colorNormal;
            break;
    }
    
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &rect);
}

void Button::handleEvent(SDL_Event* event) {
    // If it isn't displayed, or event is not compatible, GG
    if(!displayed || event->type != SDL_MOUSEBUTTONDOWN && event->type != SDL_MOUSEBUTTONUP &&
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


void Button::setColorUp(SDL_Color colorUp) {
    this->colorUp = colorUp;
}

SDL_Color Button::getColorUp() {
    return colorUp;
}

void Button::setColorDown(SDL_Color colorDown) {
    this->colorDown = colorDown;
}

SDL_Color Button::getColorDown() {
    return colorDown;
}

void Button::setColorHover(SDL_Color colorHover) {
    this->colorHover = colorHover;
}

SDL_Color Button::getColorHover() {
    return colorHover;
}

void Button::setColorNormal(SDL_Color colorNormal) {
    this->colorNormal = colorNormal;
}

SDL_Color Button::getColorNormal() {
    return colorNormal;
}

SDL_Rect Button::getRect() {
    return rect;
}

void Button::setRect(SDL_Rect rect) {
    this->rect = rect;
}


ButtonState Button::getState() {
    return state;
}

void Button::setDisplayed(bool displayed) {
    this->displayed = displayed;
}
