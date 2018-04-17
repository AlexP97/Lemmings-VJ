#ifndef _BUTTONINST_INCLUDE
#define _BUTTONINST_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"



class ButtonInst
{

public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();

private:

	Texture spritesheet;
	Sprite *sprite;

};


#endif // _BUTTONINST_INCLUDE
