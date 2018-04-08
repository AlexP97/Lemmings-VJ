#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene3.h"
#include <Windows.h>
#include <mmsystem.h>


Scene3::Scene3()
{
	map = NULL;
}

Scene3::~Scene3()
{
	if (map != NULL)
		delete map;
}

void Scene3::init()
{
	currentTime = 0;
	lemmingsIn = 0;
	int ability = 0;
	abre_Puerta = true;
	stop_Lemmings = false;
	//glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(160.f)) };
	glm::vec2 texCoords[2] = { glm::vec2(120.f / 512.0, 0.f), glm::vec2((120.f + 320.f) / 512.0f, 160.f / 256.0f) };

	initShaders();

	map = MaskedTexturedQuad::createTexturedQuad(geom, texCoords, maskedTexProgram);
	colorTexture.loadFromFile("images/taxing5.png", TEXTURE_PIXEL_FORMAT_RGBA);
	colorTexture.setMinFilter(GL_NEAREST);
	colorTexture.setMagFilter(GL_NEAREST);
	maskTexture.loadFromFile("images/taxing5_mask.png", TEXTURE_PIXEL_FORMAT_L);
	maskTexture.setMinFilter(GL_NEAREST);
	maskTexture.setMagFilter(GL_NEAREST);
	parados.loadFromFile("images/taxing5_mask.png", TEXTURE_PIXEL_FORMAT_L);
	parados.setMinFilter(GL_NEAREST);
	parados.setMagFilter(GL_NEAREST);

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);

	puerta.init(glm::vec2(50, 60), simpleTexProgram, 2);
	lava.init(glm::vec2(99, 145), simpleTexProgram);
	botonPlay.init(glm::vec2(300, 185), simpleTexProgram, 0);
	botonSpeed.init(glm::vec2(280, 185), simpleTexProgram, 1);
	cursor.init(glm::vec2(90, 30), simpleTexProgram);
	salida.init(glm::vec2(263, 14), simpleTexProgram, 2);
	panel.init(glm::vec2(10, 159), simpleTexProgram);
	iconSelected.init(glm::vec2(9, 158), simpleTexProgram);
	//iconSelected.init(glm::vec2(60, 30), simpleTexProgram);

	lemmingsIn = 0;

	lemmingInit = vector<bool>(20, false);
	Lemming lem;
	lemming = vector<Lemming>(0);
	for (int i = 0; i < 20; i++) {
		lemming.push_back(lem);
	}
	ability = 0;
	mciSendString(TEXT("play sound/LETSGO.WAV"), NULL, 0, NULL);
}

//unsigned int x = 0; 

bool Scene3::update(int deltaTime)
{
	currentTime += deltaTime;

	if (abre_Puerta && currentTime >= 2000) {
		puerta.open();
		mciSendString(TEXT("play sound/DOOR.WAV"), NULL, 0, NULL);
		abre_Puerta = false;
	}

	if (!stop_Lemmings && lemmingsIn < 8 && currentTime >= (3000 * (lemmingsIn + 1))) {
		lemming[lemmingsIn].init(glm::vec2(60, 60), simpleTexProgram, 120);
		lemming[lemmingsIn].setMapMask(&maskTexture, &parados);
		lemmingInit[lemmingsIn] = true;
		++lemmingsIn;
	}

	for (int i = 0; i < lemming.size(); i++) {
		if (lemmingInit[i]) {
			if (lemmingOnExit(lemming[i].position()) && !lemming[i].goOut()) {
				mciSendString(TEXT("play sound/YIPPEE.WAV"), NULL, 0, NULL);
				lemming[i].setComeOut(true);
			}
			if (lemming[i].eliminar()) {
				lemmingInit[i] = false;
			}
			if (lemmingOnLava(lemming[i].position())) {
				mciSendString(TEXT("play sound/PLOP.WAV"), NULL, 0, NULL);
				lemmingInit[i] = false;
			}
		}
	}

	for (int i = 0; i < lemming.size(); i++) {
		if (lemmingInit[i]) {
			int cont = lemming[i].update(deltaTime);
			if (stop_Lemmings && !cont) return false;
		}
	}

	for (int i = 0; i < lemming.size(); i++) {
		if (lemmingInit[i]) {
			pair<bool, int> putStair = lemming[i].putStair();
			if (putStair.first) {
				Stairs stair;
				glm::vec2 pos = lemming[i].position();
				pos.y += 15;
				if (putStair.second == 0) {		//escalera hacia la izquierda
					pos.x += 3;
					stair.init(pos, simpleTexProgram);
				}
				else if (putStair.second == 1) {	//escalera hacia la derecha
					pos.x += 9;
					stair.init(pos, simpleTexProgram);
				}
				for (int j = 0; j < 4; j++) {
					maskTexture.setPixel(pos.x + j + 120, pos.y, 255);
				}
				for (int j = 0; j < lemming.size(); j++) {
					if (lemmingInit[i]) {
						lemming[i].setMapMask(&maskTexture, &parados);
					}
				}
				stairs.push_back(stair);
			}
		}
	}

	puerta.update(deltaTime);
	lava.update(deltaTime);
	botonPlay.update(deltaTime);
	botonSpeed.update(deltaTime);
	salida.update(deltaTime);
	for (int i = 0; i < stairs.size(); i++) {
		stairs[i].update(deltaTime);
	}
	panel.update(deltaTime);
	if (iconSelected.getState() == 1) iconSelected.update(deltaTime);
	cursor.update(deltaTime);
	return true;
}

void Scene3::render()
{
	glm::mat4 modelview;

	maskedTexProgram.use();
	maskedTexProgram.setUniformMatrix4f("projection", projection);
	maskedTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	maskedTexProgram.setUniformMatrix4f("modelview", modelview);
	map->render(maskedTexProgram, colorTexture, maskTexture);

	simpleTexProgram.use();
	simpleTexProgram.setUniformMatrix4f("projection", projection);
	simpleTexProgram.setUniform4f("color", 1.0f, 1.0f, 1.0f, 1.0f);
	modelview = glm::mat4(1.0f);
	simpleTexProgram.setUniformMatrix4f("modelview", modelview);

	puerta.render();
	salida.render();
	for (int i = 0; i < lemming.size(); i++) {
		if (lemmingInit[i]) lemming[i].render();
	}

	botonPlay.render();
	botonSpeed.render();
	for (int i = 0; i < stairs.size(); i++) {
		stairs[i].render();
	}
	panel.render();
	lava.render();

	if (iconSelected.getState() == 1) iconSelected.render();
	cursor.render();
}

pair<bool, bool> Scene3::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton, bool paused)
{
	pair<bool, bool> ret;
	ret.first = false;
	ret.second = false;
	cursor.setPosition(mouseX, mouseY);
	if (!paused) {
		if (bLeftButton)
			eraseMask(mouseX, mouseY);
		else if (bRightButton)
			applyMask(mouseX, mouseY);

		pair<bool, int> cOL = cursorOnLemming(mouseX, mouseY);
		if (cursor.currentAnimation() == 0) {
			if (cOL.first) cursor.changeAnimation(1);
		}
		else {
			if (!cOL.first) cursor.changeAnimation(0);
		}

		if (bLeftButton) {
			clickOnAbility(mouseX, mouseY);
			if (ability == 7) {
				for (int i = 0; i < lemming.size(); i++) {
					if (lemmingInit[i]) {
						lemming[i].setAbility(ability);
						bool stop_Lemmings = true;
					}
				}
			}
			if (cOL.first && ability != 7) {
				clickOnLemming(cOL.second);
			}
		}
	}

	if (bLeftButton) {
		if (clickOnSpeed(mouseX, mouseY)) ret.first = true;
		if (clickOnPause(mouseX, mouseY)) ret.second = true;
	}
	return ret;
}

void Scene3::clickOnLemming(int indLemming) {

	lemming[indLemming].setAbility(ability);
	if (ability != 0) mciSendString(TEXT("play sound/ACTION.WAV"), NULL, 0, NULL);
}

bool Scene3::clickOnPause(int mouseX, int mouseY) {
	glm::vec2 center = botonPlay.centerPosition();
	int x = mouseX / 3 - 2.f;
	int y = mouseY / 3 - 2.f;

	float distFromCenter = sqrt((x - center.x) * (x - center.x) + (y - center.y) * (y - center.y));
	if (distFromCenter <= 8.f) {
		botonPlay.changeAnimation();
		return true;
	}
	return false;
}

bool Scene3::clickOnSpeed(int mouseX, int mouseY) {
	glm::vec2 center = botonSpeed.centerPosition();
	int x = mouseX / 3 - 2.f;
	int y = mouseY / 3 - 2.f;

	float distFromCenter = sqrt((x - center.x) * (x - center.x) + (y - center.y) * (y - center.y));
	if (distFromCenter <= 8.f) {
		botonSpeed.changeAnimation();
		return true;
	}
	return false;
}

bool Scene3::lemmingOnExit(glm::vec2 position) {
	glm::vec2 salidaPos = salida.position();
	if (position.x > (salidaPos.x + 8.f) && position.x < (salidaPos.x + 14.f)) {
		if (position.y < (salidaPos.y + 30.f) && position.y > (salidaPos.y + 25)) return true;
	}
	return false;
}

bool Scene3::lemmingOnLava(glm::vec2 position) {
	glm::vec2 lavaPos = lava.position();
	if (position.x > (lavaPos.x - 10.f) && position.x < (lavaPos.x + 124.f)) {
		if (position.y < (lavaPos.y + 14.f) && position.y > (lavaPos.y - 10.f)) return true;
	}
	return false;
}

pair<bool, int> Scene3::cursorOnLemming(int mouseX, int mouseY) {
	glm::vec2 position;
	int x = mouseX / 3 - 2.f;
	int y = mouseY / 3 - 2.f;
	pair<bool, int> p;
	p.first = false;
	p.second = -1;
	for (int i = 0; i < lemming.size(); i++) {
		if (lemmingInit[i]) {
			position = lemming[i].position();
			if (x > position.x && (x - 15) < position.x) {
				if (y > position.y && (y - 15) < position.y) {
					p.first = true;
					p.second = i;
					return p;
				}
			}
		}
	}
	return p;
}



void Scene3::clickOnAbility(int mouseX, int mouseY) {
	int x = mouseX / 3;
	int y = mouseY / 3;
	glm::vec2 positionPanel = panel.position();
	float sizeX = 465.f / 3.f;
	float sizeY = 135.f / 3.f;
	float sizeAbility = sizeX / 7.f;

	if (x > (positionPanel.x - 2) && x < (positionPanel.x + sizeX)) {
		if (y > positionPanel.y && y < (positionPanel.y + sizeY)) {
			if (x < (positionPanel.x + sizeAbility - 2)) {
				if (ability == 1) {
					iconSelected.changeState(0);
					ability = 0;
				}
				else {
					iconSelected.changeState(1);
					ability = 1;
				}
				iconSelected.setPosition(positionPanel.x - 1, 158);
			}
			else if (x > (positionPanel.x + sizeAbility - 2) && x < (positionPanel.x + sizeAbility * 2 - 2)) {
				if (ability == 2) {
					iconSelected.changeState(0);
					ability = 0;
				}
				else {
					iconSelected.changeState(1);
					ability = 2;
				}
				iconSelected.setPosition(positionPanel.x + sizeAbility, 158);
			}
			else if (x >(positionPanel.x + sizeAbility * 2 - 2) && x < (positionPanel.x + sizeAbility * 3 - 2)) {
				if (ability == 3) {
					iconSelected.changeState(0);
					ability = 0;
				}
				else {
					iconSelected.changeState(1);
					ability = 3;
				}
				iconSelected.setPosition(positionPanel.x - 1 + sizeAbility * 2, 158);
			}
			else if (x >(positionPanel.x + sizeAbility * 3 - 2) && x < (positionPanel.x + sizeAbility * 4 - 2)) {
				if (ability == 4) {
					iconSelected.changeState(0);
					ability = 0;
				}
				else {
					iconSelected.changeState(1);
					ability = 4;
				}
				iconSelected.setPosition(positionPanel.x - 1 + sizeAbility * 3, 158);
			}
			else if (x >(positionPanel.x + sizeAbility * 4 - 2) && x < (positionPanel.x + sizeAbility * 5 - 2)) {
				if (ability == 5) {
					iconSelected.changeState(0);
					ability = 0;
				}
				else {
					iconSelected.changeState(1);
					ability = 5;
				}
				iconSelected.setPosition(positionPanel.x - 1 + sizeAbility * 4, 158);
			}
			else if (x >(positionPanel.x + sizeAbility * 5 - 2) && x < (positionPanel.x + sizeAbility * 6 - 2)) {
				if (ability == 6) {
					iconSelected.changeState(0);
					ability = 0;
				}
				else {
					iconSelected.changeState(1);
					ability = 6;
				}
				iconSelected.setPosition(positionPanel.x - 1 + sizeAbility * 5, 158);
			}
			else if (x >(positionPanel.x + sizeAbility * 6 - 2) && x < (positionPanel.x + sizeAbility * 7 - 2)) {
				if (ability == 7) {
					iconSelected.changeState(0);
					ability = 0;
				}
				else {
					iconSelected.changeState(1);
					stop_Lemmings = true;
					ability = 7;
				}
				iconSelected.setPosition(positionPanel.x - 1 + sizeAbility * 6, 158);
			}
		}
	}
}

void Scene3::eraseMask(int mouseX, int mouseY)
{
	int posX, posY;

	// Transform from mouse coordinates to map coordinates
	//   The map is enlarged 3 times and displaced 120 pixels
	posX = mouseX / 3 + 120;
	posY = mouseY / 3;

	for (int y = max(0, posY - 3); y <= min(maskTexture.height() - 1, posY + 3); y++)
		for (int x = max(0, posX - 3); x <= min(maskTexture.width() - 1, posX + 3); x++)
			maskTexture.setPixel(x, y, 0);
}

void Scene3::applyMask(int mouseX, int mouseY)
{
	int posX, posY;

	// Transform from mouse coordinates to map coordinates
	//   The map is enlarged 3 times and displaced 120 pixels
	posX = mouseX / 3 + 120;
	posY = mouseY / 3;

	for (int y = max(0, posY - 3); y <= min(maskTexture.height() - 1, posY + 3); y++)
		for (int x = max(0, posX - 3); x <= min(maskTexture.width() - 1, posX + 3); x++)
			maskTexture.setPixel(x, y, 255);
}

void Scene3::initShaders()
{
	Shader vShader, fShader;

	vShader.initFromFile(VERTEX_SHADER, "shaders/texture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/texture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	simpleTexProgram.init();
	simpleTexProgram.addShader(vShader);
	simpleTexProgram.addShader(fShader);
	simpleTexProgram.link();
	if (!simpleTexProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << simpleTexProgram.log() << endl << endl;
	}
	simpleTexProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();

	vShader.initFromFile(VERTEX_SHADER, "shaders/maskedTexture.vert");
	if (!vShader.isCompiled())
	{
		cout << "Vertex Shader Error" << endl;
		cout << "" << vShader.log() << endl << endl;
	}
	fShader.initFromFile(FRAGMENT_SHADER, "shaders/maskedTexture.frag");
	if (!fShader.isCompiled())
	{
		cout << "Fragment Shader Error" << endl;
		cout << "" << fShader.log() << endl << endl;
	}
	maskedTexProgram.init();
	maskedTexProgram.addShader(vShader);
	maskedTexProgram.addShader(fShader);
	maskedTexProgram.link();
	if (!maskedTexProgram.isLinked())
	{
		cout << "Shader Linking Error" << endl;
		cout << "" << maskedTexProgram.log() << endl << endl;
	}
	maskedTexProgram.bindFragmentOutput("outColor");
	vShader.free();
	fShader.free();
}



