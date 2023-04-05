#include "TETRIS_Utils.h"
using namespace std;

void tetrisBrick::pickShape(){
    shapeCheck = 0;
    vel = OBJECT_VEL;
    srand(time(0));
    randNum = rand()%7;
    switch(randNum){
        case 0: // _
            idx = {
                {0, 3},
                {0, 4},
                {0, 5},
                {0, 6}
                };
        break;
        break;
        case 1: // [|]
            idx = {
                {0, 4},
                {0, 5},
                {1, 4},
                {1, 5}
            };
        break;
        case 2: // w
            idx = {
                {0, 4},
                {1, 3},
                {1, 4},
                {1, 5}
                };
        break;
        case 3: // z
            idx = {
                {0, 3},
                {0, 4},
                {1, 4},
                {1, 5}
            };
        break;
        case 4: // s
            idx = {
                {0, 5},
                {0, 6},
                {1, 4},
                {1, 5}
            };
        break;
        case 5: // L
            idx = {
                {0, 4},
                {1, 4},
                {2, 4},
                {2, 5}
            };
        break;
        case 6: // J
            idx = {
                {0, 5},
                {1, 5},
                {2, 5},
                {2, 4}
            };
        break;
    };
}
void tetrisBrick::pickColor(){
    srand(time(0));
    randNum = rand()%TETRIS_BLOCK_TEXTURE_5 + TETRIS_BLOCK_TEXTURE;
    switch(randNum){
        case TETRIS_BLOCK_TEXTURE:
            color = TETRIS_BLOCK_TEXTURE;
        break;
        case TETRIS_BLOCK_TEXTURE_1:
            color = TETRIS_BLOCK_TEXTURE_1;
        case TETRIS_BLOCK_TEXTURE_2:
            color = TETRIS_BLOCK_TEXTURE_2;
        break;
        case TETRIS_BLOCK_TEXTURE_3:
            color = TETRIS_BLOCK_TEXTURE_3;
        break;
        case TETRIS_BLOCK_TEXTURE_4:
            color = TETRIS_BLOCK_TEXTURE_4;
        break;
        case TETRIS_BLOCK_TEXTURE_5:
            color = TETRIS_BLOCK_TEXTURE_5;
        break;
    }
}
tetrisBrick::tetrisBrick(){
    pickColor();
    pickShape();
}

bool isGameOver(vector<vector<tetrisObject>> &boardMatrix){
    bool check = false;
    for(int j = 0; j < BOARD_COLUMNS; j++){
        if(boardMatrix[0][j].occupied){
            check = true;
        }
    }
    return check;
}

bool isRotateable(vector<vector<tetrisObject>> &boardMatrix, vector<SDL_Point> &temp){
	bool check = true;
		for(int k = 0; k < 4; k++){
            if(temp[k].x >= 0 && temp[k].x <= BOARD_ROWS - 1 && temp[k].y >= 0 && temp[k].y <= BOARD_COLUMNS - 1){
                if( (boardMatrix[ temp[k].x ][ temp[k].y ].occupied) ){
                    check = false;
                }
            } else {
                check = false;
            }
		}
	return check;
}

void rotate(vector<vector<tetrisObject>> &boardMatrix, tetrisBrick &brick){
    vector<SDL_Point> temp(4);
    switch(brick.randNum){
        case 0:
            if(brick.shapeCheck == 0){
                temp[0].x = brick.idx[1].x - 1;
                temp[0].y = brick.idx[1].y;
                temp[2].x = brick.idx[1].x + 1;
                temp[2].y = brick.idx[1].y;
                temp[3].x = brick.idx[1].x + 2;
                temp[3].y = brick.idx[1].y;
				temp[1].x = brick.idx[1].x;
				temp[1].y = brick.idx[1].y;
				if(isRotateable(boardMatrix, temp)){
					for(int i = 0; i < 4; i++){
						brick.idx[i].x = temp[i].x;
						brick.idx[i].y = temp[i].y;
					}
					brick.shapeCheck++;
				}
            } else if(brick.shapeCheck == 1){
                temp[0].x = brick.idx[1].x;
                temp[0].y = brick.idx[1].y - 1;
                temp[2].x = brick.idx[1].x;
                temp[2].y = brick.idx[1].y + 1;
                temp[3].x = brick.idx[1].x;
                temp[3].y = brick.idx[1].y + 2;
				temp[1].x = brick.idx[1].x;
				temp[1].y = brick.idx[1].y;
				if(isRotateable(boardMatrix, temp)){
					for(int i = 0; i < 4; i++){
						brick.idx[i].x = temp[i].x;
						brick.idx[i].y = temp[i].y;
					}
					brick.shapeCheck = 0;
				}
            }
        break;
        case 2:
            if(brick.shapeCheck == 0){
                temp[0].x = brick.idx[2].x;
                temp[0].y = brick.idx[2].y + 1;
                temp[1].x = brick.idx[2].x - 1;
                temp[1].y = brick.idx[2].y;
                temp[3].x = brick.idx[2].x + 1;
                temp[3].y = brick.idx[2].y;
				temp[2].x = brick.idx[2].x;
				temp[2].y = brick.idx[2].y;
                if(isRotateable(boardMatrix, temp)){
					for(int i = 0; i < 4; i++){
						brick.idx[i].x = temp[i].x;
						brick.idx[i].y = temp[i].y;
					}
					brick.shapeCheck++;
				}
            } else if(brick.shapeCheck == 1){
                temp[0].x = brick.idx[2].x;
                temp[0].y = brick.idx[2].y - 1;
                temp[1].x = brick.idx[2].x;
                temp[1].y = brick.idx[2].y + 1;
                temp[3].x = brick.idx[2].x + 1;
                temp[3].y = brick.idx[2].y;
				temp[2].x = brick.idx[2].x;
				temp[2].y = brick.idx[2].y;
                if(isRotateable(boardMatrix, temp)){
					for(int i = 0; i < 4; i++){
						brick.idx[i].x = temp[i].x;
						brick.idx[i].y = temp[i].y;
					}
					brick.shapeCheck++;
				}
            } else if(brick.shapeCheck == 2){
                temp[0].x = brick.idx[2].x;
                temp[0].y = brick.idx[2].y - 1;
                temp[1].x = brick.idx[2].x - 1;
                temp[1].y = brick.idx[2].y;
                temp[3].x = brick.idx[2].x + 1;
                temp[3].y = brick.idx[2].y;
				temp[2].x = brick.idx[2].x;
				temp[2].y = brick.idx[2].y;
                if(isRotateable(boardMatrix, temp)){
					for(int i = 0; i < 4; i++){
						brick.idx[i].x = temp[i].x;
						brick.idx[i].y = temp[i].y;
					}
					brick.shapeCheck++;
				}
            } else if(brick.shapeCheck == 3){
                temp[0].x = brick.idx[2].x - 1;
                temp[0].y = brick.idx[2].y;
                temp[1].x = brick.idx[2].x;
                temp[1].y = brick.idx[2].y + 1;
                temp[3].x = brick.idx[2].x;
                temp[3].y = brick.idx[2].y - 1;
				temp[2].x = brick.idx[2].x;
				temp[2].y = brick.idx[2].y;
                if(isRotateable(boardMatrix, temp)){
					for(int i = 0; i < 4; i++){
						brick.idx[i].x = temp[i].x;
						brick.idx[i].y = temp[i].y;
					}
					brick.shapeCheck = 0;
				}
            }
        break;
        case 3:
            if(brick.shapeCheck == 0){
                temp[0].x = brick.idx[1].x;
                temp[0].y = brick.idx[1].y - 1;
                temp[2].x = brick.idx[1].x - 1;
                temp[2].y = brick.idx[1].y;
                temp[3].x = brick.idx[1].x + 1;
                temp[3].y = brick.idx[1].y - 1;
				temp[1].x = brick.idx[1].x;
				temp[1].y = brick.idx[1].y;
               	if(isRotateable(boardMatrix, temp)){
					for(int i = 0; i < 4; i++){
						brick.idx[i].x = temp[i].x;
						brick.idx[i].y = temp[i].y;
					}
					brick.shapeCheck++;
				}
            } else if(brick.shapeCheck == 1){
                temp[0].x = brick.idx[1].x;
                temp[0].y = brick.idx[1].y - 1;
                temp[2].x = brick.idx[1].x + 1;
                temp[2].y = brick.idx[1].y;
                temp[3].x = brick.idx[1].x + 1;
                temp[3].y = brick.idx[1].y + 1;
				temp[1].x = brick.idx[1].x;
				temp[1].y = brick.idx[1].y;
                if(isRotateable(boardMatrix, temp)){
					for(int i = 0; i < 4; i++){
						brick.idx[i].x = temp[i].x;
						brick.idx[i].y = temp[i].y;
					}
					brick.shapeCheck = 0;
				}
            }
        break;
        case 4:
            if(brick.shapeCheck == 0){
                temp[1].x = brick.idx[0].x;
                temp[1].y = brick.idx[0].y - 1;
                temp[2].x = brick.idx[0].x - 1;
                temp[2].y = brick.idx[0].y - 1;
                temp[3].x = brick.idx[0].x + 1;
                temp[3].y = brick.idx[0].y;
				temp[0].x = brick.idx[0].x;
				temp[0].y = brick.idx[0].y;
                if(isRotateable(boardMatrix, temp)){
					for(int i = 0; i < 4; i++){
						brick.idx[i].x = temp[i].x;
						brick.idx[i].y = temp[i].y;
					}
					brick.shapeCheck++;
				}
            } else if(brick.shapeCheck == 1){
                temp[1].x = brick.idx[0].x;
                temp[1].y = brick.idx[0].y + 1;
                temp[2].x = brick.idx[0].x + 1;
                temp[2].y = brick.idx[0].y;
                temp[3].x = brick.idx[0].x + 1;
                temp[3].y = brick.idx[0].y - 1;
				temp[0].x = brick.idx[0].x;
				temp[0].y = brick.idx[0].y;
                if(isRotateable(boardMatrix, temp)){
					for(int i = 0; i < 4; i++){
						brick.idx[i].x = temp[i].x;
						brick.idx[i].y = temp[i].y;
					}
					brick.shapeCheck = 0;
				}
            }
        break;
        case 5:
            if(brick.shapeCheck == 0){
                temp[0].x = brick.idx[1].x;
                temp[0].y = brick.idx[1].y + 1;
                temp[2].x = brick.idx[1].x;
                temp[2].y = brick.idx[1].y - 1;
                temp[3].x = brick.idx[1].x + 1;
                temp[3].y = brick.idx[1].y - 1;
				temp[1].x = brick.idx[1].x;
				temp[1].y = brick.idx[1].y;
                if(isRotateable(boardMatrix, temp)){
					for(int i = 0; i < 4; i++){
						brick.idx[i].x = temp[i].x;
						brick.idx[i].y = temp[i].y;
					}
					brick.shapeCheck++;
				}
            } else if(brick.shapeCheck == 1){
                temp[0].x = brick.idx[1].x - 1;
                temp[0].y = brick.idx[1].y;
                temp[2].x = brick.idx[1].x + 1;
                temp[2].y = brick.idx[1].y;
                temp[3].x = brick.idx[1].x - 1;
                temp[3].y = brick.idx[1].y - 1;
				temp[1].x = brick.idx[1].x;
				temp[1].y = brick.idx[1].y;
                if(isRotateable(boardMatrix, temp)){
					for(int i = 0; i < 4; i++){
						brick.idx[i].x = temp[i].x;
						brick.idx[i].y = temp[i].y;
					}
					brick.shapeCheck++;
				}
            } else if(brick.shapeCheck == 2){
                temp[0].x = brick.idx[1].x;
                temp[0].y = brick.idx[1].y + 1;
                temp[2].x = brick.idx[1].x;
                temp[2].y = brick.idx[1].y - 1;
                temp[3].x = brick.idx[1].x - 1;
                temp[3].y = brick.idx[1].y + 1;
				temp[1].x = brick.idx[1].x;
				temp[1].y = brick.idx[1].y;
                if(isRotateable(boardMatrix, temp)){
					for(int i = 0; i < 4; i++){
						brick.idx[i].x = temp[i].x;
						brick.idx[i].y = temp[i].y;
					}
					brick.shapeCheck++;
				}
            } else if(brick.shapeCheck == 3){
                temp[0].x = brick.idx[1].x - 1;
                temp[0].y = brick.idx[1].y;
                temp[2].x = brick.idx[1].x + 1;
                temp[2].y = brick.idx[1].y;
                temp[3].x = brick.idx[1].x + 1;
                temp[3].y = brick.idx[1].y + 1;
				temp[1].x = brick.idx[1].x;
				temp[1].y = brick.idx[1].y;
                if(isRotateable(boardMatrix, temp)){
					for(int i = 0; i < 4; i++){
						brick.idx[i].x = temp[i].x;
						brick.idx[i].y = temp[i].y;
					}
					brick.shapeCheck = 0;
				}
            }
        break;
        case 6:
            if(brick.shapeCheck == 0){
                temp[0].x = brick.idx[1].x;
                temp[0].y = brick.idx[1].y + 1;
                temp[2].x = brick.idx[1].x;
                temp[2].y = brick.idx[1].y - 1;
                temp[3].x = brick.idx[1].x - 1;
                temp[3].y = brick.idx[1].y - 1;
				temp[1].x = brick.idx[1].x;
				temp[1].y = brick.idx[1].y;
				if(isRotateable(boardMatrix, temp)){
					for(int i = 0; i < 4; i++){
						brick.idx[i].x = temp[i].x;
						brick.idx[i].y = temp[i].y;
					}
					brick.shapeCheck++;
				}
            } else if(brick.shapeCheck == 1){
                temp[0].x = brick.idx[1].x - 1;
                temp[0].y = brick.idx[1].y;
                temp[2].x = brick.idx[1].x + 1;
                temp[2].y = brick.idx[1].y;
                temp[3].x = brick.idx[1].x - 1;
                temp[3].y = brick.idx[1].y + 1;
				temp[1].x = brick.idx[1].x;
				temp[1].y = brick.idx[1].y;
                if(isRotateable(boardMatrix, temp)){
					for(int i = 0; i < 4; i++){
						brick.idx[i].x = temp[i].x;
						brick.idx[i].y = temp[i].y;
					}
					brick.shapeCheck++;
				}
            } else if(brick.shapeCheck == 2){
                temp[0].x = brick.idx[1].x;
                temp[0].y = brick.idx[1].y + 1;
                temp[2].x = brick.idx[1].x;
                temp[2].y = brick.idx[1].y - 1;
                temp[3].x = brick.idx[1].x + 1;
                temp[3].y = brick.idx[1].y + 1;
				temp[1].x = brick.idx[1].x;
				temp[1].y = brick.idx[1].y;
                if(isRotateable(boardMatrix, temp)){
					for(int i = 0; i < 4; i++){
						brick.idx[i].x = temp[i].x;
						brick.idx[i].y = temp[i].y;
					}
					brick.shapeCheck++;
				}
            } else if(brick.shapeCheck == 3){
                temp[0].x = brick.idx[1].x - 1;
                temp[0].y = brick.idx[1].y;
                temp[2].x = brick.idx[1].x + 1;
                temp[2].y = brick.idx[1].y;
                temp[3].x = brick.idx[1].x + 1;
                temp[3].y = brick.idx[1].y - 1;
				temp[1].x = brick.idx[1].x;
				temp[1].y = brick.idx[1].y;
                if(isRotateable(boardMatrix, temp)){
					for(int i = 0; i < 4; i++){
						brick.idx[i].x = temp[i].x;
						brick.idx[i].y = temp[i].y;
					}
					brick.shapeCheck = 0;
				}
            }
        break;
    }
}

void renderDataAndSetBrick(vector<vector<tetrisObject>> &boardMatrix, tetrisBrick &brick, tetrisTexture* tetrisSpriteSheet,
tetrisTimer &timer, TTF_Font*& font, SDL_Renderer*& tetrisRenderer){
    static int countedScore = 0;
    static int countedLines = 0;
    static int countedLevel = 0;
    stringstream sstream;
    if(isGameOver(boardMatrix)){
        countedScore = 0;
        countedLines = 0;
        countedLevel = 0;
        timer.tetrisTimerPause();
        tetrisSpriteSheet[TETRIS_GAMEOVER_TEXTURE].renderTexture(80, 0, tetrisRenderer, nullptr);
    } else {
        int currentLines = 0;
        if(!canBrickFall(boardMatrix, brick)){
            for(int i = 0; i < 4; i++){
                boardMatrix[ brick.idx[i].x ][ brick.idx[i].y ].occupied = true;
                boardMatrix[ brick.idx[i].x ][ brick.idx[i].y ].color = brick.color;
            }
            for(int i = 0; i < BOARD_ROWS; i++){
                bool check = true;
                for(int j = 0; j < BOARD_COLUMNS; j++){
                    if(!boardMatrix[i][j].occupied) check = false;
                }
                if(check){
                    currentLines++;
                    for(int k = i; k > 0; k--){
                        for(int z = 0; z < BOARD_COLUMNS; z++){
                            boardMatrix[k][z].occupied = boardMatrix[k-1][z].occupied;
                            boardMatrix[k][z].color = boardMatrix[k-1][z].color;
                        }
                    }
                }
            }
            countedLines += currentLines;
            countedScore += 10*(currentLines*currentLines);
            brick.pickColor();
            brick.pickShape();
        }
        if(countedScore >= ((countedLevel + 1)*50) ){
            if(brick.vel - 100 >= 1){
                brick.vel -= 100;
            }
            countedLevel++;
        }
        SDL_Color textColor = {0, 0, 0, 255};
        sstream.str("");
        sstream << countedLevel;
        tetrisSpriteSheet[TETRIS_LEVEL_COUNT].loadFromText(font, sstream.str().c_str(), tetrisRenderer, textColor);

        sstream.str("");
        sstream << countedScore;
        tetrisSpriteSheet[TETRIS_SCORE_COUNT].loadFromText(font, sstream.str().c_str(), tetrisRenderer, textColor);

        sstream.str("");
        sstream << countedLines;
        tetrisSpriteSheet[TETRIS_LINES_COUNT].loadFromText(font, sstream.str().c_str(), tetrisRenderer, textColor);

        sstream.str("");
        if(timer.tetrisTimerGetTicks() >= 60000){
            sstream << timer.tetrisTimerGetTicks()/60000 << ':' << timer.tetrisTimerGetTicks()/1000 - 60*(timer.tetrisTimerGetTicks()/60000);
        } else {
            sstream << timer.tetrisTimerGetTicks()/60000 << ':' << timer.tetrisTimerGetTicks()/1000;
        }
        tetrisSpriteSheet[TETRIS_TIME_COUNT].loadFromText(font, sstream.str().c_str(), tetrisRenderer, textColor);
    }
}
bool canBrickLeft(vector<vector<tetrisObject>> &boardMatrix, tetrisBrick &brick){
    bool check = true;
    for(int i = 0; i < 4; i++){
        if(!canLeft(boardMatrix, brick.idx[i].x, brick.idx[i].y)){
            check = false;
        }
    }
    return check;
}
bool canBrickRight(vector<vector<tetrisObject>> &boardMatrix, tetrisBrick &brick){
    bool check = true;
    for(int i = 0; i < 4; i++){
        if(!canRight(boardMatrix, brick.idx[i].x, brick.idx[i].y)){
            check = false;
        }
    }
    return check;
}
bool canBrickFall(vector<vector<tetrisObject>> &boardMatrix, tetrisBrick &brick){
    bool check = true;
    for(int i = 0; i < 4; i++){
        if(!canFall(boardMatrix, brick.idx[i].x, brick.idx[i].y)){
            check = false;
        }
    }
    return check;
}
bool canFall(vector<vector<tetrisObject>> &boardMatrix, const int &i, const int &j){
    bool check = false;
        if(i + 1 <= BOARD_ROWS - 1){
            if(!boardMatrix[i+1][j].occupied){
                check = true;
            }
        }
    return check;
}
bool canLeft(vector<vector<tetrisObject>> &boardMatrix, const int &i, const int &j){
    bool check = false;
        if(j - 1 >= 0){
            if(!boardMatrix[i][j-1].occupied){
                check = true;
            }
        }
    return check;
}
bool canRight(vector<vector<tetrisObject>> &boardMatrix, const int &i, const int &j){
    bool check = false;
        if(j + 1 <= BOARD_COLUMNS - 1){
            if(!boardMatrix[i][j+1].occupied){
                check = true;
            }
        }
    return check;
}
void brickMoveLeft(vector<vector<tetrisObject>> &boardMatrix, tetrisBrick &brick){
    if(canBrickLeft(boardMatrix, brick)){
        for(int i = 0; i < 4; i++){

                brick.idx[i].y--;
        }
    }
}
void brickMoveRight(vector<vector<tetrisObject>> &boardMatrix, tetrisBrick &brick){
    if(canBrickRight(boardMatrix, brick)){
        for(int i = 0; i < 4; i++){
                brick.idx[i].y++;
        }
    }
}
void brickFallDown(vector<vector<tetrisObject>> &boardMatrix, tetrisTimer timer, tetrisBrick &brick){
    static int timeCheck = timer.tetrisTimerGetTicks();
    if(isGameOver(boardMatrix)){
        timeCheck = 0;
    } else {
        if(timer.tetrisTimerGetTicks() - timeCheck >= brick.vel)
        {
            if(canBrickFall(boardMatrix, brick)){
                for(int i = 0; i < 4; i++){
                    brick.idx[i].x++;
                }
            }
            timeCheck = timer.tetrisTimerGetTicks();
        }
    }
}
void eventHandler(SDL_Event &e, vector<vector<tetrisObject>> &boardMatrix, tetrisBrick &brick, tetrisTimer &timer, bool &quit, bool* optionList, int &arrow_X, int &arrow_Y){
    if(!optionList[PLAY_OPTION]){
        if(!optionList[OPTIONS_OPTION] && !optionList[TUTORIAL_OPTION]){

            if(e.type == SDL_KEYDOWN && e.key.repeat == 0){
                if(e.key.keysym.sym == SDLK_RETURN){
                    switch(arrow_X){
                        case ARROW_X1:
                            optionList[PLAY_OPTION] = true;
                            boardMatrix = generateMatrix();
                            timer.tetrisTimerStart();
                        break;
                        case ARROW_X2:
                            optionList[OPTIONS_OPTION] = true;
                            arrow_X = ARROW_X5;
                            arrow_Y = ARROW_Y5;
                        break;
                        case ARROW_X3:
                            optionList[TUTORIAL_OPTION] = true;
                        break;
                        case ARROW_X4:
                            quit = true;
                        break;
                    };
                }
                if(e.key.keysym.sym == SDLK_s){
                    switch(arrow_X){
                        case ARROW_X1:
                            arrow_X = ARROW_X2;
                            arrow_Y = ARROW_Y2;
                        break;
                        case ARROW_X2:
                            arrow_X = ARROW_X3;
                            arrow_Y = ARROW_Y3;
                        break;
                        case ARROW_X3:
                            arrow_X = ARROW_X4;
                            arrow_Y = ARROW_Y4;
                        break;
                    };
                }
                if(e.key.keysym.sym == SDLK_w){
                    switch(arrow_X){
                        case ARROW_X2:
                            arrow_X = ARROW_X1;
                            arrow_Y = ARROW_Y1;
                        break;
                        case ARROW_X3:
                            arrow_X = ARROW_X2;
                            arrow_Y = ARROW_Y2;
                        break;
                        case ARROW_X4:
                            arrow_X = ARROW_X3;
                            arrow_Y = ARROW_Y3;
                        break;
                    };
                }
            }

        } else if(optionList[OPTIONS_OPTION]){
            if(e.type == SDL_KEYDOWN && e.key.repeat == 0){
                if(e.key.keysym.sym == SDLK_w){
                    if(arrow_X == ARROW_X5){
                        arrow_X = ARROW_X6;
                        arrow_Y = ARROW_Y6;
                    }
                }
                if(e.key.keysym.sym == SDLK_s){
                    if(arrow_X == ARROW_X6){
                        arrow_X = ARROW_X5;
                        arrow_Y = ARROW_Y5;
                    }
                }
                if(e.key.keysym.sym == SDLK_RETURN){
                    if(arrow_X == ARROW_X5){
                        arrow_X = ARROW_X2;
                        arrow_Y = ARROW_Y2;
                        optionList[OPTIONS_OPTION] = false;
                    }
                    if(arrow_X == ARROW_X6){
                        if(optionList[MUSIC_OPTION]){
                            optionList[MUSIC_OPTION] = false;
                        } else {
                            optionList[MUSIC_OPTION] = true;
                        }
                    }
                }
            }
        } else if(optionList[TUTORIAL_OPTION]){
            arrow_X = ARROW_X7;
            arrow_Y = ARROW_Y7;
            if(e.type == SDL_KEYDOWN && e.key.repeat == 0){
                if(e.key.keysym.sym == SDLK_RETURN){
                    optionList[TUTORIAL_OPTION] = false;
                    arrow_X = ARROW_X3;
                    arrow_Y = ARROW_Y3;
                }
            }
        }
    } else {
        if(isGameOver(boardMatrix)){
            if(e.type == SDL_KEYDOWN && e.key.repeat == 0){
                if(e.key.keysym.sym == SDLK_t){
                    boardMatrix = generateMatrix();
                    timer.tetrisTimerStart();
                } else if(e.key.keysym.sym == SDLK_e){
                    optionList[PLAY_OPTION] = false;
                }
            }
        } else {
            if(e.type == SDL_KEYDOWN && e.key.repeat == 0){
                switch(e.key.keysym.sym){
                    case SDLK_a:
                        brickMoveLeft(boardMatrix, brick);
                    break;
                    case SDLK_d:
                        brickMoveRight(boardMatrix, brick);
                    break;
                    case SDLK_s:
                        brick.vel = 1;
                    break;
                    case SDLK_w:
                        rotate(boardMatrix, brick);
                    break;
                };
            } else if(e.type == SDL_KEYUP && e.key.repeat == 0){
                if(e.key.keysym.sym == SDLK_s){
                    brick.vel = OBJECT_VEL;
                }
            }
        }
    }
}

vector<vector<tetrisObject>> generateMatrix(){
    vector<vector<tetrisObject>> boardMatrix(BOARD_ROWS);
    for(int i = 0; i < BOARD_ROWS; i++){
        boardMatrix[i] = vector<tetrisObject>(BOARD_COLUMNS);
        for(int j = 0; j < BOARD_COLUMNS; j++){
            boardMatrix[i][j].coordinate = {BLOCK_SIZE*(j+2), i*BLOCK_SIZE};
        }
    }
    return boardMatrix;
}

void renderBoard(const vector<vector<tetrisObject>> &boardMatrix, tetrisTexture* tetrisSpriteSheet, SDL_Renderer*& tetrisRenderer, const tetrisBrick &brick, const bool* optionList, const int &arrow_X, const int &arrow_Y){
    if(!optionList[PLAY_OPTION]){
        if(optionList[OPTIONS_OPTION]){
            tetrisSpriteSheet[TETRIS_OPTIONS_TEXTURE].renderTexture(0, 0, tetrisRenderer, nullptr);
            if(optionList[MUSIC_OPTION]){
                tetrisSpriteSheet[TETRIS_ON_TEXT].renderTexture(480, 458, tetrisRenderer, nullptr);
            } else {
                tetrisSpriteSheet[TETRIS_OFF_TEXT].renderTexture(480, 458, tetrisRenderer, nullptr);
            }
        } else if(optionList[TUTORIAL_OPTION]){
            tetrisSpriteSheet[TETRIS_TUTORIAL_TEXTURE].renderTexture(0, 0, tetrisRenderer, nullptr);
        } else {
            tetrisSpriteSheet[TETRIS_HOME_TEXTURE].renderTexture(0, 0, tetrisRenderer, nullptr);
        }
        tetrisSpriteSheet[TETRIS_ARROW_TEXT].renderTexture(arrow_X, arrow_Y, tetrisRenderer, nullptr);
    } else {
        tetrisSpriteSheet[TETRIS_BACKGROUND_TEXTURE].renderTexture(0, 0, tetrisRenderer, nullptr);
        tetrisSpriteSheet[TETRIS_SCORE_BOX].renderTexture(520, 360, tetrisRenderer, nullptr);
        tetrisSpriteSheet[TETRIS_LINES_BOX].renderTexture(520, 600, tetrisRenderer, nullptr);
        tetrisSpriteSheet[TETRIS_TIME_BOX].renderTexture(520, 480, tetrisRenderer, nullptr);
        tetrisSpriteSheet[TETRIS_SCORE_COUNT].renderTexture(570, 400, tetrisRenderer, nullptr);
        tetrisSpriteSheet[TETRIS_LINES_COUNT].renderTexture(570, 640, tetrisRenderer, nullptr);
        tetrisSpriteSheet[TETRIS_TIME_COUNT].renderTexture(570, 520, tetrisRenderer, nullptr);
        tetrisSpriteSheet[TETRIS_LEVEL_BOX].renderTexture(520, 0, tetrisRenderer, nullptr);
        tetrisSpriteSheet[TETRIS_LEVEL_COUNT].renderTexture(570, 40, tetrisRenderer, nullptr);
        for(int i = 0; i < BOARD_ROWS; i++){
            for(int j = 0; j < BOARD_COLUMNS; j++){
                if(boardMatrix[i][j].occupied){
                    tetrisSpriteSheet[boardMatrix[i][j].color].renderTexture(boardMatrix[i][j].coordinate.x, boardMatrix[i][j].coordinate.y, tetrisRenderer, nullptr);
                }
            }
        }
        for(int i = 0; i < brick.idx.size(); i++){
            tetrisSpriteSheet[brick.color].renderTexture(boardMatrix[ brick.idx[i].x ][ brick.idx[i].y ].coordinate.x, boardMatrix[ brick.idx[i].x ][ brick.idx[i].y ].coordinate.y, tetrisRenderer, nullptr);
        }
    }
}

bool loadMedia(tetrisTexture* tetrisSpriteSheet, SDL_Renderer*& tetrisRenderer, TTF_Font*& tetrisFont, Mix_Music*& music){
    bool success = true;

    tetrisFont = TTF_OpenFont("fonts/joystix monospace.otf", 35);
    SDL_Color textColor = {0, 0, 0, 255};

    music = Mix_LoadMUS("sounds/OMFG - I Love You.mp3");
    if(music == nullptr){
        cout << "Load Music Failed | " << Mix_GetError() << endl;
        success = false;
    }
    if(tetrisFont == nullptr){
        cout << "TTF Open Font Failed" << endl;
        success = false;
    }
    if(!tetrisSpriteSheet[TETRIS_BACKGROUND_TEXTURE].loadFromFile("textures/background.png", tetrisRenderer)){
        cout << "Load Tetris Background Failed" << endl;
        success = false;
    }
    if(!tetrisSpriteSheet[TETRIS_SCORE_BOX].loadFromFile("textures/scorebox.png", tetrisRenderer)){
        cout << "Load Tetris Score Box Failed" << endl;
        success = false;
    }
    if(!tetrisSpriteSheet[TETRIS_LINES_BOX].loadFromFile("textures/linesbox.png", tetrisRenderer)){
        cout << "Load Tetris Lines Box Failed" << endl;
        success = false;
    }
    if(!tetrisSpriteSheet[TETRIS_TIME_BOX].loadFromFile("textures/timebox.png", tetrisRenderer)){
        cout << "Load Tetris Time Box Failed" << endl;
        success = false;
    }
    if(!tetrisSpriteSheet[TETRIS_BLOCK_TEXTURE].loadFromFile("textures/block.png", tetrisRenderer)){
        cout << "Load Tetris Block Texture Failed" << endl;
        success = false;
    }
    if(!tetrisSpriteSheet[TETRIS_BLOCK_TEXTURE_1].loadFromFile("textures/block1.png", tetrisRenderer)){
        cout << "Load Tetris Block Texture Failed" << endl;
        success = false;
    }
    if(!tetrisSpriteSheet[TETRIS_BLOCK_TEXTURE_2].loadFromFile("textures/block2.png", tetrisRenderer)){
        cout << "Load Tetris Block Texture Failed" << endl;
        success = false;
    }
    if(!tetrisSpriteSheet[TETRIS_BLOCK_TEXTURE_3].loadFromFile("textures/block3.png", tetrisRenderer)){
        cout << "Load Tetris Block Texture Failed" << endl;
        success = false;
    }
    if(!tetrisSpriteSheet[TETRIS_BLOCK_TEXTURE_4].loadFromFile("textures/block4.png", tetrisRenderer)){
        cout << "Load Tetris Block Texture Failed" << endl;
        success = false;
    }
    if(!tetrisSpriteSheet[TETRIS_BLOCK_TEXTURE_5].loadFromFile("textures/block5.png", tetrisRenderer)){
        cout << "Load Tetris Block Texture Failed" << endl;
        success = false;
    }
    if(!tetrisSpriteSheet[TETRIS_GAMEOVER_TEXTURE].loadFromFile("textures/gameover.png", tetrisRenderer)){
        cout << "Load Tetris Game Over Texture Failed" << endl;
        success = false;
    }
    if(!tetrisSpriteSheet[TETRIS_LEVEL_BOX].loadFromFile("textures/levelbox.png", tetrisRenderer)){
        cout << "Load Tetris LEVEL BOX Texture Failed" << endl;
        success = false;
    }
    if(!tetrisSpriteSheet[TETRIS_HOME_TEXTURE].loadFromFile("textures/home.png", tetrisRenderer)){
        cout << "Load Tetris Home Texture Failed" << endl;
        success = false;
    }
    textColor = {255, 255, 255, 255};
    if(!tetrisSpriteSheet[TETRIS_ARROW_TEXT].loadFromText(tetrisFont, ">", tetrisRenderer, textColor)){
        cout << "Load Tetris Arrow Text Failed" << endl;
        success = false;
    }
    if(!tetrisSpriteSheet[TETRIS_OPTIONS_TEXTURE].loadFromFile("textures/options.png", tetrisRenderer)){
        cout << "Load Tetris Option Texture Failed" << endl;
        success = false;
    }
    if(!tetrisSpriteSheet[TETRIS_TUTORIAL_TEXTURE].loadFromFile("textures/tutorial.png", tetrisRenderer)){
        cout << "Load Tetris Tutorial Texture Failed" << endl;
        success = false;
    }
    if(!tetrisSpriteSheet[TETRIS_ON_TEXT].loadFromText(tetrisFont, "On", tetrisRenderer, textColor)){
        cout << "Load Tetris On Text Failed" << endl;
        success = false;
    }
    if(!tetrisSpriteSheet[TETRIS_OFF_TEXT].loadFromText(tetrisFont, "Off", tetrisRenderer, textColor)){
        cout << "Load Tetris Off Text Failed" << endl;
        success = false;
    }
    return success;
}

tetrisTexture::tetrisTexture(){
    mTexture = nullptr;
    tetrisTextureWidth = 0;
    tetrisTextureHeight = 0;
}

tetrisTexture::~tetrisTexture(){
    freeTexture();
}

int tetrisTexture::getWidth(){
    return tetrisTextureWidth;
}

int tetrisTexture::getHeight(){
    return tetrisTextureHeight;
}
void tetrisTexture::freeTexture(){
    if(mTexture != nullptr){
        SDL_DestroyTexture(mTexture);
        mTexture= nullptr;
        tetrisTextureWidth = 0;
        tetrisTextureHeight = 0;
    }
}

bool tetrisTexture::loadFromFile(const string &filePath, SDL_Renderer*& tetrisRenderer){
    freeTexture();
    bool success = true;
    SDL_Surface* loadedSurface = IMG_Load(filePath.c_str());
    if(loadedSurface == nullptr){
        cout << "Load Surface From File Failed" << endl;
        success = false;
    } else {
        mTexture = SDL_CreateTextureFromSurface(tetrisRenderer, loadedSurface);
        if(mTexture == nullptr){
            cout << "Create Texture From Surface Failed" << endl;
            success = false;
        } else {
            // tang ti le kich thuoc hinh len 5 lan
            if(loadedSurface->w <= 160 && loadedSurface->h <= 144){
                tetrisTextureWidth = (loadedSurface->w)*5;
                tetrisTextureHeight = (loadedSurface->h)*5;
            } else {
                tetrisTextureWidth = loadedSurface->w;
                tetrisTextureHeight = loadedSurface->h;
            }
        }
    }
    SDL_FreeSurface(loadedSurface);
    return success;
}

bool tetrisTexture::loadFromText(TTF_Font*& font, const string &text, SDL_Renderer*& tetrisRenderer, const SDL_Color &textColor){
    freeTexture();
    bool success = true;
    SDL_Surface* loadedSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
    if(loadedSurface == nullptr){
        cout << "Load Surface From Text Failed" << TTF_GetError() << endl;
        success = false;
    } else {
        mTexture = SDL_CreateTextureFromSurface(tetrisRenderer, loadedSurface);
        if(mTexture == nullptr){
            cout << "Create Texture From Text Surface Failed" << endl;
            success = false;
        } else {
            tetrisTextureWidth = loadedSurface->w;
            tetrisTextureHeight = loadedSurface->h;
        }
    }
    SDL_FreeSurface(loadedSurface);
    return success;
}

void tetrisTexture::renderTexture(const int &x, const int &y, SDL_Renderer*& tetrisRenderer, SDL_Rect* clipRect){
    SDL_Rect dstRect;
    dstRect.w = tetrisTextureWidth;
    dstRect.h = tetrisTextureHeight;
    dstRect.x = x;
    dstRect.y = y;
    if(clipRect != nullptr){
        dstRect.w = clipRect->w;
        dstRect.h = clipRect->h;
    }
    SDL_RenderCopy(tetrisRenderer, mTexture, clipRect, &dstRect);
}

tetrisTimer::tetrisTimer(){
    startTime = 0;
    pausedTime = 0;
    tetrisStarted = false;
    tetrisPaused = false;
}

int tetrisTimer::tetrisTimerGetTicks(){
    unsigned int currentTime = 0;
    if(tetrisStarted == true){
        if(tetrisPaused == false){
            currentTime = SDL_GetTicks() - startTime;
        } else {
            currentTime = pausedTime;
        }
    }
    return currentTime;
}

void tetrisTimer::tetrisTimerStart(){
        tetrisStarted = true;
        tetrisPaused = false;
        startTime = SDL_GetTicks();
        pausedTime = 0;
}

void tetrisTimer::tetrisTimerPause(){
    if((tetrisStarted == true) && (tetrisPaused == false)){
        tetrisPaused = true;
        pausedTime = SDL_GetTicks() - startTime;
        startTime = 0;
    }
}

void tetrisTimer::tetrisTimerUnpause(){
    if((tetrisStarted == true) && (tetrisPaused == true)){
        tetrisPaused = false;
        startTime = SDL_GetTicks() - pausedTime;
        pausedTime = 0;
    }
}

void tetrisTimer::tetrisTimerStop(){
        tetrisStarted = false;
        tetrisPaused = false;
        startTime = 0;
        pausedTime = 0;
}
bool tetrisTimer::isTimerStarted(){
    return tetrisStarted;
}

bool tetrisTimer::isTimerPaused(){
    return tetrisPaused;
}

bool initSDL(SDL_Window*& tetrisWindow, SDL_Renderer*& tetrisRenderer){
    bool success = true;
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0){
        cout << "SDL Failed to Init Video & Audio | " << SDL_GetError() << endl;
        success = false;
    } else {
        tetrisWindow = SDL_CreateWindow(WINDOW_TITLE.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if(tetrisWindow == nullptr){
            cout << "Create Tetris Window Failed | " << SDL_GetError() << endl;
            success = false;
        }   else {
                tetrisRenderer = SDL_CreateRenderer(tetrisWindow,-1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
                if(tetrisRenderer == nullptr){
                    cout << "Create Tetris Renderer Failed | " << SDL_GetError() << endl;
                    success = false;
                } else SDL_SetRenderDrawColor(tetrisRenderer, 0, 0, 0, 255);
            }
        if(!(IMG_Init(IMG_INIT_PNG)&IMG_INIT_PNG)){
            cout << "IMG Init Failed | " << IMG_GetError() << endl;
            success = false;
        }
        if(TTF_Init() < 0){
            cout << "TTF Init Failed | " << TTF_GetError() << endl;
            success = false;
        }
        if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
        {
            printf( "SDL_mixer could not initialize! | ", Mix_GetError() );
            success = false;
        }
    }
    return success;
}

void closeSDL(SDL_Window*& tetrisWindow, SDL_Renderer*& tetrisRenderer, tetrisTexture* tetrisSpriteSheet, TTF_Font*& tetrisFont, Mix_Music*& music){
    for(int i = 0; i < TETRIS_TOTAL_IMAGE; i++){
        tetrisSpriteSheet[i].freeTexture();
    }
    Mix_FreeMusic(music);
    music = nullptr;
    TTF_CloseFont(tetrisFont);
    tetrisFont = nullptr;
    SDL_DestroyRenderer(tetrisRenderer);
    tetrisRenderer = nullptr;
    SDL_DestroyWindow(tetrisWindow);
    tetrisWindow = nullptr;
    TTF_Quit();
    IMG_Quit();
    Mix_Quit();
    SDL_Quit();
}
