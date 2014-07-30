/* 
 * File:   Button.h
 * Author: Jonathan Gutiérrez
 *
 * Created on 11 de julio de 2014, 11:12 PM
 * 
 * AnEpicSnake v0.5-dev
 * This file is part of AnEpicSnake, licensed under the GPLv3 license.
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
    
    //************************************
    // Draws the button to the renderer, depending on the state. If a texture
    // is set, then the texture is drawn on the renderer. If not, it draws a
    // filled rectangle of certain colors.
    // Parameter SDL_Renderer* renderer The render destination
    //************************************
    void draw(SDL_Renderer* renderer);

    //************************************
    // Handles the mouse events to update the button status
    // Parameter: SDL_Event* event The SDL_Event object that captures the environment status
    //************************************
    void handleEvent(SDL_Event* event);

    //************************************
    // Sets a texture clip to a determined status
    // Parameter: State state The status of the button to set a clip
    // Parameter: SDL_Rect clip The clip to be set
    //************************************
    void setTextureClip(State state, SDL_Rect clip);

    //************************************
    // Loads a texture from a file
    // Parameter: SDL_Renderer* renderer The renderer used to load the image
    // Parameter: std::string path The file image to load
    // Returns: boolean A boolean that 
    //************************************
    bool loadImage(SDL_Renderer* renderer, std::string path);
    
    //************************************
    // Frees the memory from the button texture
    //************************************
    void free();
    
    //************************************
    // Initial setup for clip states, based on number of positions, that
    // divides the height of the button texture by this number of positions.
    // Parameter: int positions The number of positions available.
    //************************************
    void setupClipStates(int positions);
    
    //************************************
    // Sets a position for a state clip, by moving the clip up-down, based on
    // the division from the amount of positions available on the texture.
    // As the available positions are not stored from setup, the passed value
    // for position is not checked. The positions start from zero.
    // Parameter State state The state to be set
    // Parameter int position The position of the state clip, based on previous setup.
    //************************************
    void setClipState(State state, int position);

    //************************************
    // Sets a position for all the states clips, based on the division from the amount of
    // positions available on the texture. As the available positions are not stored from 
    // setup, the passed value for position is not checked. The positions start from zero.
    //************************************
    void setClipStates(int posNormal, int posHover, int posDown, int posUp);

    //************************************
    // Sets the position, passed to the texture of the button
    // Parameter: int x The X position
    // Parameter: int y The Y position
    //************************************
    void setPos(int x, int y);

    //************************************
    // Sets the size of the button's texture
    // Parameter: int w The width for the texture
    // Parameter: int h The height for the texture
    //************************************
    void setSize(int w, int h);

    SDL_Rect getRect();

    Button::State getState() const { return state; }
    void setState(Button::State val) { state = val; }
    bool isDisplayed() const { return displayed; }
    void setDisplayed(bool val) { displayed = val; }
    Texture getTexture() const { return texture; }
    void setTexture(Texture val) { texture = val; }

private:
    State state; // Button state
    bool displayed; // This sets if the button is displayed

    // Button styles
    SDL_Rect stateTextureClips[BTN_STATE_TOTAL];
    Texture texture;
};

#endif	/* BUTTON_H */