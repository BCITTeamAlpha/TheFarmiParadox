#pragma once
#pragma once
#include "Character.h"
#include "Pickup.h"
#include "Weapon.h"
#include "AssetLoader.h"
#include "EventManager.h"
#include "PhysicsManager.h"
#include "PlayerManager.h"
#include "Weapon.h"
#include "Map.h"
#include "ISubscriber.h"
#include "EventManager.h"
#include <iostream>
#include <vector>

class PickupManager : public ISubscriber {

public:
	PickupManager(PlayerManager *playerManager, PhysicsManager *physics);
	~PickupManager();
	void updatePickup();
	void spawnPickup();
	virtual void notify(EventName eventName, Param *params) override;
	void removePickups();
private:

	PhysicsManager *physics;
	PlayerManager *playerManager;
	std::vector<Pickup*> pickupList;
	std::vector<Weapon> weapons;
};