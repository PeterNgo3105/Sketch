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

	/** @brief This function is to initialize the canvas and open comport
	*
	* @param Canvas_size is width and height of screen
	* @param comport is port number of micro controller
	*
	* @return nothing
	* 
	*/
	CSketch(cv::Size Canvas_size, int comport);
	~CSketch();
	/** @brief This function is to calculate position and range of joytick.
	*
	* @return nothing
	*
	*/
	virtual void update();
	/** @brief This function is to draw lines to canvas 
	*
	* @return nothing
	*
	*/
	virtual void draw();
	/** @brief This function is to run game sketch
	*
	* @return nothing
	*
	*/
	void run();
	/** @brief This function is to send data and get data to and from micro controller
	*
	* @return nothing
	*
	*/
	void GPIO();
	
private:
	CControl control;	///< this variable to get access to CControl class
	int _x_cursor;		///< joystick x direction
	int _y_cursor;		///< joystick y direction
	int _color = 0;		///< color change variable
	bool _reset = false;///< clear the draw variable
	int _x_percent;		///< joystick x direction in percentage
	int _y_percent;		///< joystick y direction in percentage
	cv::Point _point1;	///< first point of line sketch
	cv::Point _point2 = cv::Point(_Canvas.size().width/2, _Canvas.size().height/2); ///< second point of line sketch
};
