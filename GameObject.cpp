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
    
        if (_Invader_image.empty()) return;
    _Invader_image.copyTo(im(_shape));
}
void CGameObject::move(cv:: Point2f _velocity) {
    _position.x += 2*_velocity.x;
    _position.y += 2*_velocity.y;
}
bool CGameObject::collide(CGameObject& obj)
{
    // Create rectangle for this object
    cv::Rect r1(
        _position.x,
        _position.y,
        _shape.width,
        _shape.height
    );

    // Create rectangle for other object
    cv::Rect r2(
        obj._position.x,
        obj._position.y,
        obj._shape.width,
        obj._shape.height
    );

    // Check intersection
    return (r1 & r2).area() > 0;
}
void CGameObject::hit()
{
    _lives -= 1;
}
void CGameObject::set_shape() {
    if (_Invader_image.empty()) return;
   _shape = cv::Rect(_position.x, _position.y, _Invader_image.cols, _Invader_image.rows);
}
