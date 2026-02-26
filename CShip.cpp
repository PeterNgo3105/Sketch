#include "stdafx.h"
#include "CShip.h"

CShip::CShip() {
	_shape = cv::Rect(_position.x - _ship_size / 2, _position.y - 5, _ship_size, 5);
	_lives = 3;
}