#ifndef _CURSOR_INCLUDE
#define _CURSOR_INCLUDE


#include "Sprite.h"
#include "VariableTexture.h"



class Cursor
{

public:
	void init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram);
	void update(int deltaTime);
	void render();
	void setPosition(int x, int y);

private:
	enum CursorState
	{
		NORMAL, HOVER
	};

	CursorState state;
	Texture spritesheet;
	Sprite *sprite;

};


#endif // _CURSOR_INCLUDE