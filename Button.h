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

#include "Texture.h"

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
    /**
     * Sets a texture clip to a determined status
     * @param state The status of the button to set a clip
     * @param clip The clip to be set
     */
    void setTextureClip(State state, SDL_Rect clip);
    /**
     * Loads a texture from a file
     * @param renderer The renderer used to load the image
     * @param path The file image to load
     * @return A boolean that 
     */
    bool loadImage(SDL_Renderer* renderer, std::string path);
    
    void free();
    
    bool isDisplayed();
    void setDisplayed(bool displayed);
    
    void setState(State state);
    State getState();
    
    void setTexture(Texture texture);
    
    void setPos(int x, int y);
    void setSize(int w, int h);
    SDL_Rect getRect();
    
private:
    // Button style
    State state;
    
    bool displayed; //< This sets if the button is displayed
    
    // Button styles
    SDL_Rect stateTextureClips[BTN_STATE_TOTAL];
    Texture texture;
};

#endif	/* BUTTON_H */
