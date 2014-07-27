/*
* File:   WinManager.cpp
* Author: Jonathan Gutiérrez
* AnEpicSnake v0.5-dev
*
* This file is part of AnEpicSnake, licensed under the GPLv3 license.
* See the NOTICE.txt file for more information.
*/
#include "stdafx.h"
#include "WinManager.h"

WinManager::WinManager()
{
    squareSize = 10;
    width = 0;
    height = 0;
}


WinManager::~WinManager()
{
    close();
}

int WinManager::init(int width, int height, std::string title = "KairosDev Window")
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

void WinManager::DrawSquare(int x, int y, SDL_Color color)
{
    SDL_Rect rect = {x * squareSize, y * squareSize, squareSize, squareSize};
    SetRenderColor(color);
    SDL_RenderFillRect(renderer, &rect);
}

void WinManager::DrawSquare(int x, int y, SDL_Color color, int size = 10)
{
    SDL_Rect rect = { x, y, size, size };
    SetRenderColor(color);
    SDL_RenderFillRect(renderer, &rect);
}

void WinManager::DrawSquare(int x, int y, int size)
{
    SDL_Rect rect = { x, y, size, size };
    SDL_RenderFillRect(renderer, &rect);
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

void WinManager::close()
{
    delete[] bgpx;
    SDL_DestroyTexture(bgtx);

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}