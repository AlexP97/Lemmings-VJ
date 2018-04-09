#ifndef _MINIMAP_INCLUDE
#define _MINIMAP_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"



class Minimap
{

public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, int scene);
	void update(int deltaTime);
	void render();

private:

	Texture spritesheet;
	Sprite *sprite;

};


#endif // _MINIMAP_INCLUDE
