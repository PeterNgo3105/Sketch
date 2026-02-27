#include "stdafx.h"
#include "CShipMissile.h"

CShipMissile::CShipMissile() {
	_shape = cv::Rect(_position.x, _position.y, 3, 6);
	_lives = 1;

}
