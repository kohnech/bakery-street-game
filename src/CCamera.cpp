#include "CCamera.h"

CCamera CCamera::CameraControl;

CCamera::CCamera()
{
    X = Y = 0;

    TargetX = TargetY = nullptr;

    TargetMode = TARGET_MODE_NORMAL;
}

void CCamera::OnMove(int MoveX, int MoveY)
{
    X += MoveX;
    Y += MoveY;
}

int CCamera::GetX()
{
    if (TargetX != nullptr)
    {
        if (TargetMode == TARGET_MODE_CENTER)
        {
            return *TargetX - (SCREEN_WIDTH / 2);
        }

        return *TargetX;
    }

    return X;
}

int CCamera::GetY()
{
    if (TargetY != nullptr)
    {
        if (TargetMode == TARGET_MODE_CENTER)
        {
            return *TargetY - (SCREEN_HEIGHT / 2);
        }

        return *TargetY;
    }

    return Y;
}

void CCamera::SetPos(int X, int Y)
{
    this->X = X;
    this->Y = Y;
}

void CCamera::SetTarget(int* X, int* Y)
{
    TargetX = X;
    TargetY = Y;
}
