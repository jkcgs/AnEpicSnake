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


/**
 * This allows to display a button on screen and get its status.
 * 
 * @author Jonathan Gutiérrez
 * @version 1.0
 */
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
    
    /**
     * Draws the button to the renderer, depending on the state. If a texture
     * is set, then the texture is drawn on the renderer. If not, it draws a
     * filled rectangle of certain colors.
     * 
     * @param renderer The render destination
     */
    void draw(SDL_Renderer* renderer);
    /**
     * Handles the mouse events to update the button status
     * @param event The SDL_Event object that captures the environment status
     */
    void handleEvent(SDL_Event* event);
    
    
    SDL_Rect getRect();
    void setRect(SDL_Rect rect);
    bool isDisplayed();
    void setDisplayed(bool displayed);
    
    void setState(State state);
    State getState();
    
    void setTexture(SDL_Texture* texture);
    /**
     * Sets a texture clip to a determined status
     * @param state The status of the button to set a clip
     * @param clip The clip to be set
     */
    void setTextureClip(State state, SDL_Rect clip);

    /**
     * Set a background color to a determined status
     * @param state The status of the button to set a color
     * @param color The color to be set.
     */
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
