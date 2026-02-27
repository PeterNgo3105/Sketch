#include "stdafx.h"
#include "CMissile.h"

CMissile::CMissile(){
_shape = cv::Rect(_position.x, _position.y, 3, 6);
_lives = 1;
}
