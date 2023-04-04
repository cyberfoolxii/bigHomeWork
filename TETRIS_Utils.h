#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <sstream>
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 720

const std::string WINDOW_TITLE = "Tetris Game - Cuong Nguyen";
const int BOARD_ROWS = 18;
const int BOARD_COLUMNS = 10;
const int BLOCK_SIZE = 40;
const int OBJECT_VEL = 1000;

enum homeOptions{
    PLAY_OPTION,
    OPTIONS_OPTION,
    TUTORIAL_OPTION,
    TOTAL_OPTIONS
};

enum arrowCoor{
    ARROW_X1 = 310,
    ARROW_Y1 = 425,
    ARROW_X2 = 280,
    ARROW_Y2 = 470,
    ARROW_X3 = 260,
    ARROW_Y3 = 513,
    ARROW_X4 = 312,
    ARROW_Y4 = 555,
};
enum tetrisTextureFlags{
    TETRIS_BACKGROUND_TEXTURE,
    TETRIS_GAMEOVER_TEXTURE,
    TETRIS_SCORE_TEXT,
    TETRIS_SCORE_COUNT,
    TETRIS_LINES_TEXT,
    TETRIS_LINES_COUNT,
    TETRIS_TIME_TEXT,
    TETRIS_TIME_COUNT,
    TETRIS_LEVEL_BOX,
    TETRIS_LEVEL_COUNT,
    TETRIS_HOME_TEXTURE,
    TETRIS_ARROW_TEXT,
    TETRIS_BLOCK_TEXTURE,
    TETRIS_BLOCK_TEXTURE_1,
    TETRIS_BLOCK_TEXTURE_2,
    TETRIS_BLOCK_TEXTURE_3,
    TETRIS_BLOCK_TEXTURE_4,
    TETRIS_BLOCK_TEXTURE_5,
    TETRIS_TOTAL_IMAGE
};

class tetrisTimer{
public:
    tetrisTimer();
    int tetrisTimerGetTicks();
    void tetrisTimerStart();
    void tetrisTimerPause();
    void tetrisTimerUnpause();
    void tetrisTimerStop();
    bool isTimerStarted();
    bool isTimerPaused();
private:
    unsigned int startTime;
    unsigned int pausedTime;
    bool tetrisStarted;
    bool tetrisPaused;
};
class tetrisTexture{
public:
    tetrisTexture();
    ~tetrisTexture();
    int getWidth();
    int getHeight();
    void freeTexture();
    bool loadFromFile(const std::string &filePath, SDL_Renderer*& tetrisRenderer);
    bool loadFromText(TTF_Font*& font, const std::string &text, SDL_Renderer*& tetrisRenderer, const SDL_Color &textColor);
    void renderTexture(const int &x, const int &y, SDL_Renderer*& tetrisRenderer, SDL_Rect* clipRect);
private:
    SDL_Texture* mTexture;
    int tetrisTextureWidth;
    int tetrisTextureHeight;
};

class tetrisObject{
public:
    SDL_Point coordinate;
    tetrisTextureFlags color = TETRIS_BLOCK_TEXTURE;
    bool occupied;
    tetrisObject(){
        coordinate = {0, 0};
        occupied = false;
    }
};

class tetrisBrick{
    //tạm dùng mảng sdl point để giữ các vị trí index của khối gạch trong board
    public:
    int shapeCheck = 0;
    tetrisTextureFlags color = TETRIS_BLOCK_TEXTURE;
    std::vector<SDL_Point> idx;
    int randNum;
    void pickShape();
    void pickColor();
    tetrisBrick();
};

bool initSDL(SDL_Window*& tetrisWindow, SDL_Renderer*& tetrisRenderer);
void closeSDL(SDL_Window*& tetrisWindow, SDL_Renderer*& tetrisRenderer, tetrisTexture* tetrisSpriteSheet, TTF_Font*& tetrisFont);
bool loadMedia(tetrisTexture* tetrisSpriteSheet, SDL_Renderer*& tetrisRenderer, TTF_Font*& tetrisFont);
std::vector<std::vector<tetrisObject>> generateMatrix();
void renderBoard(const std::vector<std::vector<tetrisObject>> &boardMatrix, tetrisTexture* tetrisSpriteSheet, SDL_Renderer*& tetrisRenderer, const tetrisBrick &brick, const bool* optionList, const int &arrow_X, const int &arrow_Y);
void eventHandler(SDL_Event &e, std::vector<std::vector<tetrisObject>> &boardMatrix, tetrisBrick &brick, tetrisTimer &timer, int &VEL, bool &quit, bool* optionList, int &arrow_X, int &arrow_Y);
bool canFall(std::vector<std::vector<tetrisObject>> &boardMatrix, const int &i, const int &j);
bool canLeft(std::vector<std::vector<tetrisObject>> &boardMatrix, const int &i, const int &j);
bool canRight(std::vector<std::vector<tetrisObject>> &boardMatrix, const int &i, const int &j);
void brickMoveLeft(std::vector<std::vector<tetrisObject>> &boardMatrix, tetrisBrick &brick);
void brickMoveRight(std::vector<std::vector<tetrisObject>> &boardMatrix, tetrisBrick &brick);
void brickFallDown(std::vector<std::vector<tetrisObject>> &boardMatrix, tetrisTimer timer, tetrisBrick &brick, const int &VEL);
bool canBrickFall(std::vector<std::vector<tetrisObject>> &boardMatrix, tetrisBrick &brick);
bool canBrickLeft(std::vector<std::vector<tetrisObject>> &boardMatrix, tetrisBrick &brick);
bool canBrickRight(std::vector<std::vector<tetrisObject>> &boardMatrix, tetrisBrick &brick);
void renderDataAndSetBrick(std::vector<std::vector<tetrisObject>> &boardMatrix, tetrisBrick &brick, tetrisTexture* tetrisSpriteSheet,
tetrisTimer &timer, TTF_Font*& font, SDL_Renderer*& tetrisRenderer, int &VEL);
void rotate(std::vector<std::vector<tetrisObject>> &boardMatrix, tetrisBrick &brick);
bool isGameOver(std::vector<std::vector<tetrisObject>> &boardMatrix);
bool isRotateable(std::vector<std::vector<tetrisObject>> &boardMatrix, std::vector<SDL_Point> &temp);
