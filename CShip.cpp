#include "stdafx.h"
#include "CShip.h"

CShip::CShip()
{       
    _ship_size = 60;
    _position.y = 600 - 15;
    _shape = cv::Rect(0, 0, _ship_size, 10);
    _lives = 3;
}