#pragma once
#include "CGameObject.h"
/*The missiles inherits from the CGameObject and require NO additional members other than the constructor.
The constructor can be used to initialize the size, while the position and velocity is set when the player
presses the fire button*/
class CMissile : public CGameObject {
public:
	CMissile();
};