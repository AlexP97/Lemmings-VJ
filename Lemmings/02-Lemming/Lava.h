#ifndef _LAVA_INCLUDE
#define _LAVA_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"



class Lava
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


#endif // _LAVA_INCLUDE