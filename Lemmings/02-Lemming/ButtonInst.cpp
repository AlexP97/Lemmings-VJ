#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "ButtonInst.h"
#include "Game.h"


enum ButtonInstAnims
{
	NORMAL
};


void ButtonInst::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/izq-der.jpg", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(31, 16), glm::vec2(1.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(NORMAL, 0);
	sprite->addKeyframe(NORMAL, glm::vec2(0.f, 0.f));

	sprite->changeAnimation(NORMAL);
	sprite->setPosition(initialPosition);

}

void ButtonInst::update(int deltaTime)
{
	if (sprite->update(deltaTime) == 0)
		return;
}

void ButtonInst::render()
{
	sprite->render();
}