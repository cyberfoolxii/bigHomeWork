#include "TETRIS_Utils.h"
using namespace std;
int main(int argc, char* argv[]){
    SDL_Window* tetrisWindow = nullptr;
    SDL_Renderer* tetrisRenderer = nullptr;
    tetrisTexture tetrisSpriteSheet[TETRIS_TOTAL_IMAGE];
    TTF_Font* tetrisFont = nullptr;
    tetrisTimer timer;
    int VEL = OBJECT_VEL;
    tetrisBrick brick;
    if(!initSDL(tetrisWindow, tetrisRenderer)){
        cout << "Failed to Init SDL | " << SDL_GetError() << endl;
    } else {
        if(!loadMedia(tetrisSpriteSheet, tetrisRenderer, tetrisFont)){
            cout << "Load Tetris Media Failed" << endl;
        }
        vector<vector<tetrisObject>> gameBoard = generateMatrix();
        timer.tetrisTimerStart();
        bool quit = false;
        while(!quit){
            SDL_Event tetrisEvent;
            while(SDL_PollEvent(&tetrisEvent) != 0){
                if(tetrisEvent.type == SDL_QUIT){
                    quit = true;
                }
                eventHandler(tetrisEvent, gameBoard, brick, timer, VEL, quit);
            }
            SDL_RenderClear(tetrisRenderer);
            renderBoard(gameBoard, tetrisSpriteSheet, tetrisRenderer, brick);
            renderDataAndSetBrick(gameBoard, brick, tetrisSpriteSheet, timer, tetrisFont, tetrisRenderer);
            brickFallDown(gameBoard, timer, brick, VEL);
            SDL_RenderPresent(tetrisRenderer);
            SDL_Delay(20);
        }
    }
    closeSDL(tetrisWindow, tetrisRenderer, tetrisSpriteSheet, tetrisFont);
    return 0;
}
