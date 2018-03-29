#ifndef _SCENE_INCLUDE
#define _SCENE_INCLUDE


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


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	void update(int deltaTime);
	void render();
	
	bool mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton);

private:
	void initShaders();
	void eraseMask(int mouseX, int mouseY);
	void applyMask(int mouseX, int mouseY);
	bool cursorOnLemming(int mouseX, int mouseY);
	bool clickOnPause(int mouseX, int mouseY);
	bool lemmingOnExit(glm::vec2 position);
	int clickOnAbility(int mouseX, int mouseY);

private:
	Texture colorTexture;
	VariableTexture maskTexture;
	MaskedTexturedQuad *map;
	ShaderProgram simpleTexProgram, maskedTexProgram;
	float currentTime;
	glm::mat4 projection;
	vector<Lemming> lemming;
	vector<bool> lemmingInit;
	Puerta puerta;
	Cursor cursor;
	Boton botonPlay;
	Salida salida;
	IconPanel panel;
	IconSelected iconSelected;
	int lemmingsIn;
};


#endif // _SCENE_INCLUDE

