#include "stdafx.h"
#include "CInvader.h"


CInvader::CInvader() {
	_position = cv::Point2f(100, 50);
	_shape = cv::Rect(_position.x - 20, _position.y, 40, 40);
	_lives = 1;
	
	
	
}