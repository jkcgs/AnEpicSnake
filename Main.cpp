/* 
 * File:   Main.cpp
 * Author: Jonathan Gutiérrez
 *
 * Created on 28 de junio de 2014, 08:58 PM
 */

#include "SnakeGame.h"

int main(int argc, char *argv[]) {
    // Simply instance of the snake game
    SnakeGame* game = new SnakeGame(800, 600);

    return game->mainLoop();
}

