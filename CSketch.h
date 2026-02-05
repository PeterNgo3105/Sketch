#pragma once
#include <conio.h>

#include "CBase4618.h"
#include "stdafx.h"

/** @brief This class is a child of CBase4618 class. The class will support to sketch and draw on Canva
*
*
* @Author Peter Ngo
*/
class CSketch : CBase4618 {
public:
	CSketch();
	CSketch(cv::Size Canva_size, int comport);
	~CSketch();
	virtual void update();
	virtual void draw();
	void run();
	
private:
	/**
	*@param this variable to get access to CControl class
	*
	*/
	CControl control;
	int _x_cursor;
	int _y_cursor;
	int _color = 0;
	bool _reset = false;
};
