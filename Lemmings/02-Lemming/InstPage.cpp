#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "InstPage.h"
#include "Game.h"


enum InstPageAnims
{
	PAG1, PAG2, PAG3, PAG4
};


void InstPage::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/instrucciones.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(200, 180), glm::vec2(0.5f, 0.5f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(PAG1, 0);
	sprite->addKeyframe(PAG1, glm::vec2(0.f, 0.f));
	sprite->setAnimationSpeed(PAG2, 0);
	sprite->addKeyframe(PAG2, glm::vec2(0.5f, 0.f));
	sprite->setAnimationSpeed(PAG3, 0);
	sprite->addKeyframe(PAG3, glm::vec2(0.f, 0.5f));
	sprite->setAnimationSpeed(PAG4, 0);
	sprite->addKeyframe(PAG4, glm::vec2(0.5f, 0.5f));

	sprite->changeAnimation(PAG1);
	sprite->setPosition(initialPosition);

}

void InstPage::next() {
	int x = sprite->animation();
	if (x < 4) sprite->changeAnimation(x + 1);
}

void InstPage::before() {
	int x = sprite->animation();
	if (x > 0) sprite->changeAnimation(x - 1);
}

void InstPage::update(int deltaTime)
{
	if (sprite->update(deltaTime) == 0)
		return;
}

void InstPage::render()
{
	sprite->render();
}