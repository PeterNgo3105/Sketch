#pragma once
#include "serial.h"
/**

*
* @brief This class is meant to sending commands set data to and get data from Comport of micro controller
* This class has a private varialbe " _com" and a public variable "type"
* 
* @Author: Viet Thu Ngo, Student ID: A0103762	Set: S
*
*/
class CControl {
private:
/**
*
* @param _com is communication from Serial to microcontroller
*
*/
	Serial _com;
public:
	enum type {DIGITAL = 0, ANALOG, SEVRO};
	CControl();
	~CControl();
	/**
	*
	* @brief Function init_com is to open the comport
	* 
	* @param comport is port number
	* 
	* @return nothing
	* 
	*/
	void init_com(int comport);
	/**
	* 
	* @brief Function get_dat is to get value back from the controller
	* 
	* @param type is to assign DIGITAL, ANALOG or SERVO.
	* @param channel is to assign pin for microcontroller
	* @param result is to get value back from controller
	*
	* @return true or false
	*/

	bool get_data(int type, int channel, int& result);
	/**
	*	 
	* @brief Function set_data is to send command to controller to do something
	* 
	* @param type is to assign DIGITAL, ANALOG or SERVO.
	* @param channel is to assign pin for microcontroller
	* @param result is to set value to controller
	*
	* @return true or false
	*/
	bool set_data(int type, int channel, int val);
	/**
	*
	* @brief Function get_analog is to convert 12 bits data to percentage
	* 
	* @param value is calculate percentage of joystick position
	*
	* @return float value
	* 
	*/
	float get_analog(float value);
	/**
	*
	* @brief Function get_button is to avoid debouncing when pressing button
	* 
	* @param button is signal back from controler to avoid debouncing
	* 
	* @return true or false
	* 
	*/
	bool get_button(bool button);
};