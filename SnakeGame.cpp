/* 
 * File:   SnakeGame.cpp
 * Author: Jonathan Gutiérrez
 * 
 * Created on 28 de junio de 2014, 09:11 PM
 */

#include <stdlib.h>
#include <string>

#include "SnakeGame.h"

SnakeGame::SnakeGame(int width, int height) {
    winWidth = width;
    winHeight = height;
    
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
    
    window = SDL_CreateWindow("AnEpicSnake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winWidth, winHeight, SDL_WINDOW_SHOWN);
    if(window == NULL) {
        return 2;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL) {
        return 3;
    }
    
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
                updateKeys(&e);
            }
        }
        // --- END EVENTS ---
        
        // Move when speed says you can move
        // TODO: Draw some pause thing
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

void SnakeGame::updateKeys(SDL_Event* e) {
    if(e->type != SDL_KEYDOWN) {
        return;
    }
    
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
    
    if(e->key.keysym.sym == SDLK_RETURN) {
        paused = !paused;
    }
}
