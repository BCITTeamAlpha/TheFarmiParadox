#include "Player.h"

Player* Player::me = NULL;

Player::Player()
{
	curChar = 0;
	chars = std::vector<Character>();
	weaps = new Inventory();
	me = this;
}

void Player::addCharacter(Character *c)
{
	chars.push_back(*c);
}

void Player::addItem(Pickup item)
{
	weaps->addItem(item);
}

void Player::prevWeapon()
{
	me->weaps->selectSlot(me->weaps->currentSlot - 1);
}

void Player::nextWeapon()
{
	me->weaps->selectSlot(me->weaps->currentSlot + 1);
}

void Player::fireWeapon()
{
	me->weaps->useWeapon();
}