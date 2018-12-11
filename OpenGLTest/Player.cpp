#include "Player.h"

int Player::PLAYER_COUNT = 0;

Player::Player()
{
	PLAYER_COUNT++;
	playerID = PLAYER_COUNT; //assign a player id to each player
	curChar = 0;
	//chars = std::vector<Character *>();
	weaps = new Inventory();
}

void Player::addCharacter(Character *c)
{
	chars.push_back(c);
}

Character* Player::getFirstCharacter() {
	return chars[0];
}

void Player::addItem(Pickup item)
{
	weaps->addItem(item);
}

void Player::prevWeapon()
{
	weaps->selectSlot(weaps->currentSlot - 1);
}

void Player::nextWeapon()
{
	weaps->selectSlot(weaps->currentSlot + 1);
}

void Player::setControllable(bool c)
{
	chars[curChar]->controllable = c;
}

void Player::fireWeapon()
{
	weaps->useWeapon(chars[curChar]->get_position());
}

void Player::setAimLeft(bool b)
{
	aimLeft = b;
}

void Player::setAimRight(bool b)
{
	aimRight = b;
}

void Player::adjustAim(float dTime)
{
	float dAngle;

	if (aimLeft && !aimRight)
		dAngle = -dTime;
	else if (!aimLeft && aimRight)
		dAngle = dTime;
	else
		dAngle = 0;

	weaps->angleWeapon(dAngle);
}

void Player::clearInput()
{
	for (auto &c : chars)
	{
		c->left_input = false;
		c->right_input = false;
		c->jump_input= false;
	}
}

void Player::moveLeft(bool v)
{
	for (auto &c : chars)
	{
		c->left_input = v;
	}
}

void Player::moveRight(bool v)
{
	for (auto &c : chars)
	{
		c->right_input = v;
	}
}

void Player::jump(bool v)
{
	for (auto &c : chars)
	{
		c->jump_input = v;
	}
}
