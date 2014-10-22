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

SnakeGame::SnakeGame(int width, int height) {
    winWidth = width;
    winHeight = height;
    started = false;
    alive = true;
    turbo = false;
    playMusic = false;
    seqok = false;
    squareSize = 10;

    food.setSize(squareSize);
    food.setColor(c_white);
    specialFood.setSize(squareSize);
    specialFood.setColor(c_red);

    Mgr.setSquareSize(squareSize);

    reset();
}

SnakeGame::~SnakeGame() {
    close();
}

void SnakeGame::reset() {
    epilepsy = true;
    
    points = 0;
    snakeSpeed = 10; // Initial speed
    specialCountdown = 1;// rand() % 20 + 20;
    snake.reset();
    snake.setSize(squareSize);
    snake.setSpeed(snakeSpeed);
    food.generate(winWidth, winHeight, &snake);
    specialFood.generate(winWidth, winWidth, &snake, &food);
    specialFood.setVisible(false);
    specialFood.setRandomEffect();
    specialEffect = Effect::EFFECT_NONE;
    specialTimeout = 0;
    paused = false;
}

bool SnakeGame::init()
{
    int mginit = Mgr.Init(winWidth, winHeight, "KairosDev - AnEpicSnake v0.5-dev");

    if(mginit != 0) {
        printf("Could not initiate the window system.\n");
        return false;
    }

    // loading "screen"
    Mgr.ClearRenderer(c_black);
    Mgr.DrawChar("loading...", 10, winHeight - 30, 5, c_white);
    Mgr.UpdateRenderer();
    
    // Initialize image system
    int imgFlags = IMG_INIT_PNG;
    if(!(IMG_Init( imgFlags ) & imgFlags)) {
        printf("Unable to init SDL2_image. Error: %s\n", IMG_GetError());
        return false;
    }

    // Initialize sound system
    if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 512 ) == -1 ) {
        printf("Could not start SDL_mixer system. Error: %s", Mix_GetError());
        return false;    
    }

    // Background load check
    if(!(
        titleTex.loadFromFile(Mgr.Renderer(), "res/titlebg.png") &&
        gameoverTex.loadFromFile(Mgr.Renderer(), "res/gameover.png") &&
        startBtn.loadImage(Mgr.Renderer(), "res/start.png") &&
        restartBtn.loadImage(Mgr.Renderer(), "res/restart.png") &&
        soundBtn.loadImage(Mgr.Renderer(), "res/soundicon.png")
    )) {
        printf("One or more image files could not be loaded.\n");
        return false;
    }

    // SFX Load
    eatSFX = Mix_LoadWAV("res/eat.ogg");
    deathSFX = Mix_LoadWAV("res/die.ogg");
    specialSFX1 = Mix_LoadWAV("res/special1.ogg");
    specialSFX2 = Mix_LoadWAV("res/special2.ogg");
    // Music load
    music = Mix_LoadMUS("res/music.ogg");

    if (eatSFX == NULL || deathSFX == NULL || specialSFX1 == NULL || music == NULL) {
        printf("Could not load sound file. Error: %s\n", Mix_GetError());
        return false;
    }

    Mgr.SetWindowIcon("res/icon.bmp"); // Add an icon to the window
    Mix_Volume(-1, 50); // Low the volume to the game
    
    // Title and gameover image position
    titleTex.setPos(winWidth/2 - titleTex.getRect().w/2, winHeight/2 - titleTex.getRect().h/2);
    gameoverTex.setPos(winWidth/2 - gameoverTex.getRect().w/2, winHeight/2 - gameoverTex.getRect().h/2);

    // Start button setup
    startBtn.setPos(winWidth/2 - startBtn.getRect().w/2, winHeight/2 + 80);
    startBtn.setupClipStates(3);
    startBtn.setClipStates(0, 1, 2, 1);

    // Restart button setup
    restartBtn.setPos(winWidth/2 - restartBtn.getRect().w/2, winHeight/2 + 112);
    restartBtn.setupClipStates(3);
    restartBtn.setClipStates(0, 1, 2, 1);

    // Sound button setup
    soundBtn.setPos(winWidth - soundBtn.getRect().w - 20, 20);
    soundBtn.setupClipStates(2);
    soundBtn.setClipStates(0, 0, 0, 0);

    return true;
}

int SnakeGame::mainLoop() {    
    if (!init()) {
        return 1;
    }
    
    SDL_Event e;
    bool quit = false;

    // Now featuring some special sequence
    std::vector<SDL_Keycode> a = { 
        SDLK_UP, SDLK_UP, SDLK_DOWN, SDLK_DOWN, 
        SDLK_LEFT, SDLK_RIGHT, SDLK_LEFT, SDLK_RIGHT,
        SDLK_b, SDLK_a
    };
    KeySequence seq(a, 1000);
    
    // creates a timer to check when to move the snake
    double speedTimeout = 1000/snake.getSpeed() + SDL_GetTicks();
    
    while(!quit) {
        // --- START UPDATES ---
        // --- START EVENTS ---
        while(SDL_PollEvent(&e) != 0) {
            if(e.type == SDL_QUIT) {
                quit = true;
            }
            else {
                handleEvents(&e);
                if (!seqok) {
                    seq.handleEvent(&e);
                }
            }
        }

        if (!seqok && seq.isReady()) {
            seqok = true;
        }

        if (quit) {
            break;
        }
        // --- END EVENTS ---
        
        if(started) {

            // Move when speed says you can move
            if (!paused && speedTimeout <= SDL_GetTicks()) {
                snake.move();

                double speedMult = 1;
                if (turbo) {
                    speedMult *= 2;
                }

                if (specialEffect != EFFECT_NONE && specialTimeout > SDL_GetTicks()) {
                    if (specialEffect == EFFECT_SPEED_UP) {
                        speedMult *= 2;
                    }
                    
                    if (specialEffect == EFFECT_SPEED_DOWN) {
                        speedMult /= 2;
                    }
                }

                snake.setSpeed(snakeSpeed * speedMult); // with turbo, speed is x 2
                speedTimeout = 1000 / snake.getSpeed() + SDL_GetTicks();
            }

            // Check if player have crashed to reset the snake
            if(alive && started && hasCrashed()) {
                started = false; // set that game has not started to stop moving
                alive = false;
                turbo = false;
                Mix_PlayChannel(-1, deathSFX, 0);
            } 

            // have you touched the food? it's like eat it
            if(snake.collidesWithHead(&food.getRect())) {
                food.generate(winWidth, winHeight, &snake);
                snake.setGrow(true);
                snakeSpeed += .3; // moar fun
                Mix_PlayChannel(-1, eatSFX, 0); // yay!
                points++;
                if (specialCountdown > 0) {
                    specialCountdown--;
                }
            }

            if (specialCountdown == 0) {
                if (!specialFood.isVisible()) {
                    specialFood.setVisible(true);
                    specialFood.setRandomEffect();
                }
                else if (snake.collidesWithHead(&specialFood.getRect())) {
                    specialEffect = specialFood.getEffect();
                    switch (specialEffect) {
                        case EFFECT_SPEED_UP:
                            specialTimeout = SDL_GetTicks() + 10000;
                            Mix_PlayChannel(-1, specialSFX1, 0);
                            break;
                        case EFFECT_SPEED_DOWN:
                            specialTimeout = SDL_GetTicks() + 5000;
                            Mix_PlayChannel(-1, specialSFX2, 0);
                            break;
                        case EFFECT_5POINTS:
                        default:
                            points += 5;
                            specialTimeout = SDL_GetTicks() + 4000;
                            Mix_PlayChannel(-1, specialSFX1, 0);

                    }
                    
                    specialCountdown = (rand() % 10) + 10;
                    specialFood.generate(winWidth, winHeight, &snake, &food);
                    specialFood.setVisible(false);
                }
            }

        }

        if (started && playMusic) {
            if (Mix_PlayingMusic() == 0) {
                Mix_PlayMusic(music, -1);
            }
        }
        else if (Mix_PlayingMusic() != 0) {
            Mix_HaltMusic();
        }

        // Click on start button
        if(startBtn.isDisplayed() && startBtn.getState() == Button::BTN_STATE_UP) {
            startBtn.setState(Button::BTN_STATE_NORMAL);
            started = true;
        }

        // Click on restart button
        if(restartBtn.isDisplayed() && restartBtn.getState() == Button::BTN_STATE_UP) {
            restartBtn.setState(Button::BTN_STATE_NORMAL);
            started = true;
            alive = true;
            reset(); // move the food
        }

        if (soundBtn.getState() == Button::BTN_STATE_UP) {
            toggleMusic();
        }

        // --- END UPDATES ---
        draw();
    }
    
    return 0;
}

void SnakeGame::draw() {
    // Clear screen
    Mgr.ClearRenderer(c_black);

    soundBtn.draw(Mgr.Renderer());
    snake.draw(Mgr.Renderer());
    
    if(epilepsy && (started || !alive)) {
        Mgr.RedrawRainbowBg(); // don't be epileptic
    }

    // you must have some food or you could die
    food.draw(Mgr.Renderer());
    specialFood.draw(Mgr.Renderer());
    
    // Draw points
    Mgr.DrawChar(std::to_string(points), squareSize, squareSize, squareSize / 2, c_alpha(turbo?c_cyan:c_white, 150));

    // Temporal message
    Mgr.DrawChar("dev version!", winWidth - 190, winHeight - 20, 3, c_alpha(c_white, 100));

    if (alive) {
        // If special is enabled
        Uint32 actualTicks = SDL_GetTicks();
        if (specialEffect != EFFECT_NONE && specialTimeout > actualTicks) {
            std::string effectName;
            switch (specialEffect)
            {
                case EFFECT_5POINTS: effectName = "+5 points"; break;
                case EFFECT_SPEED_UP: effectName = "speed up"; break;
                case EFFECT_SPEED_DOWN: effectName = "slow down"; break;
                default: effectName = "special effect active";
            }

            if (specialEffect == EFFECT_SPEED_UP || 
                specialEffect == EFFECT_SPEED_DOWN) {
                effectName += " - " + std::to_string((specialTimeout - SDL_GetTicks()) / 1000 + 1);
            }

            Mgr.DrawChar(effectName, squareSize, winHeight - 20, 3, c_alpha(c_white, 100));
        }
    }

    // Debug to know when this will appear
    if (started) {
        Mgr.SetRenderColor(c_alpha(c_white, 150));
        Mgr.DrawChar(
            std::to_string(specialCountdown),
            DRAWCHAR_RIGHT,
            20, 3, -(soundBtn.getRect().w + 30)
        );
    }

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

    if (seqok) {
        Mgr.DrawChar("Sequence ok", winWidth - 130, winHeight - 40, 2);
    }
    
    Mgr.UpdateRenderer();
}

void SnakeGame::handleEvents(SDL_Event* e) {
    // Button handling
    startBtn.handleEvent(e);
    restartBtn.handleEvent(e);
    soundBtn.handleEvent(e);

    // Keydown-only handling
    if(e->type == SDL_KEYDOWN) {
        // Moves the snake on the desired direction, but you can't go back.
        if(!paused && alive && snake.isMoved())
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

            snake.setMoved(false);
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
        if (e->key.keysym.sym == SDLK_e && started && !paused) {
            epilepsy = !epilepsy;
        }

        // Sound toggle
        if (e->key.keysym.sym == SDLK_q) {
            toggleMusic();
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

void SnakeGame::toggleMusic() {
    soundBtn.setState(Button::BTN_STATE_NORMAL);
    playMusic = !playMusic;

    // Update the music status to the button
    Uint8 st = playMusic ? 0 : 1;
    soundBtn.setClipStates(st, st, st, st);
}

// bye bye!
void SnakeGame::close() {
    titleTex.free();
    gameoverTex.free();
    startBtn.free();
    restartBtn.free();
    
    Mix_FreeChunk(eatSFX);
    Mix_FreeChunk(deathSFX);
    Mix_FreeChunk(specialSFX1);
    Mix_FreeChunk(specialSFX2);
    Mix_FreeMusic(music);
    
    Mix_CloseAudio();
    
    Mgr.close();
}