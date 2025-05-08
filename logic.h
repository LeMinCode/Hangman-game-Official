#ifndef LOGIC_H_INCLUDED
#define LOGIC_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <cctype>
#include "graphics.h"
#include "virtual_keyboard.h"
using namespace std;

const vector<string> WORDS = {
    "angle", "ant", "apple", "arch", "arm", "army",
    "baby", "bag", "ball", "band", "basin", "basket", "bath", "bed", "bee", "bell", "berry","bird", "blade", "board", "boat", "bone", "book", "boot", "bottle", "box", "boy",
    "brain", "brake", "branch", "brick", "bridge", "brush", "bucket", "bulb", "button",
    "cake", "camera", "card", "cart", "carriage", "cat", "chain", "cheese", "chest",
    "chin", "church", "circle", "clock", "cloud", "coat", "collar", "comb", "cord",
    "cow", "cup", "curtain", "cushion",
    "dog", "door", "drain", "drawer", "dress", "drop", "ear", "egg", "engine", "eye",
    "face", "farm", "feather", "finger", "fish", "flag", "floor", "fly",
    "foot", "fork", "fowl", "frame", "garden", "girl", "glove", "goat", "gun",
    "hair", "hammer", "hand", "hat", "head", "heart", "hook", "horn", "horse",
    "hospital", "house", "island", "jewel", "kettle", "key", "knee", "knife", "knot",
    "leaf", "leg", "library", "line", "lip", "lock",
    "map", "match", "monkey", "moon", "mouth", "muscle",
    "nail", "neck", "needle", "nerve", "net", "nose", "nut",
    "office", "orange", "oven", "parcel", "pen", "pencil", "picture", "pig", "pin",
    "pipe", "plane", "plate", "plow", "pocket", "pot", "potato", "prison", "pump",
    "rail", "rat", "receipt", "ring", "rod", "roof", "root",
    "sail", "school", "scissors", "screw", "seed", "sheep", "shelf", "ship", "shirt",
    "shoe", "skin", "skirt", "snake", "sock", "spade", "sponge", "spoon", "spring",
    "square", "stamp", "star", "station", "stem", "stick", "stocking", "stomach",
    "store", "street", "sun", "table", "tail", "thread", "throat", "thumb", "ticket",
    "toe", "tongue", "tooth", "town", "train", "tray", "tree", "trousers", "umbrella",
    "wall", "watch", "wheel", "whip", "whistle", "window", "wire", "wing", "worm",
};

struct GameState {
    string word;
    vector<bool> guessed;
    string GuessedLetters;
    int WrongGuessCount = 0;
    const int MaxTries = 6;
};

string GetRandomWord() {
    int index = rand() % WORDS.size();
    return WORDS[index];
}

void DrawHangman(int wrong, Graphics& graphics) {
    static vector<SDL_Texture*> HangmanTextures(7, nullptr);
    if (!HangmanTextures[wrong]) {
        string path = "assets/hangman" + to_string(wrong) + ".png";
        HangmanTextures[wrong] = graphics.loadTexture(path.c_str());
        if (!HangmanTextures[wrong]) {
            cout << "Failed to load hangman image: " << path << endl;
            return;
        }
    }
    graphics.renderTexture(HangmanTextures[wrong], 200, 100);
}

void renderWordProgress(Graphics& graphics, const string& word, const vector<bool>& guessed) {
    string display = "";
    for (size_t i = 0; i < word.length(); ++i) {
        display += (guessed[i] ? word[i] : '_');
        display += ' ';
    }

    TTF_Font* font = graphics.loadFont("assets/Purisa-BoldOblique.ttf", 36);
    SDL_Color color = {0, 0, 0};
    SDL_Texture* text = graphics.renderText(display.c_str(), font, color);
    if (text) {
        graphics.renderTexture(text, 700, 100);
        SDL_DestroyTexture(text);
    }
    TTF_CloseFont(font);
}

bool CheckWin(const vector<bool>& guessed) {
    for (bool b : guessed) {
        if (!b) return false;
    }
    return true;
}

bool IsValidGuess(char guess, const string& GuessedLetters) {
    return isalpha(guess) && GuessedLetters.find(guess) == string::npos;
}

bool PlayGame(Graphics& graphics) {
    GameState game;
    game.word = GetRandomWord();
    game.guessed.resize(game.word.length(), false);

    VirtualKeyboard keyboard;
    keyboard.init();

    TTF_Font* font = graphics.loadFont("assets/Purisa-BoldOblique.ttf", 40);
    SDL_Color textColor = {0, 0, 0};

    while (game.WrongGuessCount < game.MaxTries) {
        graphics.prepareScene(nullptr);
        DrawHangman(game.WrongGuessCount, graphics);
        renderWordProgress(graphics, game.word, game.guessed);

        SDL_Texture* remainingText = graphics.renderText(
            ("Remaining attempts: " + to_string(game.MaxTries - game.WrongGuessCount)).c_str(), font, textColor);
        if (remainingText) {
            graphics.renderTexture(remainingText, 100, 600);
            SDL_DestroyTexture(remainingText);
        }

        keyboard.render(graphics);
        graphics.presentScene();

        SDL_Event e;
        int mouseX, mouseY;
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) exit(0);

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                SDL_GetMouseState(&mouseX, &mouseY);
                char GuessedChar = tolower(keyboard.HandleMouseClick(mouseX, mouseY));

                if (GuessedChar != '\0' && IsValidGuess(GuessedChar, game.GuessedLetters)) {
                    game.GuessedLetters += GuessedChar;
                    bool found = false;
                    for (size_t i = 0; i < game.word.length(); ++i) {
                        if (tolower(game.word[i]) == GuessedChar) {
                            game.guessed[i] = true;
                            found = true;
                        }
                    }
                    if (!found) game.WrongGuessCount++;
                }
            }
        }

        if (CheckWin(game.guessed)) {
            cout << "You won! The word was: " << game.word << endl;
            TTF_CloseFont(font);
            return true;
        }

        if (game.WrongGuessCount == game.MaxTries) {
            cout << "You lost! The word was: " << game.word << endl;
            TTF_CloseFont(font);
            return false;
        }

        SDL_Delay(50);
    }

    TTF_CloseFont(font);
    return false;
}

#endif
