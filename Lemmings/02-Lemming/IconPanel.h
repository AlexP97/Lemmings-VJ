#ifndef _ICONPANEL_INCLUDE
#define _ICONPANEL_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"



class IconPanel
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


#endif // _ICONPANEL_INCLUDE