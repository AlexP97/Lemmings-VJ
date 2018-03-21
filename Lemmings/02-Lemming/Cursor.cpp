#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Cursor.h"
#include "Game.h"

enum CursorAnims
{
	NORMAL, HOVER
};


void Cursor::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/miscelanea.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.9f / 20.0f, 0.9f / 30.0f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(2);

	sprite->setAnimationSpeed(NORMAL, 0);
	sprite->addKeyframe(NORMAL, glm::vec2(0.6f / 16.0f, 12.5 / 32.0));

	sprite->setAnimationSpeed(HOVER, 0);
	sprite->addKeyframe(HOVER, glm::vec2(0.0f / 16.0f, 12.5 / 32.0));

	sprite->changeAnimation(NORMAL);
	sprite->setPosition(initialPosition);
}

void Cursor::update(int deltaTime)
{
	int fall;

	if (sprite->update(deltaTime) == 0)
		return;
}

void Cursor::render()
{
	sprite->render();
}

void Cursor::setPosition(int x, int y)
{
	int posX = x / 3 - 6;
	int posY = y / 3 - 6;
	sprite->setPosition(glm::vec2(posX, posY));
}

int Cursor::currentAnimation() {
	return sprite->animation();
}

void Cursor::changeAnimation(int anim) {
	sprite->changeAnimation(anim);
}
