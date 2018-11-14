#include "Player.h"

//Player* Player::me = NULL;

Player::Player()
{
	curChar = 0;
	//chars = std::vector<Character *>();
	weaps = new Inventory();
	//me = this;
}

void Player::addCharacter(Character *c)
{
	chars.push_back(c);
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

void Player::fireWeapon()
{
	chars[curChar]->controllable = false;
	weaps->useWeapon();
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
