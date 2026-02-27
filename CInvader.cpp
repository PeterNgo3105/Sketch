#include "stdafx.h"
#include "CInvader.h"


CInvader::CInvader() {
	_shape = cv::Rect(_position.x - 30, _position.y + 2, 60, 10);
	_lives = 1;
}