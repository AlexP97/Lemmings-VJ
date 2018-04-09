#ifndef _MINIMAPRECTANGLE_INCLUDE
#define _MINIMAPRECTANGLE_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"



class MinimapRectangle
{

public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, int scene, int typeRectangle);
	void update(int deltaTime);
	void render();
	void displace(float d);

private:

	Texture spritesheet;
	Sprite *sprite;

};


#endif // _MINIMAPRECTANGLE_INCLUDE