#include "Board.h"
#include "Assets.h"
#include "Define.h"
#include "Surface.h"

#include <iostream>
#include <string>

Board Board::instance;

Board::Board()
: mTileAsset{ "" }
, mHighlightX{ 0 }
, mHighlightY{ 0 }
, mHighlightSurf{ NULL }
, mIsHighlightVisible{ true }
{
}

Board::~Board()
{
    cleanup();
}

bool Board::load(Assets& assets)
{
    mTileAsset.assign(assets.getTileAsset());
    std::string highlightFile = assets.getHighlightAsset();
    std::pair<int, int> tileSize = assets.getTileSize();

    mWidth = tileSize.first;
    mHeight = tileSize.second;

    if ((mSurface = Surface::OnLoad(mTileAsset)) == NULL)
    {
        std::cout << "ERROR: could not create mSurface: " << SDL_GetError() << std::endl;
        return false;
    }

    if ((mHighlightSurf = Surface::OnLoad(highlightFile)) == NULL)
    {
        std::cout << "ERROR: could not create mHighlightSurf: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void Board::render(SDL_Surface* Surf_Display)
{
    if (Surf_Display == NULL || mSurface == NULL)
        return;

    for (int x = 0; x < mGridRowSize; ++x)
    {
        for (int y = 0; y < mGridColumnSize; ++y)
        {
            int xPos = x * mWidth;
            int yPos = y * mHeight;

            if (mIsHighlightVisible && ((x == mHighlightX) && (y == mHighlightY)))
            {
                Surface::OnDraw(Surf_Display, mHighlightSurf, mX + xPos, mY + yPos, 0, 0, mWidth, mHeight);
            }
            else
            {
                Surface::OnDraw(Surf_Display, mSurface, mX + xPos, mY + yPos, 0, 0, mWidth, mHeight);
            }
        }
    }
}

void Board::cleanup()
{
    if (mSurface)
    {
        SDL_FreeSurface(mSurface);
    }

    if (mHighlightSurf)
    {
        SDL_FreeSurface(mHighlightSurf);
    }

    mSurface = NULL;
    mHighlightSurf = NULL;
}

Pair Board::getBoardSize()
{
    Pair size;
    size.first = mGridRowSize;
    size.second = mGridColumnSize;
    return size;
}