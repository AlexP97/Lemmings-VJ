#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "IconSelected.h"
#include "Game.h"


enum BotonAnims
{
	NORMAL
};


void IconSelected::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram)
{
	state = NOTICONSELECTED_STATE;
	spritesheet.loadFromFile("images/marker.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(74.f / 3.f, 141.f / 3.f), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(NORMAL, 0);
	sprite->addKeyframe(NORMAL, glm::vec2(0.0f / 1.0f, 0.0f / 1.0f));

	sprite->changeAnimation(NORMAL);
	sprite->setPosition(initialPosition);

}

void IconSelected::update(int deltaTime)
{
	if (sprite->update(deltaTime) == 0)
		return;
}

void IconSelected::render()
{
	sprite->render();
}

void IconSelected::changeState(int state2) {
	if (state2 == 0) state = NOTICONSELECTED_STATE;
	else state = ICONSELECTED_STATE;
}

int IconSelected::getState() {
	return state;
}

void IconSelected::setPosition(int x, int y)
{
	sprite->setPosition(glm::vec2(x, y));
}