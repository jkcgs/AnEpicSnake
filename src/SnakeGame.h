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
     * @return bool This depends that if the game was correctly initiated.
     */
    bool init();
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
    
private:
    WinManager Mgr;
    
    Mix_Chunk* eatSFX;
    Mix_Chunk* deathSFX;
    Mix_Chunk* specialSFX;
    Mix_Music* music;
    
    Texture titleTex;
    Texture gameoverTex;
    
    Button startBtn;
    Button restartBtn;
    Button soundBtn;
    
    Snake snake;
    Food food;
    Food specialFood;
    Uint8 specialDelay;
    bool paused;
    bool alive;
    bool epilepsy;
    bool turbo;
    bool playMusic;
    int winWidth;
    int winHeight;
    int squareSize;
    int points;
    double snakeSpeed; // The real snake speed, used to set turbo and more effects
    Uint8 pauseFade;
    
    bool started;
};

#endif	/* SNAKEGAME_H */

