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
}


WinManager::~WinManager()
{
    close();
}

int WinManager::init(int width, int height, std::string title = "KairosDev Window")
{
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

    // used to draw with transparency
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    return 0;
}

void WinManager::close()
{
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
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
