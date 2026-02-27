#include "stdafx.h"
#include "CInvader.h"


CInvader::CInvader() {
	_shape = cv::Rect(_position.x - 20, _position.y, 40, 40);
	_lives = 1;
	//_Invader_image = cv::imread("images.png", cv::IMREAD_COLOR);
	//cv::resize(_Invader_image, _Invader_image, cv::Size(40, 40));
	//cv::Rect roi(_position.x, _position.y, _Invader_image.cols, _Invader_image.rows);
}