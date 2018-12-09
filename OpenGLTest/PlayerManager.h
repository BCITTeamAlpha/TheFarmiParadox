#pragma once
#include "Player.h"
#include <vector>
#include "ISubscriber.h"
#include "EventManager.h"

class PlayerManager : public ISubscriber
{
public:
	static PlayerManager *instance;
	PlayerManager();
	~PlayerManager();

	static void prevWeapon();
	static void nextWeapon();
	static void aimWeapon();
	static void fireWeapon();

	void handlePlayers(float dTime);

	void NextPlayer();

	void AddPlayer(Player *player);

	void RemovePlayer(int playerID);
	Player* GetCurrentPlayer();
	void UpdatePlayerUI();

	virtual void notify(EventName eventName, Param *params) override;
	std::vector<Player*> players;

	int actionsTaken = 0;
	int maxActionsPerTurn = 5;
private:
	int currentPlayerIndex;
	size_t turnStage;

};