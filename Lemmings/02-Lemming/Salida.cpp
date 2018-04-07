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


void Salida::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, int escena)
{
	if (escena == 0) spritesheet.loadFromFile("images/puertas3.png", TEXTURE_PIXEL_FORMAT_RGBA);
	else if (escena == 1) spritesheet.loadFromFile("images/puertas3.png", TEXTURE_PIXEL_FORMAT_RGBA);
	else if (escena == 2) spritesheet.loadFromFile("images/puertas7_sinfondo.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	if (escena == 0) sprite = Sprite::createSprite(glm::ivec2(32, 24), glm::vec2(1.f, 1.f / 6.f), &spritesheet, &shaderProgram);
	else if (escena == 2) sprite = Sprite::createSprite(glm::ivec2(44, 46), glm::vec2(1.f / 6.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(1);

	sprite->setAnimationSpeed(NORMAL, 5);

	if (escena == 0) {
		for (int i = 0; i<6; i++)
			sprite->addKeyframe(NORMAL, glm::vec2(0.f, float(i) / 6.f));
	}
	else if (escena == 1) {
		for (int i = 0; i<6; i++)
			sprite->addKeyframe(NORMAL, glm::vec2(0.f, float(i) / 6.f));
	}
	else if (escena == 2) {
		for (int i = 0; i<6; i++)
			sprite->addKeyframe(NORMAL, glm::vec2(float(i) / 6.f, 0.f));
	}

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