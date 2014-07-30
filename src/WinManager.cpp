/*
* File:   WinManager.cpp
* Author: Jonathan Gutiérrez
* AnEpicSnake v0.5-dev
*
* This file is part of AnEpicSnake, licensed under the GPLv3 license.
* See the NOTICE.txt file for more information.
*/
#include "stdafx.h"
#include "Font.h"
#include "WinManager.h"

WinManager::WinManager()
{
    squareSize = 10;
    width = 0;
    height = 0;
    pauseFade = 255;
}

WinManager::~WinManager()
{
    close();
}

int WinManager::Init(int width, int height, std::string title = "KairosDev Window")
{
    this->width = width;
    this->height = height;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        return 1;
    }

    window = SDL_CreateWindow(title.c_str(),
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Unable to create window. Error: %s", SDL_GetError());
        return 2;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        printf("Unable to create renderer. Error: %s", SDL_GetError());
        return 3;
    }

    // Used to draw with transparency
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    // Used to draw the background
    bgpx = new Uint32[width * height];
    memset(bgpx, 255, width * height * sizeof(Uint32));
    bgtx = SDL_CreateTexture(renderer,
        SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, width, height);
    SDL_SetTextureBlendMode(bgtx, SDL_BLENDMODE_BLEND);

    return 0;
}

void WinManager::DrawSquare(int x, int y, int size)
{
    SDL_Rect rect = { x, y, size, size };
    SDL_RenderFillRect(renderer, &rect);
}

void WinManager::DrawSquare(int x, int y, SDL_Color color)
{
    SDL_Rect rect = {x * squareSize, y * squareSize, squareSize, squareSize};
    SetRenderColor(color);
    SDL_RenderFillRect(renderer, &rect);
}

void WinManager::DrawSquare(int x, int y, SDL_Color color, int size = 10)
{
    SetRenderColor(color);
    DrawSquare(x, y, size);
}

void WinManager::UpdateRenderer()
{
    SDL_RenderPresent(renderer);
}

void WinManager::SetRenderColor(SDL_Color color)
{
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
}

void WinManager::ClearRenderer()
{
    SDL_RenderClear(renderer);
}

void WinManager::ClearRenderer(SDL_Color color)
{
    SetRenderColor(color);
    ClearRenderer();
}

SDL_Window* WinManager::Window()
{
    return window;
}

SDL_Renderer* WinManager::Renderer()
{
    return renderer;
}

bool WinManager::SetWindowIcon(std::string icon_path)
{
    SDL_Surface* icon = SDL_LoadBMP(icon_path.c_str());
    if (icon == NULL) {
        return false;
    }
    else {
        SDL_SetWindowIcon(window, icon);
        SDL_FreeSurface(icon);
    }

    return true;
}

void WinManager::RedrawRainbowBg()
{
    if (renderer == NULL) {
        return;
    }

    // how much pretty squares can fit the window? c:
    for (int i = 0; i < height / squareSize; i++) {
        for (int j = 0; j < width / squareSize; j++) {
            // Format: 0xAARRGGBB
            int color = 0xB0000000 + (rand() % 255 << 16) + (rand() % 255 << 8) + rand() % 255;

            // this loops draws a square of square size
            for (int k = 0; k < squareSize; k++) {
                for (int l = 0; l < squareSize; l++) {
                    bgpx[(k + (10 * i))*width + (10 * j) + l] = color;
                }
            }
        }
    }

    SDL_UpdateTexture(bgtx, NULL, bgpx, width * sizeof(Uint32));
    SDL_RenderCopy(renderer, bgtx, NULL, NULL);
}
void WinManager::DrawChar(int n, int x, int y, int size = 10) {
    // If the int n is a number out of 0 and 9, it must be mapped to the characters font
    if (n < 0 || n > 9) {
        // Map the int n to the right character on the font
        if (n >= 'A' && n <= 'Z') {
            n -= 55;
        }
        else if (n >= 'a' && n <= 'z') {
            n -= 87;
        }
        else if (n >= '0' && n <= '9') {
            n -= 48;
        }
        else { // 
            switch (n) {
                case ' ': n = 36; break;
                case '-': n = 37; break;
                case '(': n = 38; break;
                case ')': n = 39; break;
                case '\'': n = 40; break;
                case ',': n = 41; break;
                case '.': n = 42; break;
                case '!': n = 43; break;
                default: n = (sizeof(chars) / sizeof(uint32_t)) - 1;
            }
        }
    }

    for (int i = 4, k = 0; i >= 0; i--, k++) {
        uint8_t byte = ((chars[n] & 0xf << 4 * i) >> 4 * i);
        for (int j = 4; j < 8; j++) {
            if ((byte & (0x80 >> j)) != 0) {
                DrawSquare((size * (j - 4)) + x, (size * k) + y, size);
            }
        }
    }
}

void WinManager::DrawChar(std::string str, int x, int y, int size = 10) {
    for (Uint16 i = 0, j = 0; i < str.size(); i++, j++) {
        // push a new line, move the characters back to the x start coordinate
        // and move them all down a line
        if (str.at(i) == '\n') {
            j = -1; // j will back to 0 on the loop
            y += size * 5 + size;
        }
        else {
            // x will be: the starting position, 
            // plus the separation from the character position number, 
            // plus a separation space, based on the square size
            // the 5 means the 4 squares from the font plus one square as a separation
            DrawChar(str.at(i), (x + (size * 5 * j)), y, size);
        }
    }
}

void WinManager::DrawChar(std::string str, int x, int y, int size, SDL_Color color)
{
    SetRenderColor(color);
    DrawChar(str, x, y, size);
}

void WinManager::DrawPause(bool fade_animation)
{
    // Pause icon properties
    Uint16 lwidth = 20; // bars width
    Uint16 lheight = 60; // bars height
    Uint8 sep = 20; // separation between bars

    SDL_Rect r1 = { // rect for bar 1
        width / 2 - lwidth - sep / 2,
        height / 2 - lheight,
        lwidth,
        lheight
    };
    SDL_Rect r2 = { // rect for bar 2
        r1.x + lwidth + sep,
        r1.y,
        lwidth,
        lheight
    };

    SetRenderColor(c_alpha(c_white, pauseFade));

    // Draw the bars

    SDL_RenderFillRect(renderer, &r1);
    SDL_RenderFillRect(renderer, &r2);

    // Display pause with a fade
    if (fade_animation) {
        pauseFade -= 5;
        if (pauseFade < 50) {
            pauseFade = 255;
        }
    }
}

void WinManager::close()
{
    delete[] bgpx;
    SDL_DestroyTexture(bgtx);

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}