/* 
 * File:   SnakeGame.cpp
 * Author: Jonathan Gutiérrez
 * 
 * Created on 28 de junio de 2014, 09:11 PM
 * 
 * AnEpicSnake v0.1.1
 * 
 * This file is part of AnEpicSnake, licenced under the GPLv3 licence.
 * See the NOTICE.txt file for more information.
 */

#include "SnakeGame.h"
#include <stdio.h>

SnakeGame::SnakeGame(int width, int height) {
    winWidth = width;
    winHeight = height;
    started = false;
    
    reset();
}

SnakeGame::~SnakeGame() {
    close();
}

void SnakeGame::reset() {
    epilepsy = true;
    
    squareSize = 10;
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
        return false;
    }
    
    window = SDL_CreateWindow("AnEpicSnake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winWidth, winHeight, SDL_WINDOW_SHOWN);
    if(window == NULL) {
        return 3;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL) {
        return 4;
    }
    
    
    // Start image background
    std::string bgpath = "titlebg.png";
    SDL_Surface* loadedSurface = IMG_Load(bgpath.c_str());
    int titleWidth = 0, titleHeight = 0;
    
    if(loadedSurface == NULL) {
        printf("Unable to load title image. Error: %s", IMG_GetError());
    } else {
        titleTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if(titleTexture == NULL) {
            printf("Unable to create texture from '%s'. Error: %s", bgpath.c_str(), SDL_GetError());
            return 5;
        } else {
            titleWidth = loadedSurface->w;
            titleHeight = loadedSurface->h;
        }
        
        SDL_FreeSurface(loadedSurface);
    }
    
    SDL_Rect trect = {winWidth/2 - titleWidth/2, winHeight/2 - titleHeight/2, titleWidth, titleHeight};
    titleProps = trect;
    
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
            if(e.type == SDL_KEYDOWN) {
                handleKeys(&e);
            }
        }
        // --- END EVENTS ---
        
        if(started) {
            // Move when speed says you can move
            if(SDL_TICKS_PASSED(SDL_GetTicks(), timeout) && !paused) {
                snake.move();
                timeout = SDL_GetTicks() + ((1/snake.getSpeed())*1000);
            }

            // Check if player have crashed to reset the snake
            if(snake.getFirstPoint().x < 0 || snake.getFirstPoint().y < 0 || 
                    snake.getFirstPoint().x*squareSize >= winWidth || 
                    snake.getFirstPoint().y*squareSize >= winHeight ||
                    snake.selfCrashed()) {
                reset(); // move the food
            }

            // have you touched the food? it's like eat it
            if(snake.collides(&food)) {
                genFood();
                snake.setGrow(true);
                snake.setSpeed(snake.getSpeed()+1); // moar fun
            }
        }
        
        // --- END UPDATES ---
        // --- START DRAW ---
        // Clear screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        
        snake.draw(renderer);
        
        if(epilepsy) {
            drawBackground(); // don't be epileptic
        }
        
        drawFood(); // you must have some food or you could die
        
        if(paused) {
            drawPause();
        }
        
        if(!started) {
            SDL_RenderCopy(renderer, titleTexture, NULL, &titleProps);
        }
        SDL_RenderPresent(renderer);
        // --- END DRAW ---
    }
    
    return 0;
}

// is good for you
void SnakeGame::drawBackground() {
    if(renderer == NULL) {
        return;
    }
    
    // how much pretty squares can fit the window? c:
    // ps: this will take too much cpu (by now)
    for(int i = 0; i < winHeight/squareSize; i++) {
        for(int j = 0; j < winWidth/squareSize; j++) {
            SDL_SetRenderDrawColor(renderer, rand()%256, rand()%256, rand()%256, 180);
            SDL_Rect sq = {j*squareSize, i*squareSize, squareSize, squareSize};
            SDL_RenderFillRect(renderer, &sq);
        }
    }
}

void SnakeGame::drawFood() {
    if(renderer == NULL) {
        return;
    }
    
    // yes, this is a copypaste from the background
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_Rect sq = {food.x*squareSize, food.y*squareSize, squareSize, squareSize};
    SDL_RenderFillRect(renderer, &sq);
}

// bye bye!
void SnakeGame::close() {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    
    SDL_Quit();
}

void SnakeGame::genFood() {
    do {
        food.x = rand()%(winWidth/squareSize);
        food.y = rand()%(winHeight/squareSize);
    } while(snake.collides(&food));
}

void SnakeGame::handleKeys(SDL_Event* e) {
    if(e->type != SDL_KEYDOWN) {
        return;
    }
    
    SDL_Keycode key = e->key.keysym.sym;
    
    // Moves the snake on the desired direction, but you can't go back.
    if(!paused && snake.isMoved())
    {
        switch(key) {
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
    
    if(key == SDLK_RETURN) {
        if(!started) {
            started = true;
        } else {
            paused = !paused;
            // this resets the value of the pause icon fade
            pauseFade = paused ? 255 : 0;
        }
    }
    
    if(key == SDLK_e) {
        epilepsy = !epilepsy;
    }
}

void SnakeGame::drawPause() {
    // Pause icon properties
    Uint16 lwidth = 20;
    Uint16 lheight = 60;
    Uint8 sep = 20;
    SDL_Point pos = {winWidth/2 - lwidth - sep/2, winHeight/2 - lheight};
    
    SDL_Rect r1 = {pos.x, pos.y, lwidth, lheight};
    SDL_Rect r2 = {r1.x + lwidth + sep, r1.y, lwidth, lheight};
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, pauseFade);
    
    SDL_RenderFillRect(renderer, &r1);
    SDL_RenderFillRect(renderer, &r2);
    
    pauseFade -= 5;
    if(pauseFade < 50) {
        pauseFade = 255;
    }
}

