#ifndef _LEMMING_INCLUDE
#define _LEMMING_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"


// Lemming is basically a Sprite that represents one lemming. As such it has
// all properties it needs to track its movement and collisions.


class Lemming
{

public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, int desplazamiento);
	bool update(int deltaTime);
	void render();
	glm::vec2 position();
	bool eliminar();
	void setComeOut(bool b);
	
	void setMapMask(VariableTexture *mapMask, VariableTexture* lemmingMask);
	void setAbility(int ability);
	bool goOut();
	pair<bool, int> putStair();
	
private:
	int collisionFloor(int maxFall);
	bool collision();
	bool hayParado();
	void eraseMask(int posX, int posY, float ymin, float ymax, float xmin, float xmax, VariableTexture *mascara);
	void applyMask();
	
	
private:
	enum LemmingState
	{
		WALKING_LEFT_STATE, WALKING_RIGHT_STATE, FALLING_LEFT_STATE, FALLING_RIGHT_STATE, COMING_OUT_STATE, DIG_STATE, 
		BLOCKING_STATE, DIG_LEFT_STATE, DIG_RIGHT_STATE, EXPLODE_STATE, CLIMBING_LEFT_STATE, CLIMBING_RIGHT_STATE,
		STOPPING_CLIMB_LEFT_STATE, STOPPING_CLIMB_RIGHT_STATE, BUILDING_LEFT_STATE, BUILDING_RIGHT_STATE
	};

	LemmingState state;
	Texture spritesheet;
	Sprite *sprite;
	VariableTexture *mask;
	VariableTexture *parados;
	bool come_Out;
	bool eliminado;
	bool climber;
	bool primeraPasada;
	int numberOfStairs;
	int displacement;
};


#endif // _LEMMING_INCLUDE


