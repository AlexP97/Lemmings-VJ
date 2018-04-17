#include <iostream>
#include <cmath>
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include "Scene2.h"
#include <Windows.h>
#include <mmsystem.h>


Scene2::Scene2()
{
	map = NULL;
}

Scene2::~Scene2()
{
	if (map != NULL)
		delete map;
}

void Scene2::init()
{
	currentTime = 0;
	lemmingsIn = 0;
	lemmingsOut = 0;
	ability = 0;
	displacement = 0;
	time = 300 * 1000;
	abre_Puerta = true;
	stop_Lemmings = false;
	won = false;
	lemmingsRemaining = 75;
	abilitiesRemaining = vector<int>(7, 0);
	abilitiesRemaining[0] = 20;
	abilitiesRemaining[1] = 20;
	abilitiesRemaining[3] = 20;
	abilitiesRemaining[4] = 20;
	abilitiesRemaining[5] = 20;
	abilitiesRemaining[6] = 20;
	pair<Stairs, bool> p;
	for (int i = 0; i < 20 * 12; i++) stairs.push_back(p);
	nStairs = 0;
	//glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(CAMERA_HEIGHT)) };
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(160.f)) };
	glm::vec2 texCoords[2] = { glm::vec2(330.f / 1024.0, 0.f), glm::vec2((650.f) / 1024.0f, 160.f / 256.0f) };

	initShaders();

	map = MaskedTexturedQuad::createTexturedQuad(geom, texCoords, maskedTexProgram);
	colorTexture.loadFromFile("images/tricky9.png", TEXTURE_PIXEL_FORMAT_RGBA);
	colorTexture.setMinFilter(GL_NEAREST);
	colorTexture.setMagFilter(GL_NEAREST);
	maskTexture.loadFromFile("images/tricky9_mask.png", TEXTURE_PIXEL_FORMAT_L);
	maskTexture.setMinFilter(GL_NEAREST);
	maskTexture.setMagFilter(GL_NEAREST);
	parados.loadFromFile("images/tricky9_mask.png", TEXTURE_PIXEL_FORMAT_L);
	parados.setMinFilter(GL_NEAREST);
	parados.setMagFilter(GL_NEAREST);

	projection = glm::ortho(0.f, float(CAMERA_WIDTH - 1), float(CAMERA_HEIGHT - 1), 0.f);

	puerta.init(glm::vec2(217, 30), simpleTexProgram, 1);
	botonPlay.init(glm::vec2(300, 185), simpleTexProgram, 0);
	botonSpeed.init(glm::vec2(280, 185), simpleTexProgram, 1);
	cursor.init(glm::vec2(90, 30), simpleTexProgram);
	salida.init(glm::vec2(217, 113), simpleTexProgram, 1);
	panel.init(glm::vec2(0, 159), simpleTexProgram);
	iconSelected.init(glm::vec2(9, 158), simpleTexProgram);
	minimap.init(glm::vec2(178, 180), simpleTexProgram, 1);
	mRectangle1.init(glm::vec2(177, 179), simpleTexProgram, 1, 0);
	mRectangle2.init(glm::vec2(202, 179), simpleTexProgram, 1, 1);
	text.init("fonts/OpenSans-Regular.ttf");

	lemmingsIn = 0;

	lemmingInit = vector<bool>(75, false);
	Lemming lem;
	lemming = vector<Lemming>(0);
	for (int i = 0; i < 75; i++) {
		lemming.push_back(lem);
	}
	ability = 0;
	mciSendString(TEXT("play sound/LETSGO.WAV"), NULL, 0, NULL);
}

//unsigned int x = 0; 

pair<bool, bool> Scene2::update(int deltaTime)
{
	pair<bool, bool> ret;
	ret.first = true;
	ret.second = false;

	currentTime += deltaTime;
	if (time > 0) time -= deltaTime;

	if (lemmingsOut >= (75.f*0.93f)) {
		won = true;
	}
	if (lemmingsRemaining == 0) {
		if (won) {
			ret.first = false;
			ret.second = true;
			return ret;
		}
		else {
			ret.first = false;
			ret.second = false;
			return ret;
		}
	}

	if (abre_Puerta && currentTime >= 2000) {
		puerta.open();
		mciSendString(TEXT("play sound/DOOR.WAV"), NULL, 0, NULL);
		abre_Puerta = false;
	}

	if (!stop_Lemmings && lemmingsIn < 75 && currentTime >= (3000 * (lemmingsIn + 1))) {
		lemming[lemmingsIn].init(glm::vec2(225 - displacement, 30), simpleTexProgram, 330 + displacement);
		lemming[lemmingsIn].setMapMask(&maskTexture, &parados);
		lemming[lemmingsIn].setStairs(stairs, nStairs);
		lemmingInit[lemmingsIn] = true;
		++lemmingsIn;
	}

	for (int i = 0; i < lemming.size(); i++) {
		if (lemmingInit[i]) {
			if (lemmingOnExit(lemming[i].position()) && !lemming[i].goOut()) {
				mciSendString(TEXT("play sound/YIPPEE.WAV"), NULL, 0, NULL);
				lemming[i].setComeOut(true);
				lemmingsOut++;
			}
			if (lemming[i].eliminar()) {
				lemmingInit[i] = false;
				lemmingsRemaining--;
			}

		}
	}

	for (int i = 0; i < lemming.size(); i++) {
		if (lemmingInit[i]) {
			int cont = lemming[i].update(deltaTime);
			if (stop_Lemmings && !cont) {
				ret.first = false;
				if (won) ret.second = true;
				else ret.second = false;
				return ret;
			}
		}
	}

	for (int i = 0; i < lemming.size(); i++) {
		if (lemmingInit[i]) {
			pair<bool, int> putStair = lemming[i].putStair();
			if (putStair.first) {
				glm::vec2 pos = lemming[i].position();
				pos.y += 15;
				nStairs++;
				if (putStair.second == 0) {		//escalera hacia la izquierda
					pos.x += 2;
					stairs[nStairs - 1].first.init(pos, simpleTexProgram);
					stairs[nStairs - 1].second = 0;
				}
				else if (putStair.second == 1) {	//escalera hacia la derecha
					pos.x += 9;
					stairs[nStairs - 1].first.init(pos, simpleTexProgram);
					stairs[nStairs - 1].second = 1;
				}
				for (int j = 0; j < lemming.size(); j++) {
					if (lemmingInit[j]) {
						lemming[j].setStairs(stairs, nStairs);
					}
				}
			}
		}
	}

	puerta.update(deltaTime);
	botonPlay.update(deltaTime);
	botonSpeed.update(deltaTime);
	salida.update(deltaTime);
	for (int i = 0; i < nStairs; i++) {
		stairs[i].first.update(deltaTime);
	}
	minimap.update(deltaTime);
	mRectangle1.update(deltaTime);
	mRectangle2.update(deltaTime);
	panel.update(deltaTime);
	if (iconSelected.getState() == 1) iconSelected.update(deltaTime);
	cursor.update(deltaTime);
	if (time < 0) {
		time = 0;
		ability = 8;
		explode();
	}
	ret.first = true;
	ret.second = false;
	return ret;
}

void Scene2::render()
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
	for (int i = 0; i < nStairs; i++) {
		stairs[i].first.render();
	}
	panel.render();
	minimap.render();
	mRectangle1.render();
	mRectangle2.render();

	if (iconSelected.getState() == 1) iconSelected.render();
	cursor.render();
	float percentage = 0;
	if (lemmingsIn > 0) percentage = (float(lemmingsOut) / float(75)) * 100;
	int timeInSeconds = time / 1000;
	int minutes = timeInSeconds / 60;
	int seconds = timeInSeconds % 60;
	if (seconds > 9) text.render("OUT  " + to_string(lemmingsIn) + "   IN  " + to_string(int(percentage)) + "%  TIME  " + to_string(minutes) + ":" + to_string(seconds), glm::vec2(545, 510), 32, glm::vec4(0.4, 0.96, 0.07, 1));
	else text.render("OUT  " + to_string(lemmingsIn) + "   IN  " + to_string(int(percentage)) + "%  TIME  " + to_string(minutes) + ":0" + to_string(seconds), glm::vec2(545, 510), 32, glm::vec4(0.4, 0.96, 0.07, 1));
	if (abilitiesRemaining[0] > 9) text.render(to_string(abilitiesRemaining[0]), glm::vec2(11, 530), 40, glm::vec4(0.0, 0.0, 0.0, 1));
	else text.render(to_string(abilitiesRemaining[0]), glm::vec2(24, 530), 40, glm::vec4(0.0, 0.0, 0.0, 1));
	if (abilitiesRemaining[1] > 9) text.render(to_string(abilitiesRemaining[1]), glm::vec2(78, 530), 40, glm::vec4(0.0, 0.0, 0.0, 1));
	else text.render(to_string(abilitiesRemaining[1]), glm::vec2(91, 530), 40, glm::vec4(0.0, 0.0, 0.0, 1));
	if (abilitiesRemaining[2] > 9) text.render(to_string(abilitiesRemaining[2]), glm::vec2(143, 530), 40, glm::vec4(0.0, 0.0, 0.0, 1));
	else text.render(to_string(abilitiesRemaining[2]), glm::vec2(156, 530), 40, glm::vec4(0.0, 0.0, 0.0, 1));
	if (abilitiesRemaining[3] > 9) text.render(to_string(abilitiesRemaining[3]), glm::vec2(210, 530), 40, glm::vec4(0.0, 0.0, 0.0, 1));
	else text.render(to_string(abilitiesRemaining[3]), glm::vec2(223, 530), 40, glm::vec4(0.0, 0.0, 0.0, 1));
	if (abilitiesRemaining[4] > 9) text.render(to_string(abilitiesRemaining[4]), glm::vec2(276, 530), 40, glm::vec4(0.0, 0.0, 0.0, 1));
	else text.render(to_string(abilitiesRemaining[4]), glm::vec2(289, 530), 40, glm::vec4(0.0, 0.0, 0.0, 1));
	if (abilitiesRemaining[5] > 9) text.render(to_string(abilitiesRemaining[5]), glm::vec2(344, 530), 40, glm::vec4(0.0, 0.0, 0.0, 1));
	else text.render(to_string(abilitiesRemaining[5]), glm::vec2(357, 530), 40, glm::vec4(0.0, 0.0, 0.0, 1));
	if (abilitiesRemaining[6] > 9) text.render(to_string(abilitiesRemaining[6]), glm::vec2(411, 530), 40, glm::vec4(0.0, 0.0, 0.0, 1));
	else text.render(to_string(abilitiesRemaining[6]), glm::vec2(425, 530), 40, glm::vec4(0.0, 0.0, 0.0, 1));
}

pair<bool, bool> Scene2::mouseMoved(int mouseX, int mouseY, bool bLeftButton, bool bRightButton, bool paused)
{
	pair<bool, bool> ret;
	ret.first = false;
	ret.second = false;
	cursor.setPosition(mouseX, mouseY);
	if (!paused) {
		pair<bool, bool> changeDisp = mouseOnBorder(mouseX, mouseY);
		if (changeDisp.first) {
			float d = 0;
			if (changeDisp.second && (displacement >= -225)) d = -2.f;
			else if (!changeDisp.second && (displacement <= 300)) d = 2.f;
			changeDisplacement(d);
			mRectangle2.displace(d / 7.5f);
		}

		bool moved = false;
		bool oneFloats = false;
		for (unsigned int i = 0; i < lemming.size(); i++) {
			if (lemmingInit[i]) {
				if (lemming[i].floats()) {
					oneFloats = true;
					moved = lemming[i].moveFloatingLemming(mouseX, mouseY);
				}
			}
		}
		pair<bool, int> cOL = cursorOnLemming(mouseX, mouseY);
		if (oneFloats) {
			if (cOL.first) cursor.changeAnimation(1);
			else if (moved && (cursor.currentAnimation() != 3)) cursor.changeAnimation(3);
			else cursor.changeAnimation(2);
		}
		else {
			if (cursor.currentAnimation() == 0) {
				if (cOL.first) cursor.changeAnimation(1);
			}
			else {
				if (!cOL.first) cursor.changeAnimation(0);
			}
		}

		if (bLeftButton) {
			clickOnAbility(mouseX, mouseY);
			if (ability == 8) {
				explode();
			}
			if (cOL.first && ability != 8) {
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

pair<bool, bool> Scene2::mouseOnBorder(int mouseX, int mouseY) {
	int x = mouseX / 3 - 2.f;
	int y = mouseY / 3 - 2.f;
	pair<bool, bool> ret;
	ret.first = ret.second = false;
	if (y < 160) {
		if (x < 20) {
			ret.first = true;
			ret.second = true;
		}
		else if (x > 300) {
			ret.first = true;
		}
	}
	return ret;
}

void Scene2::explode() {
	for (unsigned int i = 0; i < lemming.size(); i++) {
		if (lemmingInit[i]) {
			lemming[i].setAbility(ability);
			stop_Lemmings = true;
		}
	}
}

void Scene2::clickOnLemming(int indLemming) {
	switch (ability) {
	case 1:
		if (abilitiesRemaining[0] > 0) {
			lemming[indLemming].setAbility(ability);
			abilitiesRemaining[0]--;
		}
		break;
	case 2:
		if (abilitiesRemaining[1] > 0) {
			lemming[indLemming].setAbility(ability);
			abilitiesRemaining[1]--;
		}
		break;
	case 3:
		if (abilitiesRemaining[2] > 0) {
			lemming[indLemming].setAbility(ability);
			abilitiesRemaining[2]--;
		}
		break;
	case 4:
		if (abilitiesRemaining[3] > 0) {
			lemming[indLemming].setAbility(ability);
			abilitiesRemaining[3]--;
		}
		break;
	case 5:
		if (abilitiesRemaining[4] > 0) {
			lemming[indLemming].setAbility(ability);
			abilitiesRemaining[4]--;
		}
		break;
	case 6:
		if (abilitiesRemaining[5] > 0) {
			lemming[indLemming].setAbility(ability);
			abilitiesRemaining[5]--;
		}
		break;
	case 7:
		if (abilitiesRemaining[6] > 0) {
			lemming[indLemming].setAbility(ability);
			abilitiesRemaining[6]--;
		}
		break;
	}
	if (ability != 0) mciSendString(TEXT("play sound/ACTION.WAV"), NULL, 0, NULL);
}

bool Scene2::clickOnPause(int mouseX, int mouseY) {
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

bool Scene2::clickOnSpeed(int mouseX, int mouseY) {
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

bool Scene2::lemmingOnExit(glm::vec2 position) {
	glm::vec2 salidaPos = salida.position();
	if (position.x > (salidaPos.x + 4.f) && position.x < (salidaPos.x + 10.f)) {
		if (position.y < (salidaPos.y + 12.f) && position.y > salidaPos.y) return true;
	}
	return false;
}

pair<bool, int> Scene2::cursorOnLemming(int mouseX, int mouseY) {
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
					if (!lemming[i].floats()) {
						p.first = true;
						p.second = i;
						return p;
					}
				}
			}
		}
	}
	return p;
}



void Scene2::clickOnAbility(int mouseX, int mouseY) {
	int x = mouseX / 3;
	int y = mouseY / 3;
	glm::vec2 positionPanel = panel.position();
	float sizeX = 530.f / 3.f;
	float sizeY = 135.f / 3.f;
	float sizeAbility = sizeX / 8.f;

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
					ability = 7;
				}
				iconSelected.setPosition(positionPanel.x - 1 + sizeAbility * 6, 158);
			}
			else if (x >(positionPanel.x + sizeAbility * 7 - 2) && x < (positionPanel.x + sizeAbility * 8 - 2)) {
				if (ability == 8) {
					iconSelected.changeState(0);
					ability = 0;
				}
				else {
					iconSelected.changeState(1);
					stop_Lemmings = true;
					ability = 8;
				}
				iconSelected.setPosition(positionPanel.x - 1 + sizeAbility * 7, 158);
			}
		}
	}
}

void Scene2::changeDisplacement(float d) {
	displacement += d;
	
	glm::vec2 geom[2] = { glm::vec2(0.f, 0.f), glm::vec2(float(CAMERA_WIDTH), float(160.f)) };
	glm::vec2 texCoords[2] = { glm::vec2((330.f + displacement) / 1024.0, 0.f), glm::vec2((650.f + displacement) / 1024.0f, 160.f / 256.0f) };

	map = MaskedTexturedQuad::createTexturedQuad(geom, texCoords, maskedTexProgram);
	
	for (int i = 0; i < lemming.size(); i++) {
		if (lemmingInit[i]) {
			lemming[i].displace(-d);
			
		}
	}
	puerta.displace(-d);
	salida.displace(-d);
	for (int i = 0; i < nStairs; i++) {
		stairs[i].first.displace(-d);
	}
}

void Scene2::eraseMask(int mouseX, int mouseY)
{
	int posX, posY;

	// Transform from mouse coordinates to map coordinates
	//   The map is enlarged 3 times and displaced 120 pixels
	posX = mouseX / 3 + 330;
	posY = mouseY / 3;

	for (int y = max(0, posY - 3); y <= min(maskTexture.height() - 1, posY + 3); y++)
		for (int x = max(0, posX - 3); x <= min(maskTexture.width() - 1, posX + 3); x++)
			maskTexture.setPixel(x, y, 0);
}

void Scene2::applyMask(int mouseX, int mouseY)
{
	int posX, posY;

	// Transform from mouse coordinates to map coordinates
	//   The map is enlarged 3 times and displaced 120 pixels
	posX = mouseX / 3 + 330;
	posY = mouseY / 3;

	for (int y = max(0, posY - 3); y <= min(maskTexture.height() - 1, posY + 3); y++)
		for (int x = max(0, posX - 3); x <= min(maskTexture.width() - 1, posX + 3); x++)
			maskTexture.setPixel(x, y, 255);
}

void Scene2::initShaders()
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



