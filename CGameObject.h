#pragma once
#include "stdafx.h"

class CGameObject {
protected:
	cv::Point2f _position;
	cv::Point2f _velocity;
	cv::Rect _shape;
	int _lives;
	int _ship_size;
public:
	void move(cv:: Point2f _velocity  );
	bool collide(CGameObject& obj);
	//bool collide_wall(cv::Size board);
	void hit();
	int get_lives() { return _lives; }
	void set_lives(int lives) { _lives = lives; }
	void set_pos(cv::Point2f pos) { _position = pos; }
	cv::Point2f get_pos() { return _position; }
	cv::Rect get_shape() const { return _shape; }
	void draw(cv::Mat& im);
};
