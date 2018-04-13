#ifndef _SIMPLESCENE_INCLUDE
#define _SIMPLESCENE_INCLUDE


#include <glm/glm.hpp>
#include <vector>
#include "ShaderProgram.h"
#include "MaskedTexturedQuad.h"
#include "Lemming.h"
#include "Puerta.h"
#include "Salida.h"
#include "Cursor.h"
#include "Boton.h"
#include "IconPanel.h"
#include "IconSelected.h"
#include "Stairs.h"
#include "Minimap.h"
#include "MinimapRectangle.h"
#include "Text.h"


// SimpleScene contains all the entities of our game.
// It is responsible for updating and render them.


class SimpleScene
{

public:
	SimpleScene();
	~SimpleScene();

	void init();
	bool update(int deltaTime);
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
	Minimap minimap;
	Text text;
};


#endif // _SIMPLESCENE_INCLUDE

