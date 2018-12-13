#include "PlayerManager.h"
#include "UIManager.h"
#include "TextComponent.h"
#define _CRT_SECURE_NO_WARNINGS

PlayerManager * PlayerManager::instance = NULL;

PlayerManager::PlayerManager()
{
	instance = this;
	currentPlayerIndex = 0;
	turnStage = 0; //moving, aiming, firing
	timeElapsed = 0;

    EventManager::subscribe(PLAYER_LEFT, this);
    EventManager::subscribe(PLAYER_RIGHT, this);
    EventManager::subscribe(PLAYER_JUMP, this);
    EventManager::subscribe(AIM_LEFT, this);
    EventManager::subscribe(AIM_RIGHT, this);
    EventManager::subscribe(PLAYER_FIRE, this);
}


PlayerManager::~PlayerManager()
{
	EventManager::unsubscribe(PLAYER_LEFT, this);
	EventManager::unsubscribe(PLAYER_RIGHT, this);
	EventManager::unsubscribe(PLAYER_JUMP, this);
	EventManager::unsubscribe(AIM_LEFT, this);
	EventManager::unsubscribe(AIM_RIGHT, this);
	EventManager::unsubscribe(PLAYER_FIRE, this);
}

void PlayerManager::prevWeapon()
{
	instance->players[instance->currentPlayerIndex]->prevWeapon();
}

void PlayerManager::nextWeapon()
{
	instance->players[instance->currentPlayerIndex]->nextWeapon();
}

void PlayerManager::aimWeapon()
{
	instance->turnStage = 1;
	instance->players[instance->currentPlayerIndex]->setControllable(false);
	instance->players[instance->currentPlayerIndex]->aim_angle = 0.0f;
	instance->timeElapsed = 0;
}

void PlayerManager::fireWeapon()
{
	instance->turnStage = 2;
	instance->players[instance->currentPlayerIndex]->fireWeapon();
	//instance->NextPlayer();  moved this into denny code (hackjobbulletmanager.cpp) when the bullet makes contact with terrain or another player
}

float PlayerManager::SecondsRemaining()
{
	switch (turnStage)
	{
	case 0:
		return moveTime - timeElapsed;
	case 1:
		return aimTime - timeElapsed;
	default:
		return 0;
	}
}

int PlayerManager::handlePlayers(float dTime)
{
	timeElapsed += dTime;

	for (int i = 0; i < players.size(); i++)
	{
		if (players[i]->chars.size() == 0)
			RemovePlayer(players[i]->playerID);
	}

	if (players.size() == 1)
	{
		players[0]->RemoveCharacter(0);
		int id = players[0]->playerID;
		RemovePlayer(id);
		return id;
	}

	if (turnStage == 0 && timeElapsed >= moveTime)
	{
		turnStage = 1;
		players[currentPlayerIndex]->setControllable(false);
		timeElapsed = 0;
	}
	else if (turnStage == 1)
	{
		if (timeElapsed >= aimTime)
			NextPlayer();
		else
			players[currentPlayerIndex]->adjustAim(dTime);
	}

	return -1;
}

void PlayerManager::NextPlayer() 
{
	for (int i = 0; i < players.size(); i++)
	{
		if (players[i]->chars.size() == 0)
			RemovePlayer(players[i]->playerID);
	}

	if (players.size() == 1)
		return;

	turnStage = 0;
	timeElapsed = 0;

	players[currentPlayerIndex]->setControllable(false);

	players[currentPlayerIndex]->clearInput();
	currentPlayerIndex = (currentPlayerIndex + 1) % players.size();

	players[currentPlayerIndex]->setNextCharacter();
	
	printf("next player index:%d\n", currentPlayerIndex);

	players[currentPlayerIndex]->getCurrentCharacter()->bulletoAmmo = players[currentPlayerIndex]->getCurrentCharacter()->maxBulletsPerTurn;

    UIComponent *component = UIManager::GetComponentById("tBox");
    if (component != nullptr)
        component->color = players[currentPlayerIndex]->color;

    component = UIManager::GetComponentById("detail");
    if (component != nullptr)
        component->color = players[currentPlayerIndex]->accent;

    component = UIManager::GetComponentById("detailInner");
    if (component != nullptr)
        component->color = players[currentPlayerIndex]->color;

    component = UIManager::GetComponentById("rightWeapon");
    if (component != nullptr)
        component->color = players[currentPlayerIndex]->color;

    component = UIManager::GetComponentById("leftWeapon");
    if (component != nullptr)
        component->color = players[currentPlayerIndex]->color;

    for (int i = 1; i <= 6; i++) {
        component = UIManager::GetComponentById("c" + std::to_string(i) + "Container");
        if (component != nullptr)
            component->visible = (i <= players[currentPlayerIndex]->chars.size());
    }

    EventManager::notify(PICKUP_SPAWN, nullptr);
}

void PlayerManager::AddPlayer(Player * player)
{
	players.push_back(player); 
}

void PlayerManager::RemovePlayer(int playerID) {
	for (int i = 0; i < players.size(); i++) {	
		if (players[i]->playerID == playerID) {
			players.erase(players.begin() + i);
			if (players.size() > 0) {
				currentPlayerIndex = currentPlayerIndex % players.size();
			}
			break;
		}
	}
}

Player* PlayerManager::GetCurrentPlayer() {
	return players[currentPlayerIndex];
}

void PlayerManager::UpdatePlayerUI() {
    TextComponent *text = dynamic_cast<TextComponent*>(UIManager::GetComponentById("timerText"));
    if (text != nullptr)
        text->SetText(getTimeString());

    UIComponent *hpBar, *marker;
    for (Player *p : players) {
        marker = UIManager::GetComponentById("p" + std::to_string(p->playerID) + "Arrow");
        if (marker != nullptr)
            marker->visible = (GetCurrentPlayer() == p);

        hpBar = UIManager::GetComponentById("p" + std::to_string(p->playerID) + "Health");
        if (hpBar != nullptr) {
            float total = 0;
            for (Character *c : p->chars)
                total += c->health;

            hpBar->size.x = total / (10 * charPerPlayer);
            hpBar->valid = false;
        }
    }

	if (players[currentPlayerIndex] != NULL) { //update ui with info pertaining to whose turn it is, the team they belong to, and their hp
        Player *p = players[currentPlayerIndex];
        Weapon *currWeapon = p->getWeapons()->curWeapon();

        text = dynamic_cast<TextComponent*>(UIManager::GetComponentById("currPlayer"));
        if (text != nullptr)
            text->SetText("Player " + std::to_string(p->playerID) + "'s Turn");
       
        text = dynamic_cast<TextComponent*>(UIManager::GetComponentById("ammoCount"));
		if (text != nullptr)
		{
			if (currWeapon != nullptr)
				text->SetText(std::to_string(currWeapon->_charges));
			else
				text->SetText(std::to_string(0));
		}

        text = dynamic_cast<TextComponent*>(UIManager::GetComponentById("currWeapon"));
		if (text != nullptr)
		{
			if (currWeapon != nullptr)
				text->SetText(currWeapon->_name);
			else
				text->SetText("No Weapon");
		}

        for (int i = 0; i < p->chars.size(); i++) {
            Character *c = p->chars[i];
            if (c != nullptr) {
                hpBar = UIManager::GetComponentById("c" + std::to_string(i + 1) + "Health");
                if (hpBar != nullptr) {
                    hpBar->size.x = c->health / 10.0f;
                    hpBar->valid = false;
                }

                marker = UIManager::GetComponentById("c" + std::to_string(i + 1) + "Arrow");
                if (marker != nullptr)
                    marker->visible = (c == p->getCurrentCharacter());
            }
        }
    }
}


void PlayerManager::notify(EventName eventName, Param *params)
{
	if (!players.size()) return;

	switch (eventName) {
	case PLAYER_LEFT: {
		TypeParam<bool> *p = dynamic_cast<TypeParam<bool> *>(params); // Safetly cast generic param pointer to a specific type
		if (p != nullptr && turnStage == 0) 
			players[currentPlayerIndex]->moveLeft(p->Param);
		
		break;
	}
	case PLAYER_RIGHT: {
		TypeParam<bool> *p = dynamic_cast<TypeParam<bool> *>(params); // Safetly cast generic param pointer to a specific type
		if (p != nullptr && turnStage == 0) 
			players[currentPlayerIndex]->moveRight(p->Param);
		
		break;
	}
	case PLAYER_JUMP: {
		TypeParam<bool> *p = dynamic_cast<TypeParam<bool> *>(params); // Safetly cast generic param pointer to a specific type
		if (p != nullptr) 
			players[currentPlayerIndex]->jump(p->Param);
		break;
	}
	case AIM_LEFT: {
		TypeParam<bool> *p = dynamic_cast<TypeParam<bool> *>(params); // Safetly cast generic param pointer to a specific type
        if (p != nullptr && turnStage == 1) 
			players[currentPlayerIndex]->setAimLeft(p->Param);
        
		break;
	}
	case AIM_RIGHT: {
		TypeParam<bool> *p = dynamic_cast<TypeParam<bool> *>(params); // Safetly cast generic param pointer to a specific type
        if (p != nullptr && turnStage == 1)  
			players[currentPlayerIndex]->setAimRight(p->Param);
		break;
	}
	case PLAYER_FIRE: {
		if (turnStage == 0)
			aimWeapon();
		else if (turnStage == 1)
			fireWeapon();
		break;
	}
	default:
		break;
	}

}

std::string PlayerManager::getTimeString() {
    float time = SecondsRemaining();

    int seconds = (int)time;
    float msTemp = (time - seconds) * 100;
    int ms = (int)msTemp;

    char buff[6];
    sprintf_s(buff, "%02d:%02d", seconds, ms);
    
    std::string ret(buff);

    return ret;
}