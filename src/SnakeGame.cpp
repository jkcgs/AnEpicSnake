/* 
 * File:   SnakeGame.cpp
 * Author: Jonathan Gutiérrez
 * 
 * Created on 28 de junio de 2014, 09:11 PM
 * 
 * AnEpicSnake v0.5-dev
 * 
 * This file is part of AnEpicSnake, licensed under the GPLv3 license.
 * See the NOTICE.txt file for more information.
 */

#include "stdafx.h"
#include "SnakeGame.h"

enum ErrorLevel {
    ERROR_SDL_INIT = 1,
    ERROR_IMG_INIT,
    ERROR_CREATE_WIN,
    ERROR_CREATE_REN,
    ERROR_MIXER_INIT,
    ERROR_IMGFILE_LOAD,
    ERROR_SFXFILE_LOAD
};

SnakeGame::SnakeGame(int width, int height) {
    winWidth = width;
    winHeight = height;
    started = false;
    alive = true;
    turbo = false;
    squareSize = 10;

    food.setSize(squareSize);
    food.setColor(c_white);

    Mgr.setSquareSize(squareSize);

    reset();
}

SnakeGame::~SnakeGame() {
    close();
}

void SnakeGame::reset() {
    epilepsy = true;
    
    points = 0;
    snake.reset();
    snake.setSize(squareSize);
    snake.setSpeed(10);
    food.generate(&snake, winWidth, winHeight);
    paused = false;
    quit = false;
}

int SnakeGame::init() {
    int mginit = Mgr.Init(winWidth, winHeight, "KairosDev - AnEpicSnake v0.5-dev");

    if(mginit != 0) {
        return ERROR_SDL_INIT;
    }
    
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init( imgFlags ) & imgFlags)) {
        printf("Unable to init SDL2_image. Error: %s\n", IMG_GetError());
        return ERROR_IMG_INIT;
    }

    // loading "screen"
    Mgr.ClearRenderer(c_black);
    Mgr.DrawChar("loading...", 10, winHeight - 30, 5, c_white);
    Mgr.UpdateRenderer();
    
    // Add an icon to the window
    Mgr.SetWindowIcon("res/icon.bmp");

    // Initialize SDL_mixer
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 512 ) == -1 ) {
        return ERROR_MIXER_INIT;    
    }
    Mix_Volume(-1, 50); // Low the volume to the game
    
    // Background load check
    if(!titleTex.loadFromFile(Mgr.Renderer(), "res/titlebg.png") ||
       !gameoverTex.loadFromFile(Mgr.Renderer(), "res/gameover.png") ||
       !startBtn.loadImage(Mgr.Renderer(), "res/start.png") ||
       !restartBtn.loadImage(Mgr.Renderer(), "res/restart.png")) 
    {
        return ERROR_IMGFILE_LOAD;
    }

    // Title image position
    titleTex.setPos(winWidth/2 - titleTex.getRect().w/2, winHeight/2 - titleTex.getRect().h/2);

    // Gameover image position
    gameoverTex.setPos(winWidth/2 - gameoverTex.getRect().w/2, winHeight/2 - gameoverTex.getRect().h/2);

    // Start button setup
    startBtn.setPos(winWidth/2 - startBtn.getRect().w/2, winHeight/2 + 80);
    startBtn.setupClipStates(3);
    startBtn.setClipStates(0, 1, 2, 1);

    // Restart button setup
    restartBtn.setPos(winWidth/2 - restartBtn.getRect().w/2, winHeight/2 + 112);
    restartBtn.setupClipStates(3);
    restartBtn.setClipStates(0, 1, 2, 1);

    // SFX Load
    eatSFX = Mix_LoadWAV("res/eat.ogg");
    deathSFX = Mix_LoadWAV("res/die.ogg");

    if (eatSFX == NULL || deathSFX == NULL) {
        printf("Could not load sfx file. Error: %s\n", Mix_GetError());
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
            else {
                handleEvents(&e);
            }
        }

        if (quit) {
            break;
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
            if(alive && started && hasCrashed()) {
                started = false; // set that game has not started to stop moving
                alive = false;
                turbo = false;
                Mix_PlayChannel(-1, deathSFX, 0);
                // player will continue the game if he press ENTER
            } 

            // have you touched the food? it's like eat it
            if(snake.collides(&food.getRect())) {
                points++;
                food.generate(&snake, winWidth, winHeight);
                snake.setGrow(true);
                snake.setSpeed(snake.getSpeed()+.3); // moar fun
                Mix_PlayChannel(-1, eatSFX, 0); // yay!
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

void SnakeGame::draw() {
    // Clear screen
    Mgr.ClearRenderer(c_black);

    snake.draw(Mgr.Renderer());
    
    if(epilepsy && (started || !alive)) {
        Mgr.RedrawRainbowBg(); // don't be epileptic
    }

    // you must have some food or you could die
    food.draw(Mgr.Renderer());
    
    // Draw points
    Mgr.DrawChar(std::to_string(points), squareSize, squareSize, squareSize / 2, c_alpha(turbo?c_cyan:c_white, 150));

    // Temporal message
    Mgr.DrawChar("dev version, hang on bitches!", squareSize, winHeight - 20, 3, c_alpha(c_white, 100));

    if(paused) {
        Mgr.DrawPause();
    }

    if(!started && alive) {
        if(!startBtn.isDisplayed()) {
            startBtn.setDisplayed(true);
        }
        titleTex.draw(Mgr.Renderer());
        startBtn.draw(Mgr.Renderer());
    } else if(startBtn.isDisplayed()) {
        startBtn.setDisplayed(false);
    }

    // gameover place
    if(!alive) {
        if(!restartBtn.isDisplayed()) {
            restartBtn.setDisplayed(true);
        }
        gameoverTex.draw(Mgr.Renderer());
        restartBtn.draw(Mgr.Renderer());
    } else if(restartBtn.isDisplayed()) {
        restartBtn.setDisplayed(false);
    }
    
    Mgr.UpdateRenderer();
}

void SnakeGame::handleEvents(SDL_Event* e) {
    // Button handling
    startBtn.handleEvent(e);
    restartBtn.handleEvent(e);

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

bool SnakeGame::hasCrashed() {
    return snake.getFirstPoint().x < 0 || snake.getFirstPoint().y < 0 ||
        snake.getFirstPoint().x*squareSize >= winWidth ||
        snake.getFirstPoint().y*squareSize >= winHeight ||
        snake.selfCrashed();
}

// bye bye!
void SnakeGame::close() {
    titleTex.free();
    gameoverTex.free();
    startBtn.free();
    restartBtn.free();
    
    Mix_FreeChunk(eatSFX);
    Mix_FreeChunk(deathSFX);
    
    Mix_CloseAudio();
    
    Mgr.close();
}