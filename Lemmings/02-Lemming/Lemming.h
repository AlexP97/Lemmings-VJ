#ifndef _LEMMING_INCLUDE
#define _LEMMING_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"
#include "Stairs.h"


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
	void displace(float d);
	void setPosition(glm::vec2 position);
	bool floats();
	bool moveFloatingLemming(int mouseX, int mouseY);
	bool isBashing(int position);
	void setMapMask(VariableTexture *mapMask, VariableTexture* lemmingMask);
	void setStairs(vector<pair<Stairs, bool>> stairs, int n);
	void setAbility(int ability);
	bool goOut();
	pair<bool, int> putStair();
	
private:
	int collisionFloor(int maxFall);
	bool collision();
	bool collisionBasher();
	int collisionStairRight();
	int collisionStairLeft();
	bool collisionTop();
	bool hayParado();
	void eraseMask(int posX, int posY, float ymin, float ymax, float xmin, float xmax, VariableTexture *mascara);
	void applyMask();
	
	
private:
	enum LemmingState
	{
		WALKING_LEFT_STATE, WALKING_RIGHT_STATE, FALLING_LEFT_STATE, FALLING_RIGHT_STATE, COMING_OUT_STATE, DIG_STATE, 
		BLOCKING_STATE, DIG_LEFT_STATE, DIG_RIGHT_STATE, EXPLODE_STATE, CLIMBING_LEFT_STATE, CLIMBING_RIGHT_STATE,
		STOPPING_CLIMB_LEFT_STATE, STOPPING_CLIMB_RIGHT_STATE, BUILDING_LEFT_STATE, BUILDING_RIGHT_STATE, FLOATING_RIGHT_STATE,
		FLOATING_LEFT_STATE
	};

	LemmingState state;
	Texture spritesheet;
	Sprite *sprite;
	VariableTexture *mask;
	VariableTexture *parados;
	vector<pair<Stairs, bool>> stairsPos;
	int nStairs;
	bool come_Out;
	bool eliminado;
	bool climber;
	bool primeraPasada;
	int numberOfStairs;
	int displacement;
	bool floating;
	int currentFloating;
};


#endif // _LEMMING_INCLUDE


