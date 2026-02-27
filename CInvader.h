#pragma once
#include "CGameObject.h"

/**
* @brief The invaders inherits from the CGameObject and has only constructor.
* As with the ship, the constructor can be used to initialize the size and initial position at the top
* of the screen
*
* @auther Peter Ngo
*
*/
class CInvader : public CGameObject 
{
public:
/**
* @brief  the constructor can be used to initialize the size and initial position at the top
* of the screen
*
*
*/
	CInvader();
};
