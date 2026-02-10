#pragma once
#include "CControl.h"
#include "stdafx.h"

/** @brief This class is base class for lab 4 to lab 9
*
*
* @Author Peter Ngo
*/
class CBase4618 {
protected:
	CControl _Control;	///< this variable to set and get data to and from controller
	cv::Mat _Canvas;	///< this variable to set up the screen for games
};

