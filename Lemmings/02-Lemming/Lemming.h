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
	void putAbility(int ability);
	
private:
	int collisionFloor(int maxFall);
	bool collision();
	void eraseMask(int posX, int posY, float ymin, float ymax, float xmin, float xmax);
	void applyMask();
	
	
private:
	enum LemmingState
	{
		WALKING_LEFT_STATE, WALKING_RIGHT_STATE, FALLING_LEFT_STATE, FALLING_RIGHT_STATE, COMING_OUT_STATE, DIG_STATE, BLOCKING_STATE, DIG_LEFT_STATE, DIG_RIGHT_STATE
	};

	LemmingState state;
	Texture spritesheet;
	Sprite *sprite;
	VariableTexture *mask;
	bool come_Out;

};


#endif // _LEMMING_INCLUDE


