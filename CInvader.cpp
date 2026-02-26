#include "stdafx.h"
#include "CInvader.h"


CInvader::CInvader() {
	_shape = cv::Rect(_position.x - 20, _position.y + 2, 40, 5);
	_lives = 1;
}