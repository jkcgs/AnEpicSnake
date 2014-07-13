/* 
 * File:   Button.h
 * Author: Jonathan Gutiérrez
 *
 * Created on 11 de julio de 2014, 11:12 PM
 * 
 * AnEpicSnake v0.3
 * 
 * This file is part of AnEpicSnake, licenced under the GPLv3 licence.
 * See the NOTICE.txt file for more information.
 */

#ifndef BUTTON_H
#define BUTTON_H

class Button {
public:
    enum State {
        BTN_STATE_NORMAL = 0,
        BTN_STATE_HOVER,
        BTN_STATE_DOWN,
        BTN_STATE_UP,
        BTN_STATE_TOTAL
    };
    
    Button();
    ~Button();
    
    void draw(SDL_Renderer* renderer);
    void handleEvent(SDL_Event* event);
    
    SDL_Rect getRect();
    void setRect(SDL_Rect rect);
    bool isDisplayed();
    void setDisplayed(bool displayed);
    
    void setState(State state);
    State getState();

    // UNIMPLEMENTED
    void setTexture(SDL_Texture* texture);
    void setTextureClip(State state, SDL_Rect clip);
    void setColorState(State state, SDL_Color color);
    
private:
    // Button style
    State state;
    
    bool displayed; //< This sets if the button is displayed
    
    // Button styles
    SDL_Rect rect;
    SDL_Color colorStates[BTN_STATE_TOTAL];
    SDL_Rect stateTextureClips[BTN_STATE_TOTAL];
    SDL_Texture* stateTexture;
};

#endif	/* BUTTON_H */
