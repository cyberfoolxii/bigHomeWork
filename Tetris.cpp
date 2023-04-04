#include "TETRIS_Utils.h"
using namespace std;

//    vector<vector<tetrisObject>> miniMatrix(4);
//    for(int i = 0; i < 4; i++){
//        miniMatrix[i] = vector<tetrisObject>(4);
//        for(int j = 0; j < 4; j++){
//            miniMatrix[i][j].coordinate = {BLOCK_SIZE*(j+15), BLOCK_SIZE*(i+3)};
//        }
//    }
//    for(int i = 0; i < 4; i++){
//        int temp = brick.idx[i].y;
//        bool check = true;
//        for(int i = 0; i < 4; i++){
//            if(!canLeft(gameBoard, brick.idx[i].x, temp)){
//                check = false;
//            }
//        }
//        while(check){
//            temp--;
//        }
//        miniMatrix[brick.idx[i].x][temp].occupied = true;
//    }
int main(int argc, char* argv[]){
    SDL_Window* tetrisWindow = nullptr;
    SDL_Renderer* tetrisRenderer = nullptr;
    tetrisTexture tetrisSpriteSheet[TETRIS_TOTAL_IMAGE];
    bool optionList[TOTAL_OPTIONS] {false};
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
        bool quit = false;
        int arrow_X = ARROW_X1;
        int arrow_Y = ARROW_Y1;
        while(!quit){
            SDL_Event tetrisEvent;
            while(SDL_PollEvent(&tetrisEvent) != 0){
                if(tetrisEvent.type == SDL_QUIT){
                    quit = true;
                }
                eventHandler(tetrisEvent, gameBoard, brick, timer, VEL, quit, optionList, arrow_X, arrow_Y);
            }
            SDL_RenderClear(tetrisRenderer);
            renderBoard(gameBoard, tetrisSpriteSheet, tetrisRenderer, brick, optionList, arrow_X, arrow_Y);
            if(optionList[PLAY_OPTION]){
                renderDataAndSetBrick(gameBoard, brick, tetrisSpriteSheet, timer, tetrisFont, tetrisRenderer, VEL);
                brickFallDown(gameBoard, timer, brick, VEL);
            }
            SDL_RenderPresent(tetrisRenderer);
            SDL_Delay(20);
        }
    }
    closeSDL(tetrisWindow, tetrisRenderer, tetrisSpriteSheet, tetrisFont);
    return 0;
}
