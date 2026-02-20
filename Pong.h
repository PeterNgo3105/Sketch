#pragma once
#include <conio.h>
#include "stdafx.h"
#include "CBase4618.h"
/** @brief This class 
*
*
* @Author Peter Ngo
*/
class CPong : CBase4618 {
public:
	CPong();
	/** @brief This function is to initialize the canvas and open comport
	*
	* @param Canvas_size is width and height of screen
	* @param comport is port number of micro controller
	*
	* @return nothing
	*
	*/
	CPong(cv::Size Canvas_size, int comport);
	//virtual void update();
	//virtual void draw();
	void run();
	//void GPIO();
	//static void update_thread(CPong* ptr);
	//static void draw_thread(CPong* ptr);
	//static void GPIO_thread(CPong* ptr);

private:
	CControl control;		///< this variable to get access to CControl class
	bool _reset = false;	///< clear the game and reset game back to zero
	int _pointY_dir_percent;///< joystick y direction in percentage
	int _pointY_mid_position = 0;///< mid point of player baddle
	int _start_button = false;	///< game start when this button is pressed
	//int _level_up = 0;		///< speed up when the ball colision with the baddle
	int _pos1;				///< the ball x position
	int _pos2;				///< the ball y position
	cv::Point _ball;		///< pong ball
	cv::Rect _player_baddle;///< rectanle paddle for player
	cv::Rect _bot_baddle;	///< rectanle paddle for computer
	int _botY_mid_paddle_point;	///< middle point opf computer paddle
	int _player_score = 0;	///< tracking score for player
	int _computer_score = 0; ///< tracking score for computer
	int _ball_size = 30;	///< ping pong ball size setup
	int _speed = 400;		///< speed setup
	bool _running = true;	///< while loop variable
	bool _thread_exit = false;		///< stop all thread when it is true
	double _FPS;				///< measuring game speed
	virtual void update();
	virtual void draw();
	void GPIO();
	static void update_thread(CPong* ptr);
	static void draw_thread(CPong* ptr);
	static void GPIO_thread(CPong* ptr);
};