#ifndef _ICONSELECTED_INCLUDE
#define _ICONSELECTED_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"

class IconSelected
{

public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	void setPosition(int x, int y);
	void changeState(int state);
	int getState();

private:
	enum IconSelectedState
	{
		NOTICONSELECTED_STATE, ICONSELECTED_STATE
	};

	IconSelectedState state;
	Texture spritesheet;
	Sprite *sprite;

};


#endif // _ICONSELECTED_INCLUDE
