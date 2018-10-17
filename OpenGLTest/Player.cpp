#include "Player.h"

Player::Player()
{
	curChar = 0;
	chars = std::vector<Character>();
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
	weaps->selectSlot(weaps->currentSlot - 1);
}

void Player::nextWeapon()
{

}

void Player::fireWeapon()
{

}