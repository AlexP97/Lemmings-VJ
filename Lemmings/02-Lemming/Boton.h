#ifndef _BOTON_INCLUDE
#define _BOTON_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"



class Boton
{

public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

private:
	enum BotonState
	{
		OPENING
	};

	BotonState state;
	Texture spritesheet;
	Sprite *sprite;

};


#endif // _BOTON_INCLUDE
