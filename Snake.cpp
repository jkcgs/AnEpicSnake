/* 
 * File:   Snake.cpp
 * Author: Jonathan Gutiérrez
 * 
 * Created on 28 de junio de 2014, 08:59 PM
 * 
 * AnEpicSnake v0.1
 * 
 * This file is part of AnEpicSnake, licenced under the GPLv3 licence.
 * See the NOTICE.txt file for more information.
 */

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
            n.x = points[0].x;
            n.y = points[0].y - 1;
            break;
        case RIGHT:
            n.x = points[0].x + 1;
            n.y = points[0].y;
            break;
        case DOWN:
            n.x = points[0].x;
            n.y = points[0].y + 1;
            break;
        case LEFT:
        default:
            n.x = points[0].x - 1;
            n.y = points[0].y;
            break;
            
    }

    // Reserve one more space for points vector
    points.reserve(points.size()+1);
    // Add the new position at the first place
    points.insert(points.begin(), n);
    // The snake is now moved, this must be updated by direction changement
    moved = true;
}

void Snake::draw(SDL_Renderer* renderer) {
    // Draw each point of the square
    for(int i = 0; i < points.size(); i++) {
        SDL_Rect r = {points[i].x*size, points[i].y*size, size, size};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &r);
    }
}

void Snake::reset() {
    // Reset every aspect of the snake to its defaults
    // TODO: receive parameters for speed, size and direction
    direction = RIGHT;
    speed = 10;
    size = 10;
    grow = false;
    
    points.resize(5);
    points[0].x = 5; points[0].y = 5;
    points[1].x = 4; points[1].y = 5;
    points[2].x = 3; points[2].y = 5;
    points[3].x = 2; points[3].y = 5;
    points[4].x = 1; points[4].y = 5;
}

void Snake::setSize(int size) {
    this->size = size;
}

bool Snake::selfCrashed() {
    // if the head is at the same place of some point of the snake, it have
    // self crashed
    for(int i = 1; i < points.size(); i++) {
        if(points[0].x == points[i].x && points[0].y == points[i].y) {
            return true;
        }
    }

    return false;
}

int Snake::length() {
    return points.size();
}

bool Snake::collides(SDL_Point* point) {
    // checks if some point is at the same place of some point of the snake
    for(int i = 0; i < points.size(); i++) {
        if(points[i].x == point->x && points[i].y == point->y) {
            return true;
        }
    }

    return false;
}

float Snake::getSpeed() {
    return speed;
}

void Snake::setSpeed(float speed) {
    this->speed = speed;
}

void Snake::setDirection(Direction dir) {
    direction = dir;
    
    // Snake must be changed of direction only when moved
    moved = false;
}

Direction Snake::getDirection() {
    return direction;
}

SDL_Point Snake::getFirstPoint() {
    // gets the heado of the snake
    return points[0];
}

void Snake::setGrow(bool grow) {
    this->grow = grow;
}

void Snake::setMoved(bool moved) {
    this->moved = moved;
}

bool Snake::isMoved() {
    return moved;
}
