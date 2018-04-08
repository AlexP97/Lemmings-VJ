#ifndef _SALIDA_INCLUDE
#define _SALIDA_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"



class Salida
{

public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, int escena);
	void update(int deltaTime);
	void render();
	void displace(float d);
	glm::vec2 position();

private:

	Texture spritesheet;
	Sprite *sprite;

};


#endif // _SALIDA_INCLUDE
