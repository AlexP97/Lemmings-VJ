
#ifndef _INSTPAGE_INCLUDE
#define _INSTPAGE_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"



class InstPage
{

public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	void next();
	void before();

private:

	Texture spritesheet;
	Sprite *sprite;

};


#endif // _INSTPAGE_INCLUDE
