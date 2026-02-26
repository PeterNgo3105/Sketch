#include "stdafx.h"
#include "CGameObject.h"


void CGameObject::draw(cv::Mat& im) {
    cv::Rect object(
        _position.x,
        _position.y,
        _shape.width,
        _shape.height
    );
	cv::rectangle(im, object, cv::Scalar(255, 255, 255), 4, 8, 0);
}
void CGameObject::move(int dir) {
    _position.x += 2*dir;
}