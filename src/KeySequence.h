/*
* File:   KeySequence.h
* Author: Jonathan Gutiérrez
* Ported from https://github.com/makzk/KeySequence-JS
*
* AnEpicSnake v0.5-dev
*
* This file is part of AnEpicSnake, licensed under the GPLv3 license.
* See the NOTICE.txt file for more information.
*/
#pragma once
#include "stdafx.h"

class KeySequence
{
public:
    KeySequence(std::vector<SDL_Keycode> sequence, Uint32 timeout, bool resetOnReady = false);
    ~KeySequence();

    void handleEvent(SDL_Event* e);
    std::vector<SDL_Keycode> getSequence();
    bool isReady();
    void reset();

private:
    std::vector<SDL_Keycode> sequence;
    Uint32 timeout;

    int lastkey;
    Uint32 lasttime;
    bool ready;

    bool resetOnReady;
};

