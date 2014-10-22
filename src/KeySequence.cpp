/*
* File:   KeySequence.cpp
* Author: Jonathan Gutiérrez
*
* AnEpicSnake v0.5-dev
*
* This file is part of AnEpicSnake, licensed under the GPLv3 license.
* See the NOTICE.txt file for more information.
*/
#include "KeySequence.h"

KeySequence::KeySequence(std::vector<SDL_Keycode> seq, Uint32 timeout, bool resetOnReady)
{

    // If passed sequence is empty, the sequence won't be processed.
    if (seq.size() == 0) {
        this->resetOnReady = false;
        ready = true;
        return;
    }
    else {
        this->resetOnReady = resetOnReady;
        ready = false;
        this->sequence = seq;
        this->timeout = timeout;
    }

    lastkey = -1;
}

void KeySequence::handleEvent(SDL_Event* e)
{
    // If sequence is done, but must not be restarted, it is not handled.
    if ((ready && !resetOnReady) || e->type != SDL_KEYDOWN) {
        return;
    }

    SDL_Keycode key = e->key.keysym.sym;
    int now = SDL_GetTicks();

    if ((timeout == 0 || (now - lasttime) < timeout) &&
        key == sequence[lastkey + 1]) {
        lastkey++;
        lasttime = now;

        if (lastkey == (sequence.size() - 1)) {
            ready = true;
            if (resetOnReady) {
                reset();
            }
        }
    }
    else {
        reset();

        // If the key pressed is the first from the sequence, 
        // reset and retry
        if (key == sequence.at(0)) {
            lasttime = now;
            handleEvent(e);
        }
    }
}

void KeySequence::reset()
{
    lastkey = -1;
    lasttime = 0;
}

bool KeySequence::isReady()
{
    return ready;
}
