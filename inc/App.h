#pragma once

#include "Button.h"
#include "CAnimation.h"
#include "CCamera.h"
#include "CountDown.h"
#include "Entity.h"
#include "Event.h"
#include "IThread.h"
#include "Text.h"

#include <SDL.h>

class App : public Event, public IThread
{
public:
    App();
    ~App();

    bool onInit();

    void onEvent(SDL_Event* Event);

    bool onLoop();

    void onRender();

    void onCleanup();


    /// Events
    void onExit();

    void onResize(int w, int h);

    void onKeyDown(SDL_Keycode sym, Uint16 mod, SDL_Scancode unicode);

    virtual bool ThreadMethod();

private:
    bool mIsRunning;

    SDL_Window* mWindow;
    SDL_Surface* Surf_Display;
    SDL_Surface* Yoshi_Surf;

    CAnimation Anim_Yoshi;

    Entity Entity1;
    Entity Entity2;

    SDL_Surface* Background_Surf;
    SDL_Surface* GameOver_Surf;

    std::vector<Entity*> EntityList;

    int mWidth;
    int mHeight;

    Button mMuteButton;

    bool mEnableYoshi;

    CountDown mCountDown;
    bool mIsGameOver;

    void gameOver();
};
