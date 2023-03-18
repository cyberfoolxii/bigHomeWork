#include "TETRIS_Utils.h"
using namespace std;
const string WINDOW_TITLE = "Tetris Game - Cuong Nguyen";
const int BOARD_ROWS = 18;
const int BOARD_COLUMNS = 10;
const int BLOCK_SIZE = 40;
const int OBJECT_VEL = 1000;

enum tetrisTextureFlags{
    TETRIS_BACKGROUND_TEXTURE,
    TETRIS_SCORE_TEXT,
    TETRIS_SCORE_COUNT,
    TETRIS_LINES_TEXT,
    TETRIS_LINES_COUNT,
    TETRIS_TIME_TEXT,
    TETRIS_TIME_COUNT,
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
    bool loadFromFile(const string &filePath, SDL_Renderer*& tetrisRenderer);
    bool loadFromText(TTF_Font*& font, const string &text, SDL_Renderer*& tetrisRenderer);
    void renderTexture(const int &x, const int &y, SDL_Renderer*& tetrisRenderer, SDL_Rect* clipRect);
private:
    SDL_Texture* mTexture;
    int tetrisTextureWidth;
    int tetrisTextureHeight;
};

class tetrisObject{
public:
    SDL_Point coordinate;
    bool occupied;
    tetrisObject(){
        coordinate = {0, 0};
        occupied = false;
    }
};

class tetrisBrick{
    //tạm dùng mảng sdl point để giữ các vị trí index của khối gạch trong board
    public:
    vector<SDL_Point> idx;
    void pickShape(){
        srand(time(0));
        switch(rand()%8 + 1){
            case 1: // _
                idx = {
                    {0, 3},
                    {0, 4},
                    {0, 5},
                    {0, 6}
                    };
            break;
            case 2: // I
                idx = {
                    {0, 4},
                    {1, 4},
                    {2, 4},
                    {3, 4}
                };
            break;
            case 3: // [|]
                idx = {
                    {0, 4},
                    {0, 5},
                    {1, 4},
                    {1, 5}
                };
            break;
            case 4: // w
                idx = {
                    {0, 4},
                    {1, 3},
                    {1, 4},
                    {1, 5}
                    };
            break;
            case 5: // z
                idx = {
                    {0, 3},
                    {0, 4},
                    {1, 4},
                    {1, 5}
                };
            break;
            case 6: // s
                idx = {
                    {0, 5},
                    {0, 6},
                    {1, 4},
                    {1, 5}
                };
            break;
            case 7: // L
                idx = {
                    {0, 4},
                    {1, 4},
                    {2, 4},
                    {2, 5}
                };
            break;
            case 8: // J
                idx = {
                    {0, 5},
                    {1, 5},
                    {2, 5},
                    {2, 4}
                };
            break;
        };
    }
    tetrisBrick(){
        pickShape();
    }
};
bool initSDL(SDL_Window*& tetrisWindow, SDL_Renderer*& tetrisRenderer);
void closeSDL(SDL_Window*& tetrisWindow, SDL_Renderer*& tetrisRenderer, tetrisTexture* tetrisSpriteSheet, TTF_Font*& tetrisFont);
bool loadMedia(tetrisTexture* tetrisSpriteSheet, SDL_Renderer*& tetrisRenderer, TTF_Font*& tetrisFont);
vector<vector<tetrisObject>> generateMatrix();

void renderBoard(const vector<vector<tetrisObject>> &boardMatrix, tetrisTexture* tetrisSpriteSheet, SDL_Renderer*& tetrisRenderer, const tetrisBrick &brick);

void eventHandler(SDL_Event &e, vector<vector<tetrisObject>> &boardMatrix, tetrisBrick &brick, int &VEL);

bool canFall(vector<vector<tetrisObject>> &boardMatrix, const int &i, const int &j);
bool canLeft(vector<vector<tetrisObject>> &boardMatrix, const int &i, const int &j);
bool canRight(vector<vector<tetrisObject>> &boardMatrix, const int &i, const int &j);
void moveLeft(vector<vector<tetrisObject>> &boardMatrix, tetrisBrick &brick);
void moveRight(vector<vector<tetrisObject>> &boardMatrix, tetrisBrick &brick);
void fallDown(vector<vector<tetrisObject>> &boardMatrix, tetrisTimer timer, tetrisBrick &brick, const int &VEL);
bool canBrickFall(vector<vector<tetrisObject>> &boardMatrix, tetrisBrick &brick);
bool canBrickLeft(vector<vector<tetrisObject>> &boardMatrix, tetrisBrick &brick);
bool canBrickRight(vector<vector<tetrisObject>> &boardMatrix, tetrisBrick &brick);
void setBrick(vector<vector<tetrisObject>> &boardMatrix, tetrisBrick &brick);

int main(int argc, char* argv[]){
    SDL_Window* tetrisWindow = nullptr;
    SDL_Renderer* tetrisRenderer = nullptr;
    tetrisTexture tetrisSpriteSheet[TETRIS_TOTAL_IMAGE];
    TTF_Font* tetrisFont = nullptr;
    tetrisTimer timer;
    int VEL = OBJECT_VEL;
    //stringstream sstream;

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
                eventHandler(tetrisEvent, gameBoard, brick, VEL);
            }
            SDL_RenderClear(tetrisRenderer);
            tetrisSpriteSheet[TETRIS_BACKGROUND_TEXTURE].renderTexture(0, 0, tetrisRenderer, nullptr);
            tetrisSpriteSheet[TETRIS_SCORE_TEXT].renderTexture(570, 360, tetrisRenderer, nullptr);
            tetrisSpriteSheet[TETRIS_LINES_TEXT].renderTexture(570, 600, tetrisRenderer, nullptr);
            tetrisSpriteSheet[TETRIS_TIME_TEXT].renderTexture(570, 480, tetrisRenderer, nullptr);

            setBrick(gameBoard, brick);

            renderBoard(gameBoard, tetrisSpriteSheet, tetrisRenderer, brick);

            fallDown(gameBoard, timer, brick, VEL);
            static int check = timer.tetrisTimerGetTicks();
            if(timer.tetrisTimerGetTicks() - check >= VEL)
            {
                            cout << canFall(gameBoard, brick.idx[0].x, brick.idx[0].y) << endl;
                cout << canFall(gameBoard, brick.idx[1].x, brick.idx[1].y) << endl;
                cout << canFall(gameBoard, brick.idx[2].x, brick.idx[2].y) << endl;
                cout << canFall(gameBoard, brick.idx[3].x, brick.idx[3].y) << endl;
                cout << endl;
                check = timer.tetrisTimerGetTicks();
            }
            SDL_RenderPresent(tetrisRenderer);
            SDL_Delay(20);
        }
    }
    closeSDL(tetrisWindow, tetrisRenderer, tetrisSpriteSheet, tetrisFont);
    return 0;
}
void setBrick(vector<vector<tetrisObject>> &boardMatrix, tetrisBrick &brick){
    bool check = true;
    if(canBrickFall(boardMatrix, brick)){
        check = false;
    } else {
        for(int i = 0; i < 4; i++){
            boardMatrix[ brick.idx[i].x ][ brick.idx[i].y ].occupied = true;
        }
    }
    if(check){
        brick.pickShape();
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
void moveLeft(vector<vector<tetrisObject>> &boardMatrix, tetrisBrick &brick){
    if(canBrickLeft(boardMatrix, brick)){
        for(int i = 0; i < 4; i++){

                brick.idx[i].y--;
        }
    }
}
void moveRight(vector<vector<tetrisObject>> &boardMatrix, tetrisBrick &brick){
    if(canBrickRight(boardMatrix, brick)){
        for(int i = 0; i < 4; i++){
                brick.idx[i].y++;
        }
    }
}
void fallDown(vector<vector<tetrisObject>> &boardMatrix, tetrisTimer timer, tetrisBrick &brick, const int &VEL){
    static int check = timer.tetrisTimerGetTicks();
    if(timer.tetrisTimerGetTicks() - check >= VEL)
    {
        if(canBrickFall(boardMatrix, brick)){
            for(int i = 0; i < 4; i++){
                brick.idx[i].x++;
            }
        }
        check = timer.tetrisTimerGetTicks();
    }
}
void eventHandler(SDL_Event &e, vector<vector<tetrisObject>> &boardMatrix, tetrisBrick &brick, int &VEL){
    if(e.type == SDL_KEYDOWN && e.key.repeat == 0){
        switch(e.key.keysym.sym){
            case SDLK_a:
                moveLeft(boardMatrix, brick);
            break;
            case SDLK_d:
                moveRight(boardMatrix, brick);
            break;
            case SDLK_s:
                VEL -= 900;
            break;
        };
    } else if(e.type == SDL_KEYUP && e.key.repeat == 0){
        if(e.key.keysym.sym == SDLK_s){
            VEL += 900;
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

void renderBoard(const vector<vector<tetrisObject>> &boardMatrix, tetrisTexture* tetrisSpriteSheet, SDL_Renderer*& tetrisRenderer, const tetrisBrick &brick){
    for(int i = 0; i < BOARD_ROWS; i++){
        for(int j = 0; j < BOARD_COLUMNS; j++){
            if(boardMatrix[i][j].occupied){
                tetrisSpriteSheet[TETRIS_BLOCK_TEXTURE_1].renderTexture(boardMatrix[i][j].coordinate.x, boardMatrix[i][j].coordinate.y, tetrisRenderer, nullptr);
            }
        }
    }
    for(int i = 0; i < 4; i++){
        tetrisSpriteSheet[TETRIS_BLOCK_TEXTURE_1].renderTexture(boardMatrix[ brick.idx[i].x ][ brick.idx[i].y ].coordinate.x, boardMatrix[ brick.idx[i].x ][ brick.idx[i].y ].coordinate.y, tetrisRenderer, nullptr);
    }
}

bool loadMedia(tetrisTexture* tetrisSpriteSheet, SDL_Renderer*& tetrisRenderer, TTF_Font*& tetrisFont){
    bool success = true;
    tetrisFont = TTF_OpenFont("fonts/joystix monospace.otf", 35);
    if(tetrisFont == nullptr){
        cout << "TTF Open Font Failed" << endl;
        success = false;
    }
    if(!tetrisSpriteSheet[TETRIS_BACKGROUND_TEXTURE].loadFromFile("textures/background.png", tetrisRenderer)){
        cout << "Load Tetris Background Failed" << endl;
        success = false;
    }
    if(!tetrisSpriteSheet[TETRIS_SCORE_TEXT].loadFromText(tetrisFont, "SCORE", tetrisRenderer)){
        cout << "Load Tetris Score Box Failed" << endl;
        success = false;
    }
    if(!tetrisSpriteSheet[TETRIS_LINES_TEXT].loadFromText(tetrisFont, "LINES", tetrisRenderer)){
        cout << "Load Tetris Lines Box Failed" << endl;
        success = false;
    }
    if(!tetrisSpriteSheet[TETRIS_TIME_TEXT].loadFromText(tetrisFont, "TIME", tetrisRenderer)){
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
            tetrisTextureWidth = (loadedSurface->w)*5;
            tetrisTextureHeight = (loadedSurface->h)*5;
        }
    }
    SDL_FreeSurface(loadedSurface);
    return success;
}

bool tetrisTexture::loadFromText(TTF_Font*& font, const string &text, SDL_Renderer*& tetrisRenderer){
    freeTexture();
    bool success = true;
    SDL_Color textColor = {0, 0, 0, 255};
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
    }
    return success;
}

void closeSDL(SDL_Window*& tetrisWindow, SDL_Renderer*& tetrisRenderer, tetrisTexture* tetrisSpriteSheet, TTF_Font*& tetrisFont){
    // do total image khong duoc cap phat
    for(int i = 0; i < TETRIS_TOTAL_IMAGE; i++){
        tetrisSpriteSheet[i].freeTexture();
    }
    TTF_CloseFont(tetrisFont);
    tetrisFont = nullptr;
    SDL_DestroyRenderer(tetrisRenderer);
    tetrisRenderer = nullptr;
    SDL_DestroyWindow(tetrisWindow);
    tetrisWindow = nullptr;
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}
