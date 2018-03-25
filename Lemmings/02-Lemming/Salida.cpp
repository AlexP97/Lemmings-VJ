#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Salida.h"
#include "Game.h"


enum SalidaAnims
{
	NORMAL
};


void Salida::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/puertas3.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(32, 24), glm::vec2(1.f, 1.f / 6.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(NORMAL, 5);

	for (int i = 0; i<10; i++)
		sprite->addKeyframe(NORMAL, glm::vec2(0.f, float(i) / 6.f));

	sprite->changeAnimation(NORMAL);
	sprite->setPosition(initialPosition);

}

void Salida::update(int deltaTime)
{
	if (sprite->update(deltaTime) == 0)
		return;
}

void Salida::render()
{
	sprite->render();
}

glm::vec2 Salida::position() {
	return sprite->position();
}