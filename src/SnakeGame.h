/* 
 * File:   SnakeGame.h
 * Author: Jonathan Gutiérrez
 *
 * Created on 28 de junio de 2014, 09:11 PM
 * 
 * AnEpicSnake v0.5-dev
 *
 * This file is part of AnEpicSnake, licensed under the GPLv3 license.
 * See the NOTICE.txt file for more information.
 */

#ifndef SNAKEGAME_H
#define SNAKEGAME_H

#include "stdafx.h"

#include "Snake.h"
#include "Button.h"
#include "Food.h"
#include "WinManager.h"

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
    int init();
    /**
     * Update the game conditions depending on the keys pressing
     * @param e
     */
    void handleEvents(SDL_Event* e);
    /**
     * Draws all the stuff
     */
    void draw();
    /**
     * Starts the main game process
     * @return A return code, 0 is OK, anything else is bad.
     */
    int mainLoop();

    // Determines if the player has crashed
    bool hasCrashed();

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
    WinManager Mgr;
    
    Mix_Chunk* eatSFX[5];
    Mix_Chunk* deathSFX[5];
    
    Texture titleTex;
    Texture gameoverTex;
    
    Button startBtn;
    Button restartBtn;
    
    Snake snake;
    Food food;
    bool paused;
    bool alive;
    bool epilepsy;
    bool turbo;
    int winWidth;
    int winHeight;
    int squareSize;
    int points;
    Uint8 pauseFade;
    
    bool started;
    bool quit;
};

#endif	/* SNAKEGAME_H */

