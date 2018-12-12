#include "PlayerManager.h"
#include "UIManager.h"
#include "TextComponent.h"

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
	instance->timeElapsed = 0;
}

void PlayerManager::fireWeapon()
{
	instance->turnStage = 2;
	instance->players[instance->currentPlayerIndex]->fireWeapon();
	instance->NextPlayer();
}

int PlayerManager::SecondsRemaining()
{
	switch (turnStage)
	{
	case 0:
		return (int)std::round(moveTime - timeElapsed);
	case 1:
		return (int)std::round(aimTime - timeElapsed);
	default:
		return 0;
	}
}

int PlayerManager::handlePlayers(float dTime)
{
	timeElapsed += dTime;

	//printf("Time Remaining:%d\n", SecondsRemaining());

	if (turnStage == 0 && timeElapsed >= moveTime)
	{
		turnStage = 1;
		players[currentPlayerIndex]->setControllable(false);
		timeElapsed = 0;
		instance->NextPlayer();
	}
	else if (turnStage == 1)
	{
		if (timeElapsed >= aimTime)
			NextPlayer();
		else
			players[currentPlayerIndex]->adjustAim(dTime);
	}

	for (int i = 0; i < players.size(); i++)
	{
		//if players[i] has no more characters delete it
	}

	//if there is only one player left, return the winning player's id

	return -1;
}

void PlayerManager::NextPlayer() 
{
	turnStage = 0;
	timeElapsed = 0;
	players[currentPlayerIndex]->setNextCharacter();

	//players[currentPlayerIndex]->nextChar();
	players[currentPlayerIndex]->clearInput();
	currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
	
	printf("next player index:%d\n", currentPlayerIndex);

	players[currentPlayerIndex]->getCurrentCharacter()->bulletoAmmo = players[currentPlayerIndex]->getCurrentCharacter()->maxBulletsPerTurn;

	UpdatePlayerUI();
}

void PlayerManager::AddPlayer(Player * player)
{
	players.push_back(player); 
}

void PlayerManager::RemovePlayer(int playerID)
{

	for (int i = 0; i < players.size(); i++)
	{	
		if (players[i]->playerID == playerID)
		{
			players.erase(players.begin() + i);

			if (i == currentPlayerIndex)
			{
				currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
			}
			else if (i < currentPlayerIndex)
			{
				//p1(index 0) p2(index 1) p3(index 2) p4(index 3) --> if we delete p2, while the current player is p3, then currentIndex should be 2
				//However, after p2 has been deleted, an index of 2 will point at p4, so we need to decrement by 1.
				currentPlayerIndex--;
			}
			
			if(currentPlayerIndex >= players.size() || currentPlayerIndex < 0)
				currentPlayerIndex = 0; //dont want to access out of bounds element

			break;
		}
	}
}

Player* PlayerManager::GetCurrentPlayer() {
	return players[currentPlayerIndex];
}

void PlayerManager::UpdatePlayerUI() {

	if (players[currentPlayerIndex] != NULL) { //update ui with info pertaining to whose turn it is, the team they belong to, and their hp
		std::string info = "";

		info += "P:";
		info += std::to_string(players[currentPlayerIndex]->playerID);
		info += "C:";
		info += std::to_string(players[currentPlayerIndex]->getCurrentCharacter()->characterID);
		info += " HP:";
		info += std::to_string(players[currentPlayerIndex]->getCurrentCharacter()->health);

		TextComponent *topRightInfo = dynamic_cast<TextComponent*>(UIManager::GetComponentById("rText"));
		if (topRightInfo != nullptr) {
			topRightInfo->SetText(info);
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