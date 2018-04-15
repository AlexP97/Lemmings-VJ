#include <cmath>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Cursor.h"
#include "Game.h"

enum CursorAnims
{
	NORMAL, HOVER, FAN_NORMAL, FAN_MOVING
};


void Cursor::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram)
{
	spritesheet.loadFromFile("images/cursor.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(1.f / 6.f, 1.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(4);

	sprite->setAnimationSpeed(NORMAL, 0);
	sprite->addKeyframe(NORMAL, glm::vec2(1.f / 6.f, 1.f));

	sprite->setAnimationSpeed(HOVER, 0);
	sprite->addKeyframe(HOVER, glm::vec2(0.0f / 6.0f, 1.f));

	sprite->setAnimationSpeed(FAN_NORMAL, 0);
	sprite->addKeyframe(FAN_NORMAL, glm::vec2(2.f / 6.f, 1.f));

	sprite->setAnimationSpeed(FAN_MOVING, 20);
	for (int i = 2; i<6; i++)
		sprite->addKeyframe(FAN_MOVING, glm::vec2(float(i) / 6.f, 1.f));

	sprite->changeAnimation(FAN_NORMAL);
	sprite->setPosition(initialPosition);
}

void Cursor::update(int deltaTime)
{
	if (sprite->update(deltaTime) == 0)
		return;
}

void Cursor::render()
{
	sprite->render();
}

void Cursor::setPosition(int x, int y)
{
	int posX = x / 3 - 8;
	int posY = y / 3 - 8;
	sprite->setPosition(glm::vec2(posX, posY));
}

int Cursor::currentAnimation() {
	return sprite->animation();
}

void Cursor::changeAnimation(int anim) {
	sprite->changeAnimation(anim);
}
