#ifndef _PUERTA_INCLUDE
#define _PUERTA_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"



class Puerta
{

public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, int escena);
	void update(int deltaTime);
	void render();
	void open();

private:
	enum PuertaState
	{
		OPENING_STATE, CLOSED_STATE
	};

	PuertaState state;
	Texture spritesheet;
	Sprite *sprite;

};


#endif // _PUERTA_INCLUDE
