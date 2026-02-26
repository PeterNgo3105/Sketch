#pragma once
#include "CGameObject.h"
/*The invaders inherits from the CGameObject and require NO additional members other than the
constructor. As with the ship, the constructor can be used to initialize the size and initial position at the top
of the screen*/
class CInvader : public CGameObject 
{
public:
	CInvader();
};
