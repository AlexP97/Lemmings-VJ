#include <cmath>
#include <algorithm>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Lemming.h"
#include "Game.h"


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum LemmingAnims
{
	WALKING_LEFT, WALKING_RIGHT, COMING_OUT, FALLING_LEFT, FALLING_RIGHT, DIG
};


void Lemming::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram)
{
	come_Out = false;
	state = FALLING_RIGHT_STATE;
	spritesheet.loadFromFile("images/lemming_sinfondo3.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.125, 1 / 16.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(6);
	
		sprite->setAnimationSpeed(WALKING_RIGHT, 12);
		for(int i=0; i<8; i++)
			sprite->addKeyframe(WALKING_RIGHT, glm::vec2(float(i) / 8, 0.f / 16.f));
		
		sprite->setAnimationSpeed(WALKING_LEFT, 12);
		for(int i=0; i<8; i++)
			sprite->addKeyframe(WALKING_LEFT, glm::vec2(float(i) / 8, 1.f / 16.f));

		sprite->setAnimationSpeed(COMING_OUT, 9);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(COMING_OUT, glm::vec2(float(i) / 8, 2.f / 16.f));

		sprite->setAnimationSpeed(FALLING_LEFT, 12);
		for (int i = 0; i<4; i++)
			sprite->addKeyframe(FALLING_LEFT, glm::vec2(float(i)+4 / 8, 3.f / 16.f));

		sprite->setAnimationSpeed(FALLING_RIGHT, 12);
		for (int i = 0; i<4; i++)
			sprite->addKeyframe(FALLING_RIGHT, glm::vec2(float(i) / 8, 3.f / 16.f));

		sprite->setAnimationSpeed(DIG, 9);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(DIG, glm::vec2(float(i) / 8, 6.f / 16.f));
		
	sprite->changeAnimation(FALLING_RIGHT);
	sprite->setPosition(initialPosition);
}

void Lemming::update(int deltaTime)
{
	int fall;

	if(sprite->update(deltaTime) == 0)
		return;

	switch(state)
	{
	case FALLING_LEFT_STATE:
		fall = collisionFloor(2);
		if(fall > 0)
			sprite->position() += glm::vec2(0, fall);
		else {
			sprite->changeAnimation(WALKING_LEFT);
			state = WALKING_LEFT_STATE;
		}
		break;

	case FALLING_RIGHT_STATE:
		fall = collisionFloor(2);
		if(fall > 0)
			sprite->position() += glm::vec2(0, fall);
		else {
			sprite->changeAnimation(WALKING_RIGHT);
			state = WALKING_RIGHT_STATE;
		}
		break;

	case WALKING_LEFT_STATE:
		sprite->position() += glm::vec2(-1, -1);
		if (come_Out) {
			sprite->changeAnimation(COMING_OUT);
			state = COMING_OUT_STATE;
		}
		else if(collision())
		{
			sprite->position() -= glm::vec2(-1, -1);
			sprite->changeAnimation(WALKING_RIGHT);
			state = WALKING_RIGHT_STATE;
		}
		else
		{
			fall = collisionFloor(3);
			if(fall > 0)
				sprite->position() += glm::vec2(0, 1);
			if(fall > 1)
				sprite->position() += glm::vec2(0, 1);
			if (fall > 2) {
				sprite->changeAnimation(FALLING_LEFT);
				state = FALLING_LEFT_STATE;
			}
		}
		break;

	case WALKING_RIGHT_STATE:
		sprite->position() += glm::vec2(1, -1);
		if (come_Out) {
			sprite->changeAnimation(COMING_OUT);
			state = COMING_OUT_STATE;
		}
		else if(collision())
		{
			sprite->position() -= glm::vec2(1, -1);
			sprite->changeAnimation(WALKING_LEFT);
			state = WALKING_LEFT_STATE;
		}
		else
		{
			fall = collisionFloor(3);
			if(fall < 3)
				sprite->position() += glm::vec2(0, fall);
			else {
				sprite->changeAnimation(FALLING_RIGHT);
				state = FALLING_RIGHT_STATE;
			}
		}
		break;

	case COMING_OUT_STATE:
		break;

	case DIG_STATE:
		fall = collisionFloor(3);
		if (fall > 0)
			sprite->position() += glm::vec2(0, 1);
		if (fall > 1)
			sprite->position() += glm::vec2(0, 1);
		if (fall > 2) {
			sprite->changeAnimation(FALLING_RIGHT);
			state = FALLING_RIGHT_STATE;
		}
		if (state != FALLING_RIGHT_STATE) {
			eraseMask();
			sprite->position() += glm::vec2(0, 1);
		}
		break;
	}
}

void Lemming::render()
{
	sprite->render();
}

glm::vec2 Lemming::position() {
	return sprite->position();
}

void Lemming::setMapMask(VariableTexture *mapMask)
{
	mask = mapMask;
}

bool Lemming::eliminar() 
{
	return (sprite->animation() == COMING_OUT) && (sprite->currentKeyFrame() == 7);
}

void Lemming::setComeOut(bool b) {
	come_Out = b;
}

int Lemming::collisionFloor(int maxFall)
{
	bool bContact = false;
	int fall = 0;
	glm::ivec2 posBase = sprite->position() + glm::vec2(120, 0); // Add the map displacement
	
	posBase += glm::ivec2(7, 16);
	while((fall < maxFall) && !bContact)
	{
		if((mask->pixel(posBase.x, posBase.y+fall) == 0) && (mask->pixel(posBase.x+1, posBase.y+fall) == 0))
			fall += 1;
		else
			bContact = true;
	}

	return fall;
}

bool Lemming::collision()
{
	glm::ivec2 posBase = sprite->position() + glm::vec2(120, 0); // Add the map displacement
	
	posBase += glm::ivec2(7, 15);
	if((mask->pixel(posBase.x, posBase.y) == 0) && (mask->pixel(posBase.x+1, posBase.y) == 0))
		return false;

	return true;
}

void Lemming::putAbility(int ability) {
	if (ability == 4) {
		state = DIG_STATE;
		sprite->changeAnimation(DIG);
	}
}

void Lemming::eraseMask() {
	int posX, posY;
	posX = position()[0]+8+120;
	posY = position()[1]+16;

	for (int y = max(0, posY); y <= min((double)mask->height() - 1, posY + 0.25); y++) {
		for (int x = max(0, posX - 3); x <= min(mask->width() - 1, posX + 3); x++) {
			mask->setPixel(x, y, 0);
		}
	}

	
}








