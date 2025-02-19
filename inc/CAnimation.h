#pragma once
#include <SDL_hints.h>


class CAnimation
{
public:
    CAnimation();

    void OnAnimate();

    void SetFrameRate(int Rate);

    void SetCurrentFrame(int Frame);

    int GetCurrentFrame();

    int MaxFrames;

    bool Oscillate;

private:
	int CurrentFrame;

	int FrameInc;

    int FrameRate; // Milliseconds

	int OldTime;
};
