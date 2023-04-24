#include "TETRIS_Utils.h"
using namespace std;

int main(int argc, char* argv[]){
    SDL_Window* tetrisWindow = nullptr;
    SDL_Renderer* tetrisRenderer = nullptr;
    tetrisTexture tetrisSpriteSheet[TETRIS_TOTAL_IMAGE];
    bool optionList[TOTAL_OPTIONS] {false};
    TTF_Font* tetrisFont = nullptr;
    Mix_Music* music = nullptr;
    tetrisTimer timer;
    tetrisBrick brick;
    if(!initSDL(tetrisWindow, tetrisRenderer)){
        cout << "Failed to Init SDL | " << SDL_GetError() << endl;
    } else {
        if(!loadMedia(tetrisSpriteSheet, tetrisRenderer, tetrisFont, music)){
            cout << "Load Tetris Media Failed" << endl;
        }
        vector<vector<tetrisObject>> gameBoard = generateMatrix();
        bool quit = false;
        int arrow_X = ARROW_X1;
        int arrow_Y = ARROW_Y1;
        while(!quit){
            SDL_Event tetrisEvent;
            while(SDL_PollEvent(&tetrisEvent) != 0){
                if(tetrisEvent.type == SDL_QUIT){
                    quit = true;
                }
                eventHandler(tetrisEvent, gameBoard, brick, timer, quit, optionList, arrow_X, arrow_Y);
            }
            SDL_RenderClear(tetrisRenderer);
            renderBoard(gameBoard, tetrisSpriteSheet, tetrisRenderer, brick, optionList, arrow_X, arrow_Y);
            if(optionList[PLAY_OPTION]){
                if(!Mix_PlayingMusic() && optionList[MUSIC_OPTION]){
                    Mix_PlayMusic(music, -1);
                }
                renderDataAndSetBrick(gameBoard, brick, tetrisSpriteSheet, timer, tetrisFont, tetrisRenderer);
            } else {
                if(Mix_PlayingMusic() == 1){
                    Mix_HaltMusic();
                }
            }
            brickFallDown(gameBoard, timer, brick, optionList);
            SDL_RenderPresent(tetrisRenderer);
        }
    }
    closeSDL(tetrisWindow, tetrisRenderer, tetrisSpriteSheet, tetrisFont, music);
    return 0;
}
