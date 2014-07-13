/* 
 * File:   SnakeGame.cpp
 * Author: Jonathan Gutiérrez
 * 
 * Created on 28 de junio de 2014, 09:11 PM
 * 
 * AnEpicSnake v0.3
 * 
 * This file is part of AnEpicSnake, licenced under the GPLv3 licence.
 * See the NOTICE.txt file for more information.
 */

#include "SnakeGame.h"
#include <stdio.h>
#include <math.h>
#include <cmath>

unsigned char numbers[] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, //0
    0x20, 0x60, 0x20, 0x20, 0x70, //1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, //2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, //3
    0x90, 0x90, 0xF0, 0x10, 0x10, //4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, //5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, //6
    0xF0, 0x10, 0x20, 0x40, 0x40, //7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, //8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, //9
};

SnakeGame::SnakeGame(int width, int height) {
    winWidth = width;
    winHeight = height;
    started = false;
    alive = true;
    turbo = false;
    
    reset();
}

SnakeGame::~SnakeGame() {
    close();
}

void SnakeGame::reset() {
    epilepsy = true;
    
    squareSize = 10;
    food.w = squareSize;
    food.h = squareSize;
    points = 0;
    snake.reset();
    snake.setSize(squareSize);
    snake.setSpeed(10);
    paused = false;
    quit = false;
    genFood();
}


int SnakeGame::initDisplay() {
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        return 1;
    }
    
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init( imgFlags ) & imgFlags)) {
        printf("Unable to init SDL2_image. Error: %s\n", IMG_GetError());
        return 2;
    }
    
    window = SDL_CreateWindow("KairosDev - AnEpicSnake v0.3", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winWidth, winHeight, SDL_WINDOW_SHOWN);
    if(window == NULL) {
        printf("Unable to create window. Error: %s", SDL_GetError());
        return 3;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL) {
        printf("Unable to create renderer. Error: %s", SDL_GetError());
        return 4;
    }
    
    // Start image background
    if(!titleTex.loadFromFile(renderer, "titlebg.png")) {
        return 5;
    }
    
    titleTex.setPos(winWidth/2 - titleTex.getRect().w/2, winHeight/2 - titleTex.getRect().h/2);
    
    // Game over background
    if(!gameoverTex.loadFromFile(renderer, "gameover.png")) {
        return 6;
    }
    
    gameoverTex.setPos(winWidth/2 - gameoverTex.getRect().w/2, winHeight/2 - gameoverTex.getRect().h/2);
    
    return 0;
}

int SnakeGame::mainLoop() {
    int init = initDisplay();
    
    if(init != 0) {
        return init;
    }
    
    SDL_Event e;
    quit = false;
    
    // used to draw with transparency
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    // creates a timer to check when to move the snake
    Uint32 timeout = SDL_GetTicks() + ((1/snake.getSpeed())*1000);
    
    while(!quit) {
        // --- START UPDATES ---
        // --- START EVENTS ---
        while(SDL_PollEvent(&e) != 0) {
            if(e.type == SDL_QUIT) {
                quit = true;
            }
            
            handleEvents(&e);
        }
        // --- END EVENTS ---
        
        if(started) {
            // Move when speed says you can move
            if(SDL_TICKS_PASSED(SDL_GetTicks(), timeout) && !paused) {
                snake.move();
                // with turbo, speed is x 2
                timeout = SDL_GetTicks() + ((1/(snake.getSpeed()*(turbo ? 2 : 1)))*1000);
            }

            // Check if player have crashed to reset the snake
            if(snake.getFirstPoint().x < 0 || snake.getFirstPoint().y < 0 || 
                    snake.getFirstPoint().x*squareSize >= winWidth || 
                    snake.getFirstPoint().y*squareSize >= winHeight ||
                    snake.selfCrashed()) {
                started = false; // set that game has not started to stop moving
                alive = false;
                // player will continue the game if he press ENTER
            }

            // have you touched the food? it's like eat it
            if(snake.collides(&food)) {
                points++;
                genFood();
                snake.setGrow(true);
                snake.setSpeed(snake.getSpeed()+.3); // moar fun
            }
        }
        
        // --- END UPDATES ---
        draw();
    }
    
    return 0;
}

// is good for you
void SnakeGame::drawBackground() {
    if(renderer == NULL) {
        return;
    }
    
    SDL_Rect sq = {0, 0, squareSize, squareSize};
    
    // how much pretty squares can fit the window? c:
    // ps: this is still take too much cpu (by now)
    for(int i = 0; i < winHeight/squareSize; i++) {
        for(int j = 0; j < winWidth/squareSize; j++) {
            SDL_SetRenderDrawColor(renderer, rand()%256, rand()%256, rand()%256, 180);
            sq.x = j*squareSize;
            sq.y = i*squareSize;
            SDL_RenderFillRect(renderer, &sq);
        }
    }
}

void SnakeGame::draw() {
    // Clear screen
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    snake.draw(renderer);

    if(epilepsy && (started || !alive)) {
        drawBackground(); // don't be epileptic
    }

    // you must have some food or you could die
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &food);
    
    // Draw points
    SDL_SetRenderDrawColor(renderer, turbo ? 100 : 255, 255, 255, 150);
    drawNumber(points, squareSize, squareSize, squareSize / 2, 0);

    if(paused) {
        drawPause();
    }

    if(!started && alive) {
        titleTex.draw(renderer);
    }

    // game over place
    if(!alive) {
        gameoverTex.draw(renderer);
    }
    
    SDL_RenderPresent(renderer);
}


void SnakeGame::drawPause() {
    // Pause icon properties
    Uint16 lwidth = 20; // bars width
    Uint16 lheight = 60; // bars height
    Uint8 sep = 20; // separation between bars
    // first point for pause icon
    SDL_Point pos = {winWidth/2 - lwidth - sep/2, winHeight/2 - lheight};
    
    SDL_Rect r1 = {pos.x, pos.y, lwidth, lheight}; // rect for bar 1
    SDL_Rect r2 = {r1.x + lwidth + sep, r1.y, lwidth, lheight}; // rect for bar 2
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, pauseFade);
    
    // Draw the bars
    SDL_RenderFillRect(renderer, &r1);
    SDL_RenderFillRect(renderer, &r2);
    
    // Display pause with a fade
    pauseFade -= 5;
    if(pauseFade < 50) {
        pauseFade = 255;
    }
}

void SnakeGame::drawNumber(int n, int x, int y, int pixelSize = 10, int separation = 10) {
    // log10(0) = -Infinity, so I give k an Snickers
    int k = n == 0 ? 0 : log10(n); // better?
    SDL_Rect pixel = {0, 0, pixelSize, pixelSize};
    
    do {
        int digit = n % 10;
        // Numbers are drawn rtl (if the number is 123, 3 is drawn first, then 2, and finnaly 1)
        int pos = (separation*pixelSize*k + pixelSize*5*k);
        
        for(int i = 0; i < 5; i++) {
            for(int j = 0; j < 8; j++) {
                if((numbers[digit*5 + i] & (0x80 >> j)) != 0) {
                    pixel.x = (pixelSize * j) + x + pos;
                    pixel.y = (pixelSize * i) + y;
                    SDL_RenderFillRect(renderer, &pixel);
                }
            }
        }
        
        n /= 10; // =/
        k--;
    } while (n > 0);
}

// bye bye!
void SnakeGame::close() {
    titleTex.free();
    gameoverTex.free();
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    
    SDL_Quit();
}

void SnakeGame::genFood() {
    do {
        food.x = (rand()%(winWidth/squareSize)) * squareSize;
        food.y = (rand()%(winHeight/squareSize)) * squareSize;
    } while(snake.collides(&food));
}

void SnakeGame::handleEvents(SDL_Event* e) {
    // Keydown-only handling
    if(e->type == SDL_KEYDOWN) {
        // Moves the snake on the desired direction, but you can't go back.
        if(!paused && snake.isMoved())
        {
            switch(e->key.keysym.sym) {
                case SDLK_w:
                    if(snake.getDirection() != DOWN) {
                        snake.setDirection(UP);
                    }
                    break;
                case SDLK_a:
                    if(snake.getDirection() != RIGHT) {
                        snake.setDirection(LEFT);
                    }
                    break;
                case SDLK_s:
                    if(snake.getDirection() != UP) {
                        snake.setDirection(DOWN);
                    }
                    break;
                case SDLK_d:
                    if(snake.getDirection() != LEFT) {
                        snake.setDirection(RIGHT);
                    }
                    break;
            }
        }

        // Check if ENTER key was pressed
        if(e->key.keysym.sym == SDLK_RETURN) {
            // Starts the game if it has not started
            // Like when opened the game, or on game over
            if(!started) {
                started = true;
                alive = true;
                reset(); // move the food
            } else {
                paused = !paused;
                // this resets the value of the pause icon fade
                pauseFade = paused ? 255 : 0;
            }
        }

        // Not-so-hidden background toggle
        if(e->key.keysym.sym == SDLK_e && started && !paused) {
            epilepsy = !epilepsy;
        }
    }
    
    // Both Keyup-down events handling
    // Only one by now but there could be more soon
    if(e->type == SDL_KEYDOWN || e->type == SDL_KEYUP) {
        // Turbo mode, enabled only when space is pressed down
        if(e->key.keysym.sym == SDLK_SPACE && started) {
            turbo = e->type == SDL_KEYDOWN;
        }
    }
}
