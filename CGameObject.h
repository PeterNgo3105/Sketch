#pragma once
#include "stdafx.h"

/**
* @brief This class is holding objects for CShip, Invader and Missiles
*
* holing functions variables for objects that can access from the game
*
* @Author Peter Ngo
*/
class CGameObject {
protected:

	cv::Point2f _position;	///< position of the objects
	cv::Point2f _velocity;	///< speed of objects 
	cv::Rect _shape;		///< shape of objects
	int _ship_size;			///< size of the objects
	int _lives;				///< live of objects
	cv::Mat _Invader_image; ///< Invader image
public:
	/**
	* @brief this function is to move objects in game
	*
	* @param _velocisty is speed of object can move
	*
	*
	* @return nothing
	*
	*/
	void move(cv:: Point2f _velocity);
	/**
	* @brief this function is to check if there is any collision between objects
	*
	* @param obj is the object that another object checking with
	*
	*
	* @return true or false
	*
	*/
	bool collide(CGameObject& obj);
	/**
	* @brief this function is to remove object lives
	*
	*
	*
	* @return nothing
	*
	*/
	void hit();
	/**
	* @brief this function is to get object lives
	*
	* @return lives
	*
	*/
	int get_lives() { return _lives; }
	/**
	* @brief this function is to set lives to objects
	* @param live is number of live that set to objects
	*
	*
	* @return _live to objects
	*
	*/
	void set_lives(int lives) { _lives = lives; }
	/**
	* @brief this function is to set posistion to objects
	* @param pos is position of object
	*
	*
	* @return _position of object
	*
	*/
	void set_pos(cv::Point2f pos) { _position = pos; }
	/**
	* @brief this function is to check where objects are
	*
	*
	*
	* @return position
	*
	*/
	cv::Point2f get_pos() { return _position; }
	
	//cv::Rect get_shape() const { return _shape; }
	/**
	* @brief this function is to draw object in Canvas
	*
	* @param im is Canvas to draw in
	*
	* @return nothing
	*
	*/
	void draw(cv::Mat& im);
	/**
	* @brief this function is to get invader image 
	*
	*
	* @return Invader image
	*
	*/
	void set_image(cv:: Mat& image) { _Invader_image = image; }
	void set_shape();
	//_shape = cv :: Rect (_position.x, _position.y, _Invader_image.cols, _Invader_image.rows);

};
