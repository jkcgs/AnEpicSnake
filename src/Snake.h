/* 
 * File:   Snake.h
 * Author: Jonathan Gutiérrez
 *
 * Created on 28 de junio de 2014, 08:59 PM
 * 
 * AnEpicSnake v0.5-dev
 * 
 * This file is part of AnEpicSnake, licensed under the GPLv3 license.
 * See the NOTICE.txt file for more information.
 */

#ifndef SNAKE_H
#define	SNAKE_H

#include "stdafx.h"

enum Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

/**
 * @author Jonathan Guti&eacute;rrez
 * @version 1.0
 * 
 * This is the object that represents the snake of the game.
 */
class Snake {
public:
    
    Snake();
    ~Snake();
    
    //************************************
    // Reset the position, speed and length of the snake.
    //************************************
    void reset();
    
    //************************************
    // Draw the snake in a rendered
    // Parameter: SDL_Renderer* renderer The SDL renderer to draw the snake
    //************************************
    void draw(SDL_Renderer* renderer);
    
    //************************************
    // Performs the default movement of the snake, depending on the direction
    //************************************
    void move();
    
    //************************************
    // Checks if the snake self crashed
    // Returns: bool A bool that represents the self-crash status
    //************************************
    bool selfCrashed();

    //************************************
    // Returns the length (how much squares) is the snake
    // Returns: int The length of the snake
    //************************************
    int length();

    //************************************
    // Checks of a point is on the same point than other of the snake
    // Returns: bool A boolean that checks the collition state
    // Parameter: SDL_Rect* rect The point to verify if collides with the snake
    //************************************
    bool collides(SDL_Rect* rect);

    //************************************
    // Returns the first point of the snake, aka the head
    // Returns: SDL_Point The first point of the snake
    //************************************
    SDL_Point getFirstPoint();
    
    //************************************
    // Moves the tongue to the right position.
    //************************************
    void moveTongue();
    
    Direction getDirection() const { return direction; }
    void setDirection(Direction val) { direction = val; }
    double getSpeed() const { return speed; }
    void setSpeed(double val) { speed = val; }
    int getSize() const { return size; }
    void setSize(int size);
    bool isGrow() const { return grow; }
    void setGrow(bool val) { grow = val; }
    bool isMoved() const { return moved; }
    void setMoved(bool val) { moved = val; }
private:
    Direction direction;
    double speed;
    int size;
    bool grow;
    bool moved;
    std::vector<SDL_Point> points;
    
    SDL_Rect tongue;
    
    SDL_Rect a, b; // Used for eyes
    SDL_Rect p; // Used when drawing the snake points
};

#endif	/* SNAKE_H */

