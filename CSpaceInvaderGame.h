#pragma once
#include "CBase4618.h"

struct Enemies {
	cv::Point position;
	int speed;
	bool alive;
};
struct missile {
	cv::Point position;
	int speed =5;
	bool alive ;
};
/** @brief This class is a child of CBase4618 class. The class will support to sketch and draw on Canva
*
*
* @Author Peter Ngo
*/
class CSpaceInvaderGame : CBase4618 {
private:
	CControl control;					///< this variable to get access to CControl class
	int _point_horizontal_mid;			///< position of ship 
	int _ship_position_percentage;		///< joystick x direction in percentage
	int _pointHorizontal_mid_position;	///< mid point of ship
	bool _running = true;				///< while loop variable	
	double _FPS;						///< measuring game speed
	bool _reset = false;				///< clear the game and reset game back to zero
	bool _thread_exit = false;			///< stop all thread when it is true
	cv::Rect _player_Ship;				///< object as play firing ship
	cv::Rect _gun;						///< ship gun 
	cv::Rect _enemies;					///< enermy ships
	std::vector<Enemies>  enemies;		///< vector enermies spawn
	std::vector<missile> Missile;			///< vector missle
	/** @brief This function is to calculate position and range of joytick.
	*
	* @return nothing
	*
	*/
	virtual void update();
	/** @brief This function is to create invaders and missel and game information
	*
	* @return nothing
	*
	*/
	virtual void draw();
	/** @brief This function is to send data and get data to and from micro controller
	*
	* @return nothing
	*
	*/
	void GPIO();
	/** @brief This function is to run update function seperately
	*
	* @return nothing
	*
	*/
	//static void update_thread(CSpaceInvaderGame* ptr);
	/** @brief This function is to run draw function seperately
	*
	* @return nothing
	*
	*/
	//static void draw_thread(CSpaceInvaderGame* ptr);
	/** @brief This function is to run GPIO function seperately
	*
	* @return nothing
	*
	*/
	//static void GPIO_thread(CSpaceInvaderGame* ptr);
public:
	CSpaceInvaderGame();
	/** @brief This function is to initialize the canvas and open comport
	*
	* @param Canvas_size is width and height of screen
	* @param comport is port number of micro controller
	*
	* @return nothing
	*
	*/
	CSpaceInvaderGame(cv::Size Canvas_size, int comport);

	/** @brief This function is to run game Pong
	*
	* @return nothing
	*
	*/
	void run();
};