/* 
 * File:   Main.cpp
 * Author: Jonathan Gutiérrez
 *
 * Created on 28 de junio de 2014, 08:58 PM
 * 
 * AnEpicSnake v0.5-dev
 * 
 * This file is part of AnEpicSnake, licensed under the GPLv3 license.
 * See the NOTICE.txt file for more information.
 */

#include "stdafx.h"
#include "SnakeGame.h"

int main(int argc, char *argv[]) {
    bool wide = false;
    std::vector<std::string> args(argv, argv+argc);
    for (size_t i = 1; i < args.size(); ++i) {
        // widescreen mode
        if (args[i] == "--wide") {
            wide = true;
        }
    }
    
    // Simply instance of the snake game
    SnakeGame* game = new SnakeGame(wide ? 1280 : 800, 600);

    return game->mainLoop();
}