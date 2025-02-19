#pragma once

#include "Assets.h"

#include <SDL.h>
#include <SDL_mixer.h>
#include <map>
#include <string>

class Sounds
{
public:
    static Sounds instance;

    explicit Sounds();

    ~Sounds();

    bool load(Assets& assets);

    void cleanup();

    void play(std::string ID);

    static void stop();

    void toggleMute();

private:
    std::map<std::string, Mix_Chunk*> mSounds;
    bool mIsMute;
};
