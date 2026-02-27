#pragma once
#include "CGameObject.h"
/**
* @brief The ship missiles inherits from the CGameObject and has only constructor.
* The constructor can be used to initialize the size, while the position and velocity is set when the player
* presses the fire button
*
* @auther Peter Ngo
*
*/
class CShipMissile : public CGameObject {
public:
/**
* @brief The constructor is to initialize the size, while the position and velocity is set when the player
* presses the fire button
*
*
*/
	CShipMissile();
};