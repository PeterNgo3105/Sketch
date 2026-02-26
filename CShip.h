#pragma once
#include "CGameObject.h"
/*The ship inherits from the CGameObject and require NO additional members other than the constructor.
The ships unique attributes, such as size and number of lives can be initialized in the constructor. The
position of the ship is controlled by the joystick (using the set_pos) method*/
class CShip : public CGameObject {
public:
	CShip();
};