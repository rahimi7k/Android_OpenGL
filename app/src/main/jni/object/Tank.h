#ifndef GAME_TANK_H
#define GAME_TANK_H


#include "../SimObject3D.h"

class Tank {

private:


public:
	SimObject3D simObject;

	Tank(): simObject(0.7, 0.5f, 1.0f) {


		simObject.sprite3D.loadTexture("image/tank.png", 1152, 409);
		simObject.sprite3D.setAtlasImage(0, 1152, 409, 0, 0, 217, 169);
		simObject.sprite3D.setAtlasImage(1, 1152, 409, 218, 0, 215, 169);
		simObject.sprite3D.setAtlasImage(2, 1152, 409, 440, 0, 153, 402); //Right
		simObject.sprite3D.setAtlasImage(3, 1152, 409, 598, 0, 153, 402); //Left
		simObject.sprite3D.setAtlasImage(4, 1152, 409, 770, 0 , 198, 400);
		//simObject.sprite3D.setAtlasImage(5, 1010, 382,0,0,0,0);




	}

	~Tank() {
	}



};

#endif //GAME_TANK_H
