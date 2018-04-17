#ifndef _INSTRUCCIONS_INCLUDE
#define _INSTRUCCIONS_INCLUDE


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
#include "InstPage.h"
#include "ButtonInst.h"


// Instruccions contains all the entities of our game.
// It is responsible for updating and render them.


class Instruccions
{

public:
	Instruccions();
	~Instruccions();

	void init();
	bool update(int deltaTime);
	void render();
	void keyPressed(int key);

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
	ButtonInst buttonInst;
	InstPage instPage;
};


#endif // _INSTRUCCIONS_INCLUDE