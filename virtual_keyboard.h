#ifndef VIRTUAL_KEYBOARD_H_INCLUDED
#define VIRTUAL_KEYBOARD_H_INCLUDED

#include <SDL.h>
#include <string>
#include <vector>
#include "graphics.h"
using namespace std;

struct VirtualKeyboard {
    vector<SDL_Rect> Keys;
    vector<char> KeyLetters;
    vector<bool> KeyPressed;
    SDL_Color KeyColor = {0, 0, 0};

void init() {
    int startX = 500, startY = 300;
    int KeyWidth = 50, KeyHeight = 50;
    int gapX = 10, gapY = 10;
    string letters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for (int i = 0; i < 26; ++i) {
        int row = i / 10;
        int col = i % 10;
        int x = startX + col * (KeyWidth + gapX);
        int y = startY + row * (KeyHeight + gapY);

        SDL_Rect KeyRect = { x, y, KeyWidth, KeyHeight };
        Keys.push_back(KeyRect);
        KeyLetters.push_back(letters[i]);
        KeyPressed.push_back(false);
    }
}

    void render(Graphics &graphics) {
        for (int i = 0; i < Keys.size(); ++i) {
            if (!KeyPressed[i]) {
                SDL_SetRenderDrawColor(graphics.renderer, 255, 255, 255, 255);
                SDL_RenderFillRect(graphics.renderer, &Keys[i]);

                string letter(1, KeyLetters[i]);
                SDL_Texture* letterTexture = graphics.renderText(letter.c_str(), graphics.loadFont("assets/Purisa-BoldOblique.ttf", 25), KeyColor);
                int textWidth, textHeight;
                SDL_QueryTexture(letterTexture, NULL, NULL, &textWidth, &textHeight);
                SDL_RenderCopy(graphics.renderer, letterTexture, NULL, &Keys[i]);
                SDL_DestroyTexture(letterTexture);
            }
        }
    }

    char handleMouseClick(int x, int y) {
        for (int i = 0; i < Keys.size(); ++i) {
            if (x >= Keys[i].x && x <= Keys[i].x + Keys[i].w && y >= Keys[i].y && y <= Keys[i].y + Keys[i].h) {
                if (!KeyPressed[i]) {
                    KeyPressed[i] = true;
                    return KeyLetters[i];
                }
            }
        }
        return '\0';
    }
};

#endif
