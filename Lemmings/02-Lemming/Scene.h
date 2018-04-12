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
#include "Stairs.h"
#include "Minimap.h"
#include "MinimapRectangle.h"
#include "Text.h"


// Scene contains all the entities of our game.
// It is responsible for updating and render them.


class Scene
{

public:
	Scene();
	~Scene();

	void init();
	bool update(int deltaTime);
	void render();
	
	pair<bool, bool> mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton, bool paused);

private:
	void initShaders();
	void changeDisplacement(float d);
	void eraseMask(int mouseX, int mouseY);
	void applyMask(int mouseX, int mouseY);
	pair<bool,int> cursorOnLemming(int mouseX, int mouseY);
	void clickOnLemming(int indLemming);
	bool clickOnPause(int mouseX, int mouseY);
	bool clickOnSpeed(int mouseX, int mouseY);
	bool lemmingOnExit(glm::vec2 position);
	void clickOnAbility(int mouseX, int mouseY);
	void explode();
	pair<bool, bool> mouseOnBorder(int mouseX, int mouseY);

private:
	Texture colorTexture;
	VariableTexture maskTexture, parados;
	MaskedTexturedQuad *map;
	ShaderProgram simpleTexProgram, maskedTexProgram;
	float currentTime;
	glm::mat4 projection;
	vector<Lemming> lemming;
	vector<bool> lemmingInit;
	Puerta puerta;
	Cursor cursor;
	Boton botonPlay;
	Boton botonSpeed;
	Salida salida;
	IconPanel panel;
	IconSelected iconSelected;
	vector<pair<Stairs, bool>> stairs;
	Minimap minimap;
	MinimapRectangle mRectangle1, mRectangle2;
	Text text;
	int lemmingsIn;
	int lemmingsOut;
	int time;
	int ability;
	bool abre_Puerta;
	bool stop_Lemmings;
	float displacement;
};


#endif // _SCENE_INCLUDE

