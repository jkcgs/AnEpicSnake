/*
* File:   WinManager.h
* Author: Jonathan Gutiérrez
* AnEpicSnake v0.5-dev
*
* This file is part of AnEpicSnake, licensed under the GPLv3 license.
* See the NOTICE.txt file for more information.
*/
#pragma once
#include "stdafx.h"
#include "Color.h"

class WinManager
{
public:
    WinManager();
    ~WinManager();

    //************************************
    // Creates the window and the renderer.
    // Parameter: int width The width of the window.
    // Parameter: int height The height of the window.
    // Parameter: std::string title 
    // Returns: If the initialization failed will return a number > 0, else, 0 is returned.
    //************************************
    int Init(int width, int height, std::string title);

    //************************************
    // Frees the memory used by the window and the renderer
    //************************************
    void close();

    //************************************
    // Draws a square to the window, following the square size matrix. 
    // Example: If square size is 10, and x is 2, then the square will
    // be drawn on x=20
    // Parameter: int x: The X position on the square matrix
    // Parameter: int y: The Y position on the square matrix
    // Parameter: SDL_Color color The color for the square
    //************************************
    void DrawSquare(int x, int y, SDL_Color color);

    //************************************
    // Draws a square on the desired position, square matrix independent.
    // Parameter: int x The x position on the renderer;
    // Parameter: int y The y position on the renderer;
    // Parameter: int size The size for the square
    //************************************
    void DrawSquare(int x, int y, int size);
    //************************************
    // Draws a square on the desired position, square matrix independent.
    // Parameter: int x The x position on the renderer;
    // Parameter: int y The y position on the renderer;
    // Parameter: SDL_Color color The color for the square
    // Parameter: int size The size for the square
    //************************************
    void DrawSquare(int x, int y, SDL_Color color, int size);

    //************************************
    // Updates the renderer (SDL_RendererPresent())
    //************************************
    void UpdateRenderer();

    //************************************
    // Updates the renderer color
    // Parameter: SDL_Color color The color to set
    //************************************
    void SetRenderColor(SDL_Color color);

    //************************************
    // Clears the renderer to the setted color. 
    // This DOES NOT updates the screen, so you must use UpdateRenderer()
    //************************************
    void ClearRenderer();

    //************************************
    // Clears the renderer to the desired color.
    // As this does not updates the screen, you must use UpdateRenderer()
    // Parameter: SDL_Color color The color to use to clear the renderer screen
    //************************************
    void ClearRenderer(SDL_Color color);

    //************************************
    // Updates the window icon from a BMP file
    // Parameter: std::string icon_path The BMP file path
    // Returns: bool true if the operation was successful. Else, false is returned.
    //************************************
    bool SetWindowIcon(std::string icon_path);

    //************************************
    // This method redraws the magical background.
    //************************************
    void RedrawRainbowBg();

    //************************************
    // Draws a pixeled-style character
    // Parameter: int n The char equivalent int or a one-digit number (0-9)
    // Parameter: int x The X position for the first digit
    // Parameter: int y The Y position for the first digit
    // Parameter: int size The size of each square of the numbers' font
    //************************************
    void DrawChar(int n, int x, int y, int size);

    //************************************
    // Draws a pixeled-style string with compatible characters
    // Parameter: std::string str The string to draw (that includes 0-9, a-z, A-Z, and the characters: ( ) - ! ? '
    // Parameter: int x The X position for the first character
    // Parameter: int y The Y position for the first character
    // Parameter: int size The size of each square of the characters' font
    //************************************
    void DrawChar(std::string str, int x, int y, int size);

    //************************************
    // Draws a pixeled-style string with compatible characters
    // Parameter: std::string str The string to draw (that includes 0-9, a-z, A-Z, and the characters: ( ) - ! ? '
    // Parameter: int x The X position for the first character
    // Parameter: int y The Y position for the first character
    // Parameter: int size The size of each square of the characters' font
    // Parameter: SDL_Color color The color of the text
    //************************************
    void DrawChar(std::string str, int x, int y, int size, SDL_Color color);

    //************************************
    // Draws a pause icon in the screen's center with an fade animation
    // Parameter: bool fade_animation Enables or disables the fade animation
    //************************************
    void DrawPause(bool fade_animation = true);

    int getSquareSize() const { return squareSize; }
    void setSquareSize(int val) { squareSize = val; }

    SDL_Window* Window();
    SDL_Renderer* Renderer();
private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    int width; // The window width
    int height; // The window height
    int squareSize; // The square size for squares matrix

    int pauseFade; // Used to animate the fade icon

    SDL_Texture* bgtx; // Background texture
    Uint32* bgpx; // Background pixels
};

