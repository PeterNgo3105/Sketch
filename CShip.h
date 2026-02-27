#pragma once
#include "CGameObject.h"
/**
* @brief The ship inherits from the CGameObject and has only constructor.
* The ships unique attributes, such as size and number of lives can be initialized in the constructor. The
* position of the ship is controlled by the joystick (using the set_pos) method
* 
* @auther Peter Ngo
* 
*/
class CShip : public CGameObject {
public:
/**
* @brief this contructor to initialize ship.
* The position of the ship is controlled by the joystick (using the set_pos) method
*
* @ return nothing
*/
	CShip();
};