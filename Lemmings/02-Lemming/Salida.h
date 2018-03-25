#ifndef _SALIDA_INCLUDE
#define _SALIDA_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"



class Salida
{

public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	glm::vec2 position();

private:

	Texture spritesheet;
	Sprite *sprite;

};


#endif // _SALIDA_INCLUDE
