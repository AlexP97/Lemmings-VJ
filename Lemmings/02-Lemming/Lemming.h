#ifndef _LEMMING_INCLUDE
#define _LEMMING_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"


// Lemming is basically a Sprite that represents one lemming. As such it has
// all properties it needs to track its movement and collisions.


class Lemming
{

public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	glm::vec2 position();
	bool eliminar();
	void setComeOut(bool b);
	
	void setMapMask(VariableTexture *mapMask);
	
private:
	int collisionFloor(int maxFall);
	bool collision();
	
private:
	enum LemmingState
	{
		WALKING_LEFT_STATE, WALKING_RIGHT_STATE, FALLING_LEFT_STATE, FALLING_RIGHT_STATE, COMING_OUT_STATE
	};

	LemmingState state;
	Texture spritesheet;
	Sprite *sprite;
	VariableTexture *mask;
	bool come_Out;

};


#endif // _LEMMING_INCLUDE


