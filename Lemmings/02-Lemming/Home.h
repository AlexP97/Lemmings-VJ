#ifndef _HOME_INCLUDE
#define _HOME_INCLUDE


#include <glm/glm.hpp>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"


// Home contains all the entities of our home screen.
// It is responsible for updating and render them.


class Home
{

public:
	Home();
	~Home();

	void init();
	void update(int deltaTime);
	void render();

private:
	void initShaders();

private:
	Texture colorTexture;
	VariableTexture maskTexture;
	MaskedTexturedQuad *map;
	ShaderProgram simpleTexProgram, maskedTexProgram;
	float currentTime;
	glm::mat4 projection;
	
};


#endif // _HOME_INCLUDE
