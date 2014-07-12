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

enum ButtonState {
    BTN_STATE_HOVER,
    BTN_STATE_DOWN,
    BTN_STATE_UP,
    BTN_STATE_NORMAL
};

class Button {
public:
    Button();
    ~Button();
    
    void draw(SDL_Renderer* renderer);
    void handleEvent(SDL_Event* event);
    void setState(ButtonState state);
    void setColorUp(SDL_Color colorUp);
    SDL_Color getColorUp();
    void setColorDown(SDL_Color colorDown);
    SDL_Color getColorDown();
    void setColorHover(SDL_Color colorHover);
    SDL_Color getColorHover();
    void setColorNormal(SDL_Color colorNormal);
    SDL_Color getColorNormal();
    SDL_Rect getRect();
    void setRect(SDL_Rect rect);
    bool isDisplayed();
    void setDisplayed(bool displayed);
    
    ButtonState getState();
private:
    // Button style
    ButtonState state;
    
    bool displayed; //< This sets if the button is displayed
    
    // Button styles
    SDL_Rect rect;
    SDL_Color colorNormal;
    SDL_Color colorHover;
    SDL_Color colorDown;
    SDL_Color colorUp;
};

#endif	/* BUTTON_H */

