#ifndef _PUERTA_INCLUDE
#define _PUERTA_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"



class Puerta
{

public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

private:
	enum PuertaState
	{
		OPENING
	};

	PuertaState state;
	Texture spritesheet;
	Sprite *sprite;

};


#endif // _PUERTA_INCLUDE
