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
    bool moving;
    char blockImage;
    tetrisObject(){
        coordinate = {0, 0};
        occupied = false;
        moving = false;
        blockImage = TETRIS_BLOCK_TEXTURE;
    }
};

bool initSDL(SDL_Window*& tetrisWindow, SDL_Renderer*& tetrisRenderer);
void closeSDL(SDL_Window*& tetrisWindow, SDL_Renderer*& tetrisRenderer, tetrisTexture* tetrisSpriteSheet, TTF_Font*& tetrisFont);
bool loadMedia(tetrisTexture* tetrisSpriteSheet, SDL_Renderer*& tetrisRenderer, TTF_Font*& tetrisFont);
vector<vector<tetrisObject>> generateMatrix();
void setObjectGravity(vector<vector<tetrisObject>> &boardMatrix, tetrisTimer timer, const int &VEL);
void renderBoard(const vector<vector<tetrisObject>> &boardMatrix, tetrisTexture* tetrisSpriteSheet, SDL_Renderer*& tetrisRenderer);
int checkScoreAndGenerateObject(vector<vector<tetrisObject>> &boardMatrix);
void eventHandler(SDL_Event &e, vector<vector<tetrisObject>> &boardMatrix, int &VEL);

int main(int argc, char* argv[]){
    SDL_Window* tetrisWindow = nullptr;
    SDL_Renderer* tetrisRenderer = nullptr;
    tetrisTexture tetrisSpriteSheet[TETRIS_TOTAL_IMAGE];
    TTF_Font* tetrisFont = nullptr;
    tetrisTimer timer;
    int VEL = OBJECT_VEL;
    stringstream sstream;
    if(!initSDL(tetrisWindow, tetrisRenderer)){
        cout << "Failed to Init SDL | " << SDL_GetError() << endl;
    } else {
        if(!loadMedia(tetrisSpriteSheet, tetrisRenderer, tetrisFont)){
            cout << "Load Tetris Media Failed" << endl;
        }
        vector<vector<tetrisObject>> boardMatrix = generateMatrix();
        timer.tetrisTimerStart();
        bool quit = false;
        while(!quit){
            setObjectGravity(boardMatrix, timer, VEL);
            SDL_Event tetrisEvent;
            while(SDL_PollEvent(&tetrisEvent) != 0){
                if(tetrisEvent.type == SDL_QUIT){
                    quit = true;
                }
                eventHandler(tetrisEvent, boardMatrix, VEL);
            }
            SDL_RenderClear(tetrisRenderer);
            tetrisSpriteSheet[TETRIS_BACKGROUND_TEXTURE].renderTexture(0, 0, tetrisRenderer, nullptr);
            tetrisSpriteSheet[TETRIS_SCORE_TEXT].renderTexture(570, 360, tetrisRenderer, nullptr);
            tetrisSpriteSheet[TETRIS_LINES_TEXT].renderTexture(570, 600, tetrisRenderer, nullptr);
            tetrisSpriteSheet[TETRIS_TIME_TEXT].renderTexture(570, 480, tetrisRenderer, nullptr);

            sstream.str("");
            sstream << checkScoreAndGenerateObject(boardMatrix);
            tetrisSpriteSheet[TETRIS_SCORE_COUNT].loadFromText(tetrisFont, sstream.str().c_str(), tetrisRenderer);
            tetrisSpriteSheet[TETRIS_SCORE_COUNT].renderTexture(570, 400, tetrisRenderer, nullptr);

            sstream.str("");
            sstream << checkScoreAndGenerateObject(boardMatrix) / 10;
            tetrisSpriteSheet[TETRIS_LINES_COUNT].loadFromText(tetrisFont, sstream.str().c_str(), tetrisRenderer);
            tetrisSpriteSheet[TETRIS_LINES_COUNT].renderTexture(570, 640, tetrisRenderer, nullptr);

            sstream.str("");
            if(timer.tetrisTimerGetTicks() >= 60000){
                sstream << timer.tetrisTimerGetTicks()/60000 << ':' << timer.tetrisTimerGetTicks()/1000 - 60*(timer.tetrisTimerGetTicks()/60000);
            } else {
                sstream << timer.tetrisTimerGetTicks()/60000 << ':' << timer.tetrisTimerGetTicks()/1000;
            }
            tetrisSpriteSheet[TETRIS_TIME_COUNT].loadFromText(tetrisFont, sstream.str().c_str(), tetrisRenderer);
            tetrisSpriteSheet[TETRIS_TIME_COUNT].renderTexture(570, 520, tetrisRenderer, nullptr);

            renderBoard(boardMatrix, tetrisSpriteSheet, tetrisRenderer);
            SDL_RenderPresent(tetrisRenderer);
            SDL_Delay(20);
        }
    }
    closeSDL(tetrisWindow, tetrisRenderer, tetrisSpriteSheet, tetrisFont);
    return 0;
}

void eventHandler(SDL_Event &e, vector<vector<tetrisObject>> &boardMatrix, int &VEL){
    if(e.type == SDL_KEYDOWN && e.key.repeat == 0){
        switch(e.key.keysym.sym){
            case SDLK_a:
                for(int j = 0; j < BOARD_COLUMNS; j++){
                    int c = 0;
                    for(int i = BOARD_ROWS - 1; i >= 0; i--){
                        if(boardMatrix[i][j].moving && boardMatrix[i][j].occupied){
                            if(j - 1 >= 0){
                                c += boardMatrix[i][j-1].occupied;
                            } else {
                                c++;
                            }
                        }
                    }
                    if(c == 0){
                        for(int k = 0; k < BOARD_ROWS; k++){
                            if(boardMatrix[k][j].moving && boardMatrix[k][j].occupied){
                                boardMatrix[k][j].occupied = false;
                                boardMatrix[k][j].moving = false;
                                boardMatrix[k][j-1].occupied = true;
                                boardMatrix[k][j-1].moving = true;
                            }
                        }
                    }
                }
            break;
            case SDLK_d:
                for(int j = BOARD_COLUMNS - 1; j >= 0; j--){
                    int c = 0;
                    for(int i = BOARD_ROWS - 1; i >= 0; i--){
                        if(boardMatrix[i][j].moving && boardMatrix[i][j].occupied){
                            if(j + 1 < BOARD_COLUMNS){
                                c += boardMatrix[i][j+1].occupied;
                            } else {
                                c++;
                            }
                        }
                    }
                    if(c == 0){
                        for(int k = 0; k < BOARD_ROWS; k++){
                            if(boardMatrix[k][j].moving && boardMatrix[k][j].occupied){
                                boardMatrix[k][j].occupied = false;
                                boardMatrix[k][j].moving = false;
                                boardMatrix[k][j+1].occupied = true;
                                boardMatrix[k][j+1].moving = true;
                            }
                        }
                    }
                }
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

void setObjectGravity(vector<vector<tetrisObject>> &boardMatrix, tetrisTimer timer, const int &VEL){
    static int check = timer.tetrisTimerGetTicks();
    if(timer.tetrisTimerGetTicks() - check >= VEL)
    {
        for(int i = BOARD_ROWS - 1; i >= 0; i--){
            int c = 0;
            for(int j = 0; j < BOARD_COLUMNS; j++){
                if(boardMatrix[i][j].occupied && boardMatrix[i][j].moving){
                    if(i < BOARD_ROWS - 1){
                        if(boardMatrix[i+1][j].occupied && !boardMatrix[i+1][j].moving){
                            c = 0;
                            break;
                        } else if(!boardMatrix[i+1][j].occupied && !boardMatrix[i+1][j].moving){
                            c++;
                        }
                    } else {
                        c = 0;
                        break;
                    }
                }
            }
            if(c == 0){
                for(int k = 0; k < BOARD_COLUMNS; k++){
                    boardMatrix[i][k].moving = false;
                }
            } else {
                for(int k = 0; k < BOARD_COLUMNS; k++){
                    if(boardMatrix[i][k].occupied && boardMatrix[i][k].moving){
                        boardMatrix[i][k].moving = false;
                        boardMatrix[i][k].occupied = false;
                        boardMatrix[i+1][k].moving = true;
                        boardMatrix[i+1][k].occupied = true;
                    }
                }
            }
        }
        check = timer.tetrisTimerGetTicks();
    }
}

void renderBoard(const vector<vector<tetrisObject>> &boardMatrix, tetrisTexture* tetrisSpriteSheet, SDL_Renderer*& tetrisRenderer){
    for(int i = 0; i < BOARD_ROWS; i++){
        for(int j = 0; j < BOARD_COLUMNS; j++){
            if(boardMatrix[i][j].occupied == true){
                tetrisSpriteSheet[TETRIS_BLOCK_TEXTURE_1].renderTexture(boardMatrix[i][j].coordinate.x, boardMatrix[i][j].coordinate.y, tetrisRenderer, nullptr);
            }
        }
    }
}

int checkScoreAndGenerateObject(vector<vector<tetrisObject>> &boardMatrix){
    static int score = 0;
    int checkMoving = 0;
    for(int i = 0; i < BOARD_ROWS; i++){
        for(int j = 0; j < BOARD_COLUMNS; j++){
            checkMoving += boardMatrix[i][j].moving;
        }
    }
    if(checkMoving == 0){
        for(int i = 0; i < BOARD_ROWS; i++){
            int checkOccupied = 0;
            for(int j = 0; j < BOARD_COLUMNS; j++){
                checkOccupied += boardMatrix[i][j].occupied;
            }
            if(checkOccupied == BOARD_COLUMNS){
                for(int j = 0; j < BOARD_COLUMNS; j++){
                    boardMatrix[i][j].occupied = false;
                }
                score += 10;
            }
        }
        srand(time(0));
        char ranNum = rand()%8 + 1;
        switch(ranNum){
            case 1: // que doc
                for(int i = 0; i <= 3; i++){
                    boardMatrix[i][4].occupied = true;
                    boardMatrix[i][4].moving = true;
                }
            break;
            case 2: // que ngang
                for(int j = 3; j <= 6; j++){
                    boardMatrix[0][j].occupied = true;
                    boardMatrix[0][j].moving = true;
                }
            break;
            case 3: // hinh vuong
                for(int i = 0; i <= 1; i++){
                    for(int j = 4; j <= 5; j++){
                        boardMatrix[i][j].occupied = true;
                        boardMatrix[i][j].moving = true;
                    }
                }
            break;
            case 4: // hinh xe tang
                boardMatrix[0][4].occupied = true;
                boardMatrix[0][4].moving = true;
                for(int j = 3; j <= 5; j++){
                    boardMatrix[1][j].occupied = true;
                    boardMatrix[1][j].moving = true;
                }
            break;
            case 5: // hinh chu z quay sang trai
                for(int j = 3; j <= 4; j++){
                    boardMatrix[0][j].occupied = true;
                    boardMatrix[0][j].moving = true;
                    boardMatrix[1][j+1].occupied = true;
                    boardMatrix[1][j+1].moving = true;
                }
            break;
            case 6: // hinh chu z quay sang phai
                for(int j = 3; j <= 4; j++){
                    boardMatrix[0][j+1].occupied = true;
                    boardMatrix[0][j+1].moving = true;
                    boardMatrix[1][j].occupied = true;
                    boardMatrix[1][j].moving = true;
                }
            break;
            case 7: // hinh chu L quay sang phai
                boardMatrix[2][5].occupied = true;
                boardMatrix[2][5].moving = true;
                for(int i = 0; i <= 2; i++){
                    boardMatrix[i][4].occupied = true;
                    boardMatrix[i][4].moving = true;
                }
            break;
            case 8: // hinh chu L quay sang trai
                boardMatrix[2][3].occupied = true;
                boardMatrix[2][3].moving = true;
                for(int i = 0; i <= 2; i++){
                    boardMatrix[i][4].occupied = true;
                    boardMatrix[i][4].moving = true;
                }
            break;
        };
    }
    return score;
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
