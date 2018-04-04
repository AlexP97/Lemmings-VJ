#ifndef _EXPLOSION_INCLUDE
#define _EXPLOSION_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"



class Explosion
{

public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

private:

	Texture spritesheet;
	Sprite *sprite;

};


#endif // _EXPLOSION_INCLUDE
