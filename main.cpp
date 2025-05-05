#include <cstdlib>
#include <ctime>
#include <SDL.h>
#include <SDL_image.h>
#include "defs.h"
#include "graphics.h"
#include "logic.h"
using namespace std;

int main(int argc, char* argv[]) {
    srand(time(0));

    Graphics graphics;
    graphics.init();

    graphics.ShowStartScreen();

    graphics.waitUntilKeyPressed();


    Mix_Music *gMusic = graphics.loadMusic("assets\\Hangman_BackGround_Music.mp3");
    graphics.play(gMusic);

do {
    bool playerWon = PlayGame(graphics);

    graphics.ShowEndScreen(playerWon ? "won" : "lost");

    SDL_Event e;
    bool quit = false;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            quit = true;
            break;
        }
        if (e.type == SDL_KEYDOWN) break;
    }
    if (quit) break;

} while (true);
    if (gMusic != nullptr) Mix_FreeMusic(gMusic);
    graphics.quit();
    return 0;
}
