/* 
 * File:   Snake.cpp
 * Author: Jonathan Gutiérrez
 * 
 * Created on 28 de junio de 2014, 08:59 PM
 * 
 * AnEpicSnake v0.5-dev
 * 
 * This file is part of AnEpicSnake, licensed under the GPLv3 license.
 * See the NOTICE.txt file for more information.
 */

#include "stdafx.h"
#include "Snake.h"

Snake::Snake() {
    reset();
}

Snake::~Snake() {
}

void Snake::move() {
    // If the snake growment is not enabled, remove the last square on move
    if(!grow) {
        points.pop_back();
    } else {
        // If the snake have grown, stop it
        grow = false;
    }

    SDL_Point n;
    switch(direction) {
        case UP:
        case DOWN:
            n.x = points[0].x;
            n.y = points[0].y + (direction == DOWN ? 1 : -1);
            break;
        case LEFT:
        case RIGHT:
            n.x = points[0].x + (direction == RIGHT ? 1 : -1);
            n.y = points[0].y;
            
    }

    // Reserve one more space for points vector
    points.reserve(points.size()+1);
    // Add the new position at the first place
    points.insert(points.begin(), n);
    // Move the tongue with the new first point position
    moveTongue();
    // The snake is now moved, this must be updated by direction changement
    moved = true;
}

void Snake::draw(SDL_Renderer* renderer) {
    // Draw each point of the square
    for(Uint16 i = 0; i < points.size(); i++) {
        p.x = points[i].x*size;
        p.y = points[i].y*size;
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &p);
    }
    
    // Draw eyes
    if(direction == UP || direction == LEFT) {
        // Sets a square in the up-left first point corner
        a.x = points[0].x * size + a.w;
        a.y = points[0].y * size + a.h;
    } else {
        // Sets a square in the down-right first point corner
        a.x = points[0].x * size + size - a.w * 2;
        a.y = points[0].y * size + size - a.h * 2;
    }
    
    if(direction == UP || direction == RIGHT) {
        // Sets a square in the up-right first point corner
        b.x = points[0].x * size + size - b.w * 2;
        b.y = points[0].y * size + b.h;
    } else {
        // Sets a quare in the down-left first point corner
        b.x = points[0].x * size + b.w;
        b.y = points[0].y * size + size - b.h * 2;
    }
    
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderFillRect(renderer, &a);
    SDL_RenderFillRect(renderer, &b);
    SDL_RenderFillRect(renderer, &tongue);
}

void Snake::moveTongue() {
    if(direction == UP || direction == DOWN) {
        tongue.w = size / 5;
        tongue.h = size / 2;
        tongue.x = points[0].x * size + (size / 2) - (tongue.w / 2);
    } else {
        tongue.w = size / 2;
        tongue.h = size / 5;
        tongue.y = points[0].y * size + (size / 2) - (tongue.h / 2);
    }
    
    switch(direction) {
        case UP:
            tongue.y = points[0].y * size - tongue.h;
            break;
        case DOWN:
            tongue.y = points[0].y * size + size;
            break;
        case LEFT:
            tongue.x = points[0].x * size - tongue.w;
            break;
        case RIGHT:
            tongue.x = points[0].x * size + size;
            break;
    }
}

void Snake::reset() {
    // Reset every aspect of the snake to its defaults
    // TODO: receive parameters for speed, size and direction
    direction = RIGHT;
    speed = 10;
    size = 10;
    grow = false;
    
    p.w = size;
    p.h = size;
    a.w = size / 5;
    a.h = a.w;
    b.w = a.w;
    b.h = a.w;
    
    points.resize(5);
    points[0].x = 5; points[0].y = 5;
    points[1].x = 4; points[1].y = 5;
    points[2].x = 3; points[2].y = 5;
    points[3].x = 2; points[3].y = 5;
    points[4].x = 1; points[4].y = 5;
    
    moveTongue();
}

void Snake::setSize(int size) {
    this->size = size;
    p.w = size;
    p.h = size;
}

bool Snake::selfCrashed() {
    // if the head is at the same place of some point of the snake, it have
    // self crashed
    for(Uint16 i = 1; i < points.size(); i++) {
        if(points[0].x == points[i].x && points[0].y == points[i].y) {
            return true;
        }
    }

    return false;
}

int Snake::length() {
    return (int) points.size();
}

bool Snake::collides(SDL_Rect* rect) {
    // checks if some point is at the same place of some point of the snake
    for(Uint16 i = 0; i < points.size(); i++) {
        if(points[i].x*size == rect->x && points[i].y*size == rect->y) {
            return true;
        }
    }

    return false;
}

SDL_Point Snake::getFirstPoint() {
    // gets the head of the snake
    return points[0];
}

bool Snake::collidesWithHead(SDL_Rect* rect) {
    return (points[0].x*size == rect->x && points[0].y*size == rect->y);
}
