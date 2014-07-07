/* 
 * File:   SnakeGame.h
 * Author: Jonathan Gutiérrez
 *
 * Created on 28 de junio de 2014, 09:11 PM
 * 
 * AnEpicSnake v0.1.1
 * 
 * This file is part of AnEpicSnake, licenced under the GPLv3 licence.
 * See the NOTICE.txt file for more information.
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include <string>

#include "Snake.h"

#ifndef SNAKEGAME_H
#define	SNAKEGAME_H

/**
 * This controls the main game system for teh win (no, not Windows only).
 * @param width The game window width
 * @param height The game window height
 * 
 * @author Jonathan Gutiérrez
 * @version 1.0
 */
class SnakeGame {
public:
    /**
     * Initiates the game with window size values
     * @param width The game window width
     * @param height The game window height
     */
    SnakeGame(int width, int height);
    ~SnakeGame();
    
    /**
     * Initiate the SDL system
     * @return 0 if everything started OK, 1 if could not initiate SDL,
     * 2 if could not create window, and 3 if could not create SDL renderer.
     */
    int initDisplay();
    /**
     * Generates a new position for the food
     */
    void genFood();
    /**
     * Update the game conditions depending on the keys pressing
     * @param e
     */
    void handleKeys(SDL_Event* e);
    /**
     * Draws a beautiful and epilepsic background for you
     */
    void drawBackground();
    /**
     * Draws the food on the window at its position
     */
    void drawFood();
    /**
     * Starts the main game process
     * @return A return code, 0 is OK, anything else is bad.
     */
    int mainLoop();
    /**
     * Resets the game.
     */
    void reset();
    
    /**
     * Stop everything, free memory, and close the program.
     */
    void close();
    
    /**
     * Draws the pause icon
     */
    void drawPause();
    
private:
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    SDL_Texture* titleTexture;
    SDL_Rect titleProps;
    SDL_Texture* goTexture;
    SDL_Rect goProps;
    
    Snake snake;
    SDL_Point food;
    bool paused;
    bool alive;
    bool epilepsy;
    int winWidth;
    int winHeight;
    int squareSize;
    Uint8 pauseFade;
    
    bool started;
    bool quit;
};

#endif	/* SNAKEGAME_H */

