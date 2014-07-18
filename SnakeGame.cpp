/* 
 * File:   SnakeGame.cpp
 * Author: Jonathan Gutiérrez
 * 
 * Created on 28 de junio de 2014, 09:11 PM
 * 
 * AnEpicSnake v0.5-dev
 * 
 * This file is part of AnEpicSnake, licenced under the GPLv3 licence.
 * See the NOTICE.txt file for more information.
 */

#include "SnakeGame.h"
#include <stdio.h>
#include <math.h>
#include <cmath>

enum ErrorLevel {
    ERROR_SDL_INIT = 1,
    ERROR_IMG_INIT,
    ERROR_CREATE_WIN,
    ERROR_CREATE_REN,
    ERROR_MIXER_INIT,
    ERROR_IMGFILE_LOAD,
    ERROR_SFXFILE_LOAD
};

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
    squareSize = 10;
    
    bgpx = new Uint32[winWidth * winHeight];
    memset(bgpx, 255, winWidth * winHeight * sizeof(Uint32));
    
    reset();
}

SnakeGame::~SnakeGame() {
    close();
}

void SnakeGame::reset() {
    epilepsy = true;
    
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

int SnakeGame::init() {
    if(SDL_Init(SDL_INIT_VIDEO) != 0) {
        return ERROR_SDL_INIT;
    }
    
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init( imgFlags ) & imgFlags)) {
        printf("Unable to init SDL2_image. Error: %s\n", IMG_GetError());
        return ERROR_IMG_INIT;
    }
    
    window = SDL_CreateWindow("KairosDev - AnEpicSnake v0.5-dev", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, winWidth, winHeight, SDL_WINDOW_SHOWN);
    if(window == NULL) {
        printf("Unable to create window. Error: %s", SDL_GetError());
        return ERROR_CREATE_WIN;
    }
    
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    if(renderer == NULL) {
        printf("Unable to create renderer. Error: %s", SDL_GetError());
        return ERROR_CREATE_REN;
    }
    // used to draw with transparency
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    // Used to draw the background
    bgtx = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, winWidth, winHeight);
    SDL_SetTextureBlendMode(bgtx, SDL_BLENDMODE_BLEND);
    
    
    // Initialize SDL_mixer
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 512 ) == -1 )
    {
        return ERROR_MIXER_INIT;    
    }
    
    // Start image background
    if(!titleTex.loadFromFile(renderer, "res/titlebg.png")) {
        return ERROR_IMGFILE_LOAD;
    }
    titleTex.setPos(winWidth/2 - titleTex.getRect().w/2, winHeight/2 - titleTex.getRect().h/2);
    
    // Game over background
    if(!gameoverTex.loadFromFile(renderer, "res/gameover.png")) {
        return ERROR_IMGFILE_LOAD;
    }
    gameoverTex.setPos(winWidth/2 - gameoverTex.getRect().w/2, winHeight/2 - gameoverTex.getRect().h/2);
    
    // Start button
    if(!startBtn.loadImage(renderer, "res/start.png")) {
        return ERROR_IMGFILE_LOAD;
    }
    startBtn.setPos(winWidth/2 - startBtn.getRect().w/2, winHeight/2 + 80);
    startBtn.setupClipStates(3);
    startBtn.setClipState(Button::BTN_STATE_NORMAL, 0);
    startBtn.setClipState(Button::BTN_STATE_HOVER, 1);
    startBtn.setClipState(Button::BTN_STATE_DOWN, 2);
    startBtn.setClipState(Button::BTN_STATE_UP, 1);
    
    // Restart button
    if(!restartBtn.loadImage(renderer, "res/restart.png")) {
        return ERROR_IMGFILE_LOAD;
    }
    restartBtn.setPos(winWidth/2 - restartBtn.getRect().w/2, winHeight/2 + 112);
    restartBtn.setupClipStates(3);
    restartBtn.setClipState(Button::BTN_STATE_NORMAL, 0);
    restartBtn.setClipState(Button::BTN_STATE_HOVER, 1);
    restartBtn.setClipState(Button::BTN_STATE_DOWN, 2);
    restartBtn.setClipState(Button::BTN_STATE_UP, 1);
    
    SDL_Surface* icon = SDL_LoadBMP("res/icon.bmp");
    if(icon == NULL) {
        return ERROR_IMGFILE_LOAD;
    } else {
        SDL_SetWindowIcon(window, icon);
    }
    
    eatSound = Mix_LoadWAV("res/eat1.ogg");
    if(eatSound == NULL) {
        printf("Could not load sfx file. Error: %s", Mix_GetError());
        return ERROR_SFXFILE_LOAD;
    }
    
    dieSound = Mix_LoadWAV("res/die1.ogg");
    if(dieSound == NULL) {
        printf("Could not load sfx file. Error: %s", Mix_GetError());
        return ERROR_SFXFILE_LOAD;
    }
    
    return 0;
}

int SnakeGame::mainLoop() {
    int initLevel = init();
    
    if(initLevel != 0) {
        return initLevel;
    }
    
    SDL_Event e;
    quit = false;
    
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
            startBtn.handleEvent(&e);
            restartBtn.handleEvent(&e);
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
            if(alive && started &&
                    snake.getFirstPoint().x < 0 || snake.getFirstPoint().y < 0 || 
                    snake.getFirstPoint().x*squareSize >= winWidth || 
                    snake.getFirstPoint().y*squareSize >= winHeight ||
                    snake.selfCrashed()) {
                started = false; // set that game has not started to stop moving
                alive = false;
                turbo = false;
                Mix_PlayChannel(-1, dieSound, 0);
                // player will continue the game if he press ENTER
            } 

            // have you touched the food? it's like eat it
            if(snake.collides(&food)) {
                points++;
                genFood();
                snake.setGrow(true);
                snake.setSpeed(snake.getSpeed()+.3); // moar fun
                Mix_PlayChannel(-1, eatSound, 0); // yay!
            }
        } else if(alive && startBtn.getState() == Button::BTN_STATE_UP) {
            startBtn.setState(Button::BTN_STATE_NORMAL);
            started = true;
        } else if(!alive && restartBtn.getState() == Button::BTN_STATE_UP) {
            restartBtn.setState(Button::BTN_STATE_NORMAL);
            started = true;
            alive = true;
            reset(); // move the food
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
    
    // how much pretty squares can fit the window? c:
    for(int i = 0; i < winHeight/squareSize; i++) {
        for(int j = 0; j < winWidth/squareSize; j++) {
            // Format: 0xAARRGGBB
            int color = 0xB0000000 + (rand()%255 << 16) + (rand()%255 << 8) + rand()%255;
            
            // this loops draws a square of square size
            for(int k = 0; k < squareSize; k++) {
                for(int l = 0; l < squareSize; l++) {
                    bgpx[(k+(10*i))*winWidth + (10*j) + l] = color;
                }
            }
        }
    }
    
    SDL_UpdateTexture(bgtx, NULL, bgpx, winWidth * sizeof(Uint32));
    SDL_RenderCopy(renderer, bgtx, NULL, NULL);
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
        if(!startBtn.isDisplayed()) {
            startBtn.setDisplayed(true);
        }
        titleTex.draw(renderer);
        startBtn.draw(renderer);
    } else if(startBtn.isDisplayed()) {
        startBtn.setDisplayed(false);
    }

    // game over place
    if(!alive) {
        if(!restartBtn.isDisplayed()) {
            restartBtn.setDisplayed(true);
        }
        gameoverTex.draw(renderer);
        restartBtn.draw(renderer);
    } else if(restartBtn.isDisplayed()) {
        restartBtn.setDisplayed(false);
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
        // Numbers are drawn rtl (if the number is 123, 3 is drawn first, then 2, and finnaly 1)        
        for(int i = 0; i < 5; i++) {
            for(int j = 0; j < 8; j++) {
                if((numbers[(n % 10)*5 + i] & (0x80 >> j)) != 0) {
                    pixel.x = (pixelSize * j) + x + (separation*pixelSize*k + pixelSize*5*k);
                    pixel.y = (pixelSize * i) + y;
                    SDL_RenderFillRect(renderer, &pixel);
                }
            }
        }
        
        n /= 10; // =/
        k--;
    } while (n > 0);
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
                case SDLK_UP:
                    if(snake.getDirection() != DOWN) {
                        snake.setDirection(UP);
                    }
                    break;
                case SDLK_a:
                case SDLK_LEFT:
                    if(snake.getDirection() != RIGHT) {
                        snake.setDirection(LEFT);
                    }
                    break;
                case SDLK_s:
                case SDLK_DOWN:
                    if(snake.getDirection() != UP) {
                        snake.setDirection(DOWN);
                    }
                    break;
                case SDLK_d:
                case SDLK_RIGHT:
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
                startBtn.setDisplayed(false);
                restartBtn.setDisplayed(false);
            } else {
                paused = !paused;
                // this resets the value of the pause icon fade
                pauseFade = 0;
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

// bye bye!
void SnakeGame::close() {
    titleTex.free();
    gameoverTex.free();
    startBtn.free();
    restartBtn.free();
    
    delete[] bgpx;
    SDL_DestroyTexture(bgtx);
    
    Mix_FreeChunk(eatSound);
    Mix_FreeChunk(dieSound);
    Mix_CloseAudio();
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    SDL_Quit();
}