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
#include "KeySequence.h"

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
    //************************************
    // Initiates the game with window size values
    // Parameter: int width The game window width
    // Parameter: int height The game window height
    //************************************
    SnakeGame(int width, int height);
    ~SnakeGame();
    
    //************************************
    // Initiate the SDL system
    // Returns:   bool This depends that if the game was correctly initiated.
    //************************************
    bool init();

    //************************************
    // Update the game conditions depending on the keys pressing
    // Parameter: SDL_Event* e The event object to handle
    //************************************
    void handleEvents(SDL_Event* e);

    //************************************
    // Draws all the stuff
    //************************************
    void draw();

    //************************************
    // Starts the main game process
    // Returns:   int A return code, 0 is OK, anything else is bad.
    //************************************
    int mainLoop();

    //************************************
    // Determines if the player has crashed
    // Returns:   bool true or false, depending on if the snake has crashed
    //************************************
    bool hasCrashed();

    //************************************
    // Toggles the music on-off, and updates the sound button
    //************************************
    void toggleMusic();

    //************************************
    // Resets the game.
    //************************************
    void reset();
    
    //************************************
    // Stop everything, free used memory, and close the program.
    //************************************
    void close();
    
private:
    WinManager Mgr;
    
    Mix_Chunk* eatSFX;
    Mix_Chunk* deathSFX;
    Mix_Chunk* specialSFX1;
    Mix_Chunk* specialSFX2;
    Mix_Music* music;
    
    Texture titleTex;
    Texture gameoverTex;
    
    Button startBtn;
    Button restartBtn;
    Button soundBtn;
    
    Snake snake;
    Food food;
    Food specialFood;
    Uint8 specialCountdown;
    Effect specialEffect;
    Uint32 specialTimeout;
    bool paused;
    bool alive;
    bool epilepsy;
    bool turbo;
    bool playMusic;
    bool seqok;
    Uint16 winWidth;
    Uint16 winHeight;
    Uint8 squareSize;
    Uint16 points; // Player points. No one will reach 65535 points, but who knows.
    double snakeSpeed; // The real snake speed, used to set turbo and more effects.
    Uint8 pauseFade;
    
    bool started;
};

#endif	/* SNAKEGAME_H */

