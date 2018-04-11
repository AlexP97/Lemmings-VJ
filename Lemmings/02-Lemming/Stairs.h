#ifndef _STAIRS_INCLUDE
#define _STAIRS_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"



class Stairs
{

public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	void displace(float d);
	glm::vec2 getPosition();

private:

	Texture spritesheet;
	Sprite *sprite;

};


#endif // _STAIRS_INCLUDE