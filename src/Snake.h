/* 
 * File:   Snake.h
 * Author: Jonathan Gutiérrez
 *
 * Created on 28 de junio de 2014, 08:59 PM
 * 
 * AnEpicSnake v0.5-dev
 * 
 * This file is part of AnEpicSnake, licenced under the GPLv3 licence.
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
    
    /**
     * Reset the position, speed and length of the snake.
     */
    void reset();
    
    /**
     * Draw the snake in a rendered
     * @param renderer The SDL renderer to draw the snake
     */
    void draw(SDL_Renderer* renderer);
    
    /**
     * Performs the default movement of the snake, depending on the direction
     */
    void move();
    
    /**
     * Checks if the snake self crashed
     * @return A bool that represents the self-crash status
     */
    bool selfCrashed();
    
    /**
     * Set the actual direction of the snake
     * @param dir A Direction parameter (UP, DOWN, LEFT, RIGHT)
     */
    void setDirection(Direction dir);
    
    /**
     * Returns the actual direction of the snake
     * @return The actual direction of the snake
     */
    Direction getDirection();
    
    /**
     * Sets the size of each square of the snake
     * @param size The size for each square of the snake
     */
    void setSize(int size);
    /**
     * Returns the length (how much squares) is the snake
     * @return The length of the snake
     */
    int length();
    /**
     * Checks of a point is on the same point than other of the snake
     * @param point The point to verify if collides with the snake
     * @return A boolean that checks the collition state
     */
    bool collides(SDL_Rect* rect);
    /**
     * Returns the actual speed of the snake
     * @return The actual speed of the snake
     */
    float getSpeed();
    /**
     * Sets the speed of the snake
     * @param speed The speed to set to the snake
     */
    void setSpeed(float speed);
    /**
     * Sets if the snake will grow on move (will not loose the last square on
     * move)
     * @param grow The boolean that defines if snakw will grow or on move.
     */
    void setGrow(bool grow);
    /**
     * Returns the first point (the head) of the snake.
     * @return The first SDL_Point of the snake.
     */
    SDL_Point getFirstPoint();
    
    /**
     * Moves the tongue to the right position.
     */
    void moveTongue();
    
    bool isMoved();
    void setMoved(bool moved);
private:
    Direction direction;
    float speed;
    int size;
    bool grow;
    bool moved;
    std::vector<SDL_Point> points;
    
    SDL_Rect tongue;
    
    SDL_Rect a, b; //< Used for eyes
    SDL_Rect p; //< Used when drawing the snake points
};

#endif	/* SNAKE_H */

