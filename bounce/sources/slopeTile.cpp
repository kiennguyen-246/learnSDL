// slopeTile.cpp

#include "slopeTile.h"

slopeTile::slopeTile()
{
    setSize(SLOPE_TILE_WIDTH, SLOPE_TILE_HEIGHT);
    slopeType = 0;
    mEquationA = mEquationB = 0;
}

slopeTile::slopeTile(const int& __PosX, const int& __PosY, const int& __slopeType)
{
    mPosX = __PosX;
    mPosY = __PosY;
    slopeType = __slopeType;
    mEquationA = (slopeType % 2 ? 1 : -1);
    if (slopeType == 2 || slopeType == 4) mEquationB = mPosY - mEquationA * mPosX;
    else mEquationB = (mPosY) - mEquationA * (mPosX + mWidth);
    setSize(SLOPE_TILE_WIDTH, SLOPE_TILE_HEIGHT);
}

int slopeTile::getSlopeType() const
{
    return slopeType;
}

void slopeTile::setPos(const int& x, const int& y)
{
    mPosX = x;
    mPosY = y;
    mEquationA = (slopeType % 2 ? 1 : -1);
    mEquationB = y - mEquationA * x;
}

std::pair <double, double> slopeTile::getEquation() const
{
    return std::make_pair(mEquationA, mEquationB);
}

double slopeTile::distanceFromCenter(const ball& __ball) const
{
    auto ballCenterPoint = __ball.getBallCenter();
    double distance = abs(ballCenterPoint.first * mEquationA - ballCenterPoint.second + mEquationB);
    distance /= sqrt(2);
    return distance;
}

bool slopeTile::checkBallIsOnSlope(const ball& __ball) const
{
    //The ball is on the slope if d(O, slopeEquation) = radius and cos(vOA, vBA) < 1 and cos(vOB, vAB) < 1
    auto O = __ball.getBallCenter();
    std::pair <double, double> A = {mPosX, mPosY};
    std::pair <double, double> B = {mPosX + mWidth * (slopeType == 2 || slopeType == 3 ? 1 : -1), 
                                    mPosY - mHeight * (slopeType == 1 || slopeType == 2 ? 1 : -1)};
    std::pair <double, double> vAO = {O.first - A.first, O.second - A.second};
    double AO = sqrt(vAO.first * vAO.first + vAO.second * vAO.second);
    std::pair <double, double> vOB = {B.first - O.first, B.second - O.second};
    double OB = sqrt(vOB.first * vOB.first + vOB.second * vOB.second);
    std::pair <double, double> vAB = {B.first - A.first, B.second - A.second};
    double AB = sqrt(vAB.first * vAB.first + vAB.second * vAB.second);

    double cosOAB = (vAO.first * vAB.first + vAO.second * vAB.second) / AO / AB;
    double cosOBA = (vOB.first * vAB.first + vOB.second * vAB.second) / OB / AB;

    return (abs(distanceFromCenter(__ball) - __ball.getRadius()) < 1e-1
            && cosOAB > 0 && cosOBA > 0);
}

bool slopeTile::checkBallIsInsideSlope(const ball& __ball) const
{
    /*The ball is inside the slope if d(O, slopeEquation) < radius and one of these happens
        - cos(vOA, vBA) < 1 and cos(vOB, vAB) < 1
        - OB < radius
        - OA < radius
    */
    auto O = __ball.getBallCenter();
    std::pair <double, double> A, B;
    if (slopeType == 2 || slopeType == 4)
    {
        A = {mPosX, mPosY};
        B = {mPosX + mWidth , mPosY - mHeight};
    }
    else
    {
        A = {mPosX, mPosY - mHeight};
        B = {mPosX + mWidth, mPosY};
    }
    
    std::pair <double, double> vAO = {O.first - A.first, O.second - A.second};
    double AO = sqrt(vAO.first * vAO.first + vAO.second * vAO.second);
    std::pair <double, double> vOB = {B.first - O.first, B.second - O.second};
    double OB = sqrt(vOB.first * vOB.first + vOB.second * vOB.second);
    std::pair <double, double> vAB = {B.first - A.first, B.second - A.second};
    double AB = sqrt(vAB.first * vAB.first + vAB.second * vAB.second);

    double cosOAB = (vAO.first * vAB.first + vAO.second * vAB.second) / AO / AB;
    double cosOBA = (vOB.first * vAB.first + vOB.second * vAB.second) / OB / AB;

    if (distanceFromCenter(__ball) - __ball.getRadius() < -1e-3)
    {
        if ((cosOAB > 0 && cosOBA > 0)) return 1;
        if (AO < __ball.getRadius() || OB < __ball.getRadius()) return 1;
    }
    return 0; 
}

void slopeTile::render(SDL_Renderer* renderer, LTexture& spritesheet)
{
    setSpriteClip(spritesheet, SLOPE_TILE_SPRITE_POS_x[slopeType], SLOPE_TILE_SPRITE_POS_Y[slopeType], SLOPE_TILE_WIDTH / 2, SLOPE_TILE_HEIGHT / 2);
    spritesheet.render(renderer, mPosX, mPosY - SLOPE_TILE_HEIGHT, &mSpriteClip, 2);
}