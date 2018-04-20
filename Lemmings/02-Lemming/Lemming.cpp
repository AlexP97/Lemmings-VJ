#include <cmath>
#include <algorithm>
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "Lemming.h"
#include "Game.h"
#include <Windows.h>
#include <mmsystem.h>


#define JUMP_ANGLE_STEP 4
#define JUMP_HEIGHT 96
#define FALL_STEP 4


enum LemmingAnims
{
	WALKING_LEFT, WALKING_RIGHT, COMING_OUT, FALLING_LEFT, FALLING_RIGHT, DIG, BLOCKING, DIG_LEFT, DIG_RIGHT, EXPLODING, CLIMBING_LEFT,
	CLIMBING_RIGHT, STOPPING_CLIMB_LEFT, STOPPING_CLIMB_RIGHT, BUILDING_LEFT, BUILDING_RIGHT, WALKING_RIGHT_STAIR, FLOATING_RIGHT,
	FLOATING_LEFT
};


void Lemming::init(const glm::vec2 &initialPosition, ShaderProgram &shaderProgram, int desplazamiento)
{
	eliminado = false;
	climber = false;
	come_Out = false;
	primeraPasada = false;
	numberOfStairs = 0;
	floating = false;
	currentFloating = 0;
	displacement = desplazamiento;
	state = FALLING_RIGHT_STATE;
	spritesheet.loadFromFile("images/lemming3_sinfondo.png", TEXTURE_PIXEL_FORMAT_RGBA);
	spritesheet.setMinFilter(GL_NEAREST);
	spritesheet.setMagFilter(GL_NEAREST);
	sprite = Sprite::createSprite(glm::ivec2(16, 16), glm::vec2(0.125, 1 / 24.f), &spritesheet, &shaderProgram);
	sprite->setNumberAnimations(18);
	
		sprite->setAnimationSpeed(WALKING_RIGHT, 12);
		for(int i=0; i<8; i++)
			sprite->addKeyframe(WALKING_RIGHT, glm::vec2(float(i) / 8, 0.f / 24.f));
		
		sprite->setAnimationSpeed(WALKING_LEFT, 12);
		for(int i=0; i<8; i++)
			sprite->addKeyframe(WALKING_LEFT, glm::vec2(float(i) / 8, 1.f / 24.f));

		sprite->setAnimationSpeed(COMING_OUT, 9);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(COMING_OUT, glm::vec2(float(i) / 8, 2.f / 24.f));

		sprite->setAnimationSpeed(FALLING_LEFT, 12);
		for (int i = 7; i>3; i--)
			sprite->addKeyframe(FALLING_LEFT, glm::vec2(float(i) / 8, 3.f / 24.f));

		sprite->setAnimationSpeed(FALLING_RIGHT, 12);
		for (int i = 0; i<4; i++)
			sprite->addKeyframe(FALLING_RIGHT, glm::vec2(float(i) / 8, 3.f / 24.f));

		sprite->setAnimationSpeed(DIG, 9);
		for (int i = 0; i<8; i++)
			sprite->addKeyframe(DIG, glm::vec2(float(i) / 8, 6.f / 24.f));

		sprite->setAnimationSpeed(DIG_RIGHT, 9);
		for (int j = 0; j < 2; j++) {
			for (int i = 0; i < 8; i++) {
				sprite->addKeyframe(DIG_RIGHT, glm::vec2(float(i) / 8, (7.f + float(j)) / 24.f));
			}
		}

		sprite->setAnimationSpeed(DIG_LEFT, 9);
		for (int j = 0; j < 2; j++) {
			for (int i = 0; i < 8; i++) {
				sprite->addKeyframe(DIG_LEFT, glm::vec2(float(i) / 8, (9.f + float(j)) / 24.f));
			}
		}


		sprite->setAnimationSpeed(BLOCKING, 9);
		for (int j = 0; j < 2; j++) {
			for (int i = 0; i < 8; i++) {
				sprite->addKeyframe(BLOCKING, glm::vec2(float(i) / 8, (4.f+float(j)) / 24.f));
			}
		}

		sprite->setAnimationSpeed(EXPLODING, 12);
		for (int j = 0; j < 4; j++) {
			for (int i = 0; i < 8; i++) {
				sprite->addKeyframe(EXPLODING, glm::vec2(float(i) / 8, (15.f + float(j)) / 24.f));
			}
		}


		sprite->setAnimationSpeed(CLIMBING_LEFT, 9);
		for (int i = 0; i < 8; i++) {
			sprite->addKeyframe(CLIMBING_LEFT, glm::vec2(float(i) / 8, 13.f / 24.f));
		}

		sprite->setAnimationSpeed(CLIMBING_RIGHT, 9);
		for (int i = 0; i < 8; i++) {
			sprite->addKeyframe(CLIMBING_RIGHT, glm::vec2(float(i) / 8, 11.f / 24.f));
		}

		sprite->setAnimationSpeed(STOPPING_CLIMB_LEFT, 12);
		for (int i = 0; i < 8; i++) {
			sprite->addKeyframe(STOPPING_CLIMB_LEFT, glm::vec2(float(i) / 8, 14.f / 24.f));
		}

		sprite->setAnimationSpeed(STOPPING_CLIMB_RIGHT, 12);
		for (int i = 0; i < 8; i++) {
			sprite->addKeyframe(STOPPING_CLIMB_RIGHT, glm::vec2(float(i) / 8, 12.f / 24.f));
		}

		sprite->setAnimationSpeed(BUILDING_LEFT, 9);
		for (int j = 0; j < 2; j++) {
			for (int i = 7; i >= 0; i--) {
				sprite->addKeyframe(BUILDING_LEFT, glm::vec2(float(i) / 8, (21.f + float(j)) / 24.f));
			}
		}

		sprite->setAnimationSpeed(BUILDING_RIGHT, 9);
		for (int j = 0; j < 2; j++) {
			for (int i = 0; i < 8; i++) {
				sprite->addKeyframe(BUILDING_RIGHT, glm::vec2(float(i) / 8, (19.f + float(j)) / 24.f));
			}
		}
		sprite->setAnimationSpeed(FLOATING_RIGHT, 12);
		for (int i = 0; i < 4; i++) {
			sprite->addKeyframe(FLOATING_RIGHT, glm::vec2(glm::vec2(float(i) / 8, 23.f / 24.f)));
		}
		sprite->setAnimationSpeed(FLOATING_LEFT, 12);
		for (int i = 4; i < 8; i++) {
			sprite->addKeyframe(FLOATING_LEFT, glm::vec2(glm::vec2(float(i) / 8, 23.f / 24.f)));
		}
		
	sprite->changeAnimation(FALLING_RIGHT);
	sprite->setPosition(initialPosition);
}

bool Lemming::update(int deltaTime)
{
	int fall;

	if(sprite->update(deltaTime) == 0)
		return true;

	switch (state) {

	case FALLING_LEFT_STATE:
		primeraPasada = false;
		fall = collisionFloor(2);
		if (fall > 0)
			sprite->position() += glm::vec2(0, fall);
		else {
			sprite->changeAnimation(WALKING_LEFT);
			state = WALKING_LEFT_STATE;
		}
		break;

	case FALLING_RIGHT_STATE:
		primeraPasada = false;
		fall = collisionFloor(2);
		if (fall > 0)
			sprite->position() += glm::vec2(0, fall);
		else {
			sprite->changeAnimation(WALKING_RIGHT);
			state = WALKING_RIGHT_STATE;
		}
		break;

	case WALKING_LEFT_STATE:
		
		sprite->position() += glm::vec2(-1, -1);
		int fallStair;
		fallStair = collisionStairLeft();
		if (collision() || hayParado() && primeraPasada)
		{
			primeraPasada = true;
			if (!climber) {
				sprite->position() -= glm::vec2(-1, -1);
				sprite->changeAnimation(WALKING_RIGHT);
				state = WALKING_RIGHT_STATE;
			}
			else {
				sprite->position() -= glm::vec2(-1, -1);
				sprite->changeAnimation(CLIMBING_LEFT);
				state = CLIMBING_LEFT_STATE;
			}
		}
		else if (fallStair != -2) {
			sprite->position() += glm::vec2(0, fallStair);
		}
		else
		{
			if (!hayParado())
				primeraPasada = true;
			fall = collisionFloor(3);
			if (fall > 0)
				sprite->position() += glm::vec2(0, 1);
			if (fall > 1)
				sprite->position() += glm::vec2(0, 1);
			if (fall > 2) {
				sprite->changeAnimation(FALLING_LEFT);
				state = FALLING_LEFT_STATE;
			}
		}
		
		break;

	case WALKING_RIGHT_STATE:
		sprite->position() += glm::vec2(1, -1);
		fallStair = collisionStairRight();
		if (collision() || (hayParado() && primeraPasada))
		{
			primeraPasada = true;
			if (!climber) {
				sprite->position() -= glm::vec2(1, -1);
				sprite->changeAnimation(WALKING_LEFT);
				state = WALKING_LEFT_STATE;
			}
			else {
				sprite->position() -= glm::vec2(1, -1);
				sprite->changeAnimation(CLIMBING_RIGHT);
				state = CLIMBING_RIGHT_STATE;
			}
		}
		else if (fallStair != -2) {
			sprite->position() += glm::vec2(0, fallStair);
		}
		else
		{
			if (!hayParado())
				primeraPasada = true;
			fall = collisionFloor(3);
			if (fall < 3)
				sprite->position() += glm::vec2(0, fall);
			else {
				sprite->changeAnimation(FALLING_RIGHT);
				state = FALLING_RIGHT_STATE;
			}
		}
		break;
	case COMING_OUT_STATE:
		if (sprite->currentKeyFrame() == 7) {
			eliminado = true;
		}
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
			eraseMask(position()[0] + 8 + displacement, position()[1] + 16, 0, 0.25,-3,3, mask);
			sprite->position() += glm::vec2(0, 0.25);
		}
		break;

	case BLOCKING_STATE:
		break;

	case DIG_LEFT_STATE:
		sprite->position() += glm::vec2(-1.0, 0);
		if (!collision()) {
			sprite->changeAnimation(WALKING_LEFT);
			state = WALKING_LEFT_STATE;
		}
		break;

	case DIG_RIGHT_STATE:
		sprite->position() += glm::vec2(1.0, 0);
		if (!collision()) {
			sprite->changeAnimation(WALKING_RIGHT);
			state = WALKING_RIGHT_STATE;
		}
		break;

	case EXPLODE_STATE:
		if (sprite->currentKeyFrame() == 20) {
			mciSendString(TEXT("play sound/BANG.WAV"), NULL, 0, NULL);
		}
		else if (sprite->currentKeyFrame() == 31) {
			eliminado = true;
			return false;
		}
		break;
	case CLIMBING_LEFT_STATE:
		sprite->position() += glm::vec2(-1, -1);
		if (collision()) {
			sprite->position() += glm::vec2(1, 0.5);
		}
		else {
			sprite->changeAnimation(STOPPING_CLIMB_LEFT);
			state = STOPPING_CLIMB_LEFT_STATE;
		}
		break;
	case CLIMBING_RIGHT_STATE:
		sprite->position() += glm::vec2(1, -1);
		if (collision()) {
			sprite->position() += glm::vec2(-1, 0.5);
		}
		else {
			sprite->changeAnimation(STOPPING_CLIMB_RIGHT);
			state = STOPPING_CLIMB_RIGHT_STATE;
		}
		break;
	case STOPPING_CLIMB_LEFT_STATE:
		if (sprite->animation() == STOPPING_CLIMB_LEFT && sprite->currentKeyFrame() == 7) {
			sprite->changeAnimation(WALKING_LEFT);
			state = WALKING_LEFT_STATE;
		}
		break;
	case STOPPING_CLIMB_RIGHT_STATE:
		if (sprite->animation() == STOPPING_CLIMB_RIGHT && sprite->currentKeyFrame() == 7) {
			sprite->changeAnimation(WALKING_RIGHT);
			state = WALKING_RIGHT_STATE;
		}
		break;
	case BUILDING_LEFT_STATE:
		sprite->position() += glm::vec2(-3, -1);
		if (numberOfStairs == 11) {
			sprite->position() -= glm::vec2(-3, -1);
			sprite->changeAnimation(WALKING_LEFT);
			state = WALKING_LEFT_STATE;
		}
		else if (collision()) {
			sprite->position() -= glm::vec2(-3, -1);
			sprite->changeAnimation(WALKING_RIGHT);
			state = WALKING_RIGHT_STATE;
		}
		else {
			sprite->position() -= glm::vec2(-3, -1);
			if (sprite->currentKeyFrame() == 0) {
				sprite->position() += glm::vec2(-2.0f, -1.0f);
				numberOfStairs++;
			}
		}
		break;
	case BUILDING_RIGHT_STATE:
		sprite->position() += glm::vec2(3, -1);
		if (numberOfStairs == 11) {
			sprite->position() -= glm::vec2(3, -1);
			sprite->changeAnimation(WALKING_RIGHT);
			state = WALKING_RIGHT_STATE;
		}
		else if (collision()) {
			sprite->position() -= glm::vec2(3, -1);
			sprite->changeAnimation(WALKING_LEFT);
			state = WALKING_LEFT_STATE;
		}
		else {
			sprite->position() -= glm::vec2(3, -1);
			if (sprite->currentKeyFrame() == 0) {
				sprite->position() += glm::vec2(2.0f, -1.0f);
				numberOfStairs++;
			}
		}
		break;
	case FLOATING_RIGHT_STATE:
		sprite->position() += glm::vec2(0, -1);
		if (collisionTop()) {
			sprite->position() -= glm::vec2(0, -1);
		}
		currentFloating += deltaTime;
		if (currentFloating > (5 * 1000)) {
			sprite->changeAnimation(WALKING_RIGHT);
			state = WALKING_RIGHT_STATE;
			floating = false;
		}
		break;
	case FLOATING_LEFT_STATE:
		sprite->position() += glm::vec2(0, -1);
		if (collisionTop()) {
			sprite->position() -= glm::vec2(0, -1);
		}
		currentFloating += deltaTime;
		if (currentFloating > (5 * 1000)) {
			sprite->changeAnimation(WALKING_LEFT);
			state = WALKING_LEFT_STATE;
			floating = false;
		}
		break;
	}
	return true;
}

int Lemming::collisionStairRight() {
	int ret = -2;
	for (unsigned int i = 0; i < nStairs; i++) {
		glm::vec2 lemmingPosBase = sprite->position();
		lemmingPosBase += glm::ivec2(7, 16);
		glm::vec2 stairPos = stairsPos[i].first.getPosition();
		if (stairsPos[i].second == 1) {
			if (lemmingPosBase.x == stairPos.x && lemmingPosBase.y == stairPos.y) {
				ret = 0;
				return ret;
			}
			else if ((lemmingPosBase.y + 1) == stairPos.y) {
				if ((lemmingPosBase.x == stairPos.x) || (lemmingPosBase.x == (stairPos.x + 1))) {
					ret = 1;
					return ret;
				}
			}
		}
		else {
			if ((lemmingPosBase.y + 1) == stairPos.y) {
				if ((lemmingPosBase.x == stairPos.x) || (lemmingPosBase.x == (stairPos.x - 1))) {
					ret = 1;
					return ret;
				}
			}
			else if ((lemmingPosBase.y + 2) == stairPos.y) {
				if ((lemmingPosBase.x == stairPos.x) || (lemmingPosBase.x == (stairPos.x - 1))) {
					ret = 2;
					return ret;
				}
			}
		}
	}
	return ret;
}

int Lemming::collisionStairLeft() {
	int ret = -2;
	for (unsigned int i = 0; i < nStairs; i++) {
		glm::vec2 lemmingPosBase = sprite->position();
		lemmingPosBase += glm::ivec2(6, 16);
		glm::vec2 stairPos = stairsPos[i].first.getPosition();
		if (stairsPos[i].second == 0) {
			if (lemmingPosBase.x == stairPos.x && lemmingPosBase.y == stairPos.y) {
				ret = 0;
				return ret;
			}
			else if ((lemmingPosBase.y + 1) == stairPos.y) {
				if ((lemmingPosBase.x == stairPos.x) || (lemmingPosBase.x == (stairPos.x - 1))) {
					ret = 1;
					return ret;
				}
			}
		}
		else {
			if ((lemmingPosBase.y + 1) == stairPos.y) {
				if ((lemmingPosBase.x == stairPos.x) || (lemmingPosBase.x == (stairPos.x + 1))) {
					ret = 1;
					return ret;
				}
			}
			else if ((lemmingPosBase.y + 2) == stairPos.y) {
				if ((lemmingPosBase.x == stairPos.x) || (lemmingPosBase.x == (stairPos.x + 1))) {
					ret = 2;
					return ret;
				}
			}
		}
	}
	return ret;
}

void Lemming::render()
{
	sprite->render();
}

glm::vec2 Lemming::position() {
	return sprite->position();
}

bool Lemming::floats() {
	return floating;
}

void Lemming::setPosition(glm::vec2 position) {
	sprite->setPosition(position);
}

bool Lemming::moveFloatingLemming(int mouseX, int mouseY)
{
	int x = mouseX / 3.f - 2.f;
	int y = mouseY / 3.f - 2.f;
	glm::vec2 move = glm::vec2(0, 0);
	glm::vec2 position = sprite->position();
	position += glm::vec2(7, 7);
	bool moved = false;
	if ((x + 15) > position.x && (x - 15) < position.x) {
		if ((y + 15) > position.y && (y - 15) < position.y) {
			moved = true;
			sprite->position() += glm::vec2(-4, 0);
			if (x > position.x && !collision()) move += glm::vec2(-1, 0);
			sprite->position() += glm::vec2(7, 0);
			if (x < position.x && !collision()) move += glm::vec2(1, 0);
			sprite->position() += glm::vec2(-3, 0);

			sprite->position() += glm::vec2(0, -2);
			if (y > position.y && !collisionTop()) move += glm::vec2(0, -1);
			sprite->position() += glm::vec2(0, 2);
			int fall = collisionFloor(3);
			if (y < position.y && fall > 2) move += glm::vec2(0, 1);

			position -= glm::vec2(7, 7);
			setPosition(position + move);
		}
	}
	return moved;
}

void Lemming::setMapMask(VariableTexture *mapMask, VariableTexture *lemmingMask)
{
	mask = mapMask;
	parados = lemmingMask;
}

void Lemming::setStairs(vector<pair<Stairs, bool>> stairs, int n)
{
	stairsPos = stairs;
	nStairs = n;
}

bool Lemming::eliminar() 
{
	return eliminado;
}

bool Lemming::collisionTop() {
	glm::ivec2 posBase = sprite->position() + glm::vec2(displacement, 0); // Add the map displacement

	posBase += glm::ivec2(7, 7);
	if ((mask->pixel(posBase.x, posBase.y) == 0) && (mask->pixel(posBase.x + 1, posBase.y) == 0))
		return false;

	return true;
}

void Lemming::setComeOut(bool b) {
	come_Out = b;
	if (b) {
		sprite->changeAnimation(COMING_OUT);
		state = COMING_OUT_STATE;
	}
}

void Lemming::displace(float d)
{
	glm::vec2 pos = sprite->position();
	pos.x += d;
	sprite->setPosition(pos);
	displacement -= d;
}

bool Lemming::goOut() {
	return come_Out;
}

pair<bool, int> Lemming::putStair() {
	pair<bool, int> ret;
	if (state == BUILDING_LEFT_STATE && sprite->currentKeyFrame() == 9) {
		ret.first = true;
		ret.second = 0;
	}
	else if (state == BUILDING_RIGHT_STATE && sprite->currentKeyFrame() == 9) {
		ret.first = true;
		ret.second = 1;
	}
	return ret;
}

int Lemming::collisionFloor(int maxFall)
{
	bool bContact = false;
	int fall = 0;
	glm::ivec2 posBase = sprite->position() + glm::vec2(displacement, 0); // Add the map displacement
	
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
	glm::ivec2 posBase = sprite->position() + glm::vec2(displacement, 0); // Add the map displacement
	
	posBase += glm::ivec2(7, 15);
	if((mask->pixel(posBase.x, posBase.y) == 0) && (mask->pixel(posBase.x+1, posBase.y) == 0))
		return false;

	return true;
}

bool Lemming::collisionBasher()
{
	glm::ivec2 posBase = sprite->position() + glm::vec2(displacement, 0); // Add the map displacement

	posBase += glm::ivec2(15, 8);
	for (int i = max(posBase.y - 8, 0); i <= min(mask->height() - 1, posBase.y + 7); i++) {
		if (mask->pixel(posBase.x, i) == 255)
			return true;
	}
	return false;
}

bool Lemming::hayParado()
{
	glm::ivec2 posBase = sprite->position() + glm::vec2(displacement, 0); // Add the map displacement

	posBase += glm::ivec2(7, 15);
	if ((parados->pixel(posBase.x, posBase.y) == 0) && (parados->pixel(posBase.x + 1, posBase.y) == 0))
		return false;

	return true;
}

void Lemming::setAbility(int ability) {
	if (state != BLOCKING_STATE) {
		if (ability == 2 && (state == WALKING_LEFT_STATE || state == WALKING_RIGHT_STATE)) {
			state = BLOCKING_STATE;
			sprite->changeAnimation(BLOCKING);
			applyMask();
		}
		else if (ability == 3) {
			state = DIG_STATE;
			sprite->changeAnimation(DIG);
		}
		else if (ability == 4) {
			if (state == WALKING_LEFT_STATE) {
				state = DIG_LEFT_STATE;
				sprite->changeAnimation(DIG_LEFT);
			}
			if (state == WALKING_RIGHT_STATE) {
				state = DIG_RIGHT_STATE;
				sprite->changeAnimation(DIG_RIGHT);
			}
		}
		else if (ability == 5) climber = true;
		else if (ability == 6) {
			if (state == WALKING_LEFT_STATE) {
				state = BUILDING_LEFT_STATE;
				sprite->changeAnimation(BUILDING_LEFT);
				numberOfStairs = 0;
			}
			else if (state == WALKING_RIGHT_STATE) {
				state = BUILDING_RIGHT_STATE;
				sprite->changeAnimation(BUILDING_RIGHT);
				numberOfStairs = 0;
			}
			else if (state == BUILDING_LEFT_STATE || state == BUILDING_RIGHT_STATE) numberOfStairs = 0;
		}
		else if (ability == 7) {
			if (state == WALKING_RIGHT_STATE) {
				sprite->changeAnimation(FLOATING_RIGHT);
				state = FLOATING_RIGHT_STATE;
				floating = true;
				currentFloating = 0;
			}
			if (state == WALKING_LEFT_STATE) {
				sprite->changeAnimation(FLOATING_LEFT);
				state = FLOATING_LEFT_STATE;
				floating = true;
				currentFloating = 0;
			}
		}
	}
	if (ability == 8 || ability == 1) {
		if(ability == 1 && state == BLOCKING_STATE) 
			eraseMask(position()[0] + displacement, position()[1], 0, 16, 0, 16, parados);
		state = EXPLODE_STATE;
		sprite->changeAnimation(EXPLODING);
	}
}

void Lemming::eraseMask(int posX, int posY, float ymin, float ymax, float xmin, float xmax, VariableTexture* mascara) {
	for (int y = max(0.f, posY+ymin); y <= min((float)mask->height() - 1, posY + ymax); y++) {
		for (int x = max(0.f, posX + xmin); x <= min((float)mask->width() - 1, posX + xmax); x++) {
			mascara->setPixel(x, y, 0);
		}
	}
}

bool Lemming::isBashing(int position) {
	if (position == 2)
		return state == DIG_RIGHT_STATE;
	if (position == 1)
		return state == DIG_LEFT_STATE;
	return false;
}

void Lemming::applyMask() {
	int posX, posY;
	posX = position()[0] + displacement;
	posY = position()[1];

	for (int y = max(0, posY); y <= min(mask->height() - 1, posY + 16); y++) {
		for (int x = max(0, posX); x <= min(mask->width() - 1, posX + 16); x++) {
			parados->setPixel(x, y, 255);
		}
	}
}








