#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <sstream>
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 720
using namespace std;
const string WINDOW_TITLE = "Tetris Game - Cuong Nguyen";
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
const int BLOCK_SIZE = 40;
const int SPRITE_VEL = BLOCK_SIZE;
class tetrisTexture{
public:
    SDL_Point spritePos;
    tetrisTexture();
    ~tetrisTexture();
    int getWidth();
    int getHeight();
    void freeTexture();
    bool loadFromFile(const string &filePath, SDL_Renderer*& tetrisRenderer);
    bool loadFromText(TTF_Font*& font, const string &text, SDL_Renderer*& tetrisRenderer);
    void eventHandler(SDL_Event& tetrisEvent);
    void move();
    void renderTexture(SDL_Renderer*& tetrisRenderer, SDL_Rect* clipRect);
private:
    SDL_Texture* mTexture;
    int tetrisTextureWidth;
    int tetrisTextureHeight;
    int velY;
};
bool initSDL(SDL_Window*& tetrisWindow, SDL_Renderer*& tetrisRenderer);
void closeSDL(SDL_Window*& tetrisWindow, SDL_Renderer*& tetrisRenderer, tetrisTexture* tetrisSpriteSheet, TTF_Font*& tetrisFont);
bool loadMedia(tetrisTexture* tetrisSpriteSheet, SDL_Renderer*& tetrisRenderer, TTF_Font*& tetrisFont);
void setGravity(tetrisTexture* tetrisSpriteSheet, tetrisTimer& timer);
enum tetrisTextureFlags{
    TETRIS_BACKGROUND_TEXTURE = 0,
    TETRIS_SPRITE_SCORE_TEXT = 1,
    TETRIS_SPRITE_LEVEL_TEXT = 2,
    TETRIS_SPRITE_LINES_TEXT = 3,
    TETRIS_SPRITE_TIME_TEXT = 4,
    TETRIS_SPRITE_TIMER_TEXT = 5,
    TETRIS_SPRITE_OBJECT_1 = 6,
    TETRIS_TOTAL_IMAGE = 7
};
int main(int argc, char* argv[]){
    SDL_Window* tetrisWindow = nullptr;
    SDL_Renderer* tetrisRenderer = nullptr;
    tetrisTexture tetrisSpriteSheet[TETRIS_TOTAL_IMAGE];
    TTF_Font* tetrisFont = nullptr;
    tetrisTimer timer;
    if(!initSDL(tetrisWindow, tetrisRenderer)){
        cout << "Failed to Init SDL | " << SDL_GetError() << endl;
    } else {
        if(!loadMedia(tetrisSpriteSheet, tetrisRenderer, tetrisFont)){
            cout << "Load Tetris Media Failed" << endl;
        }
        timer.tetrisTimerStart();
        bool quit = false;
        while(!quit){
            SDL_Event tetrisEvent;
            while(SDL_PollEvent(&tetrisEvent) != 0){
                if(tetrisEvent.type == SDL_QUIT){
                    quit = true;
                }
                tetrisSpriteSheet[TETRIS_SPRITE_OBJECT_1].eventHandler(tetrisEvent);
            }
            SDL_RenderClear(tetrisRenderer);
            tetrisSpriteSheet[TETRIS_BACKGROUND_TEXTURE].renderTexture(tetrisRenderer, nullptr);
            tetrisSpriteSheet[TETRIS_SPRITE_SCORE_TEXT].renderTexture(tetrisRenderer, nullptr);
            tetrisSpriteSheet[TETRIS_SPRITE_LEVEL_TEXT].renderTexture(tetrisRenderer, nullptr);
            tetrisSpriteSheet[TETRIS_SPRITE_LINES_TEXT].renderTexture(tetrisRenderer, nullptr);
            setGravity(tetrisSpriteSheet, timer);
            tetrisSpriteSheet[TETRIS_SPRITE_OBJECT_1].renderTexture(tetrisRenderer, nullptr);
            tetrisSpriteSheet[TETRIS_SPRITE_OBJECT_1].move();
            SDL_RenderPresent(tetrisRenderer);
            SDL_Delay(20);
        }
    }
    closeSDL(tetrisWindow, tetrisRenderer, tetrisSpriteSheet, tetrisFont);
    return 0;
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
    if(!tetrisSpriteSheet[TETRIS_SPRITE_SCORE_TEXT].loadFromText(tetrisFont, "SCORE", tetrisRenderer)){
        cout << "Load Tetris Score Box Failed" << endl;
        success = false;
    } else {
        tetrisSpriteSheet[TETRIS_SPRITE_SCORE_TEXT].spritePos.x = 570;
        tetrisSpriteSheet[TETRIS_SPRITE_SCORE_TEXT].spritePos.y = 360;
    }
    if(!tetrisSpriteSheet[TETRIS_SPRITE_LEVEL_TEXT].loadFromText(tetrisFont,"LEVEL",tetrisRenderer)){
        cout << "Load Tetris Level Box Failed" << endl;
        success = false;
    } else {
        tetrisSpriteSheet[TETRIS_SPRITE_LEVEL_TEXT].spritePos.x = 570;
        tetrisSpriteSheet[TETRIS_SPRITE_LEVEL_TEXT].spritePos.y = 480;
    }
    if(!tetrisSpriteSheet[TETRIS_SPRITE_LINES_TEXT].loadFromText(tetrisFont, "LINES", tetrisRenderer)){
        cout << "Load Tetris Lines Box Failed" << endl;
        success = false;
    } else {
        tetrisSpriteSheet[TETRIS_SPRITE_LINES_TEXT].spritePos.x = 570;
        tetrisSpriteSheet[TETRIS_SPRITE_LINES_TEXT].spritePos.y = 600;
    }
    if(!tetrisSpriteSheet[TETRIS_SPRITE_TIME_TEXT].loadFromText(tetrisFont, "TIME", tetrisRenderer)){
        cout << "Load Tetris Time Box Failed" << endl;
        success = false;
    }
    if(!tetrisSpriteSheet[TETRIS_SPRITE_OBJECT_1].loadFromFile("textures/spriteObject1.png", tetrisRenderer)){
        cout << "Load Tetris Object 1 Failed" << endl;
        success = false;
    } else {
        tetrisSpriteSheet[TETRIS_SPRITE_OBJECT_1].spritePos.x = BLOCK_SIZE*5;
        tetrisSpriteSheet[TETRIS_SPRITE_OBJECT_1].spritePos.y = 0;
    }
    return success;
}

void setGravity(tetrisTexture* tetrisSpriteSheet, tetrisTimer& timer){
    for(int i = TETRIS_SPRITE_OBJECT_1; i < TETRIS_TOTAL_IMAGE; i++){
        static int check = timer.tetrisTimerGetTicks();
        if(timer.tetrisTimerGetTicks() - (check) >= 1000)
        {
            tetrisSpriteSheet[i].spritePos.y += SPRITE_VEL;
            check = ( timer.tetrisTimerGetTicks()/1000 ) * 1000;
        }
    }
}

void tetrisTexture::eventHandler(SDL_Event& tetrisEvent){
    if( (tetrisEvent.type == SDL_KEYDOWN) && (tetrisEvent.key.repeat == 0) ){
        switch(tetrisEvent.key.keysym.sym){
            case SDLK_a:
                spritePos.x -= SPRITE_VEL;
            break;
            case SDLK_d:
                spritePos.x += SPRITE_VEL;
            break;
            case SDLK_s:
                velY += SPRITE_VEL;
            break;
        }
    } else if( (tetrisEvent.type == SDL_KEYUP) && (tetrisEvent.key.repeat == 0) ){
            if(tetrisEvent.key.keysym.sym == SDLK_s){
                velY -= SPRITE_VEL;
            }
    }
}

void tetrisTexture::move(){
        spritePos.y += velY;
        if( (spritePos.y + tetrisTextureHeight) >= BLOCK_SIZE*18){
        spritePos.y -+ velY;
        }
}

tetrisTexture::tetrisTexture(){
    mTexture = nullptr;
    tetrisTextureWidth = 0;
    tetrisTextureHeight = 0;
    spritePos.x = 0;
    spritePos.y = 0;
    velY = 0;
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

void tetrisTexture::renderTexture(SDL_Renderer*& tetrisRenderer, SDL_Rect* clipRect){
    SDL_Rect dstRect;
    dstRect.w = tetrisTextureWidth;
    dstRect.h = tetrisTextureHeight;
    dstRect.x = spritePos.x;
    dstRect.y = spritePos.y;
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
