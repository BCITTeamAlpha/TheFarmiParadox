#include "PickupManager.h"
#include "UIManager.h"
#include "TextComponent.h"

PickupManager::PickupManager(PlayerManager *playerManager, PhysicsManager *physics) {
	this->physics = physics;
	this->playerManager = playerManager;
	pickupList = std::vector<Pickup*>();
	weapons = std::vector<Weapon>();
	
	Weapon weap1 = Weapon("Pistol", 8, 100, 4, 40);
	Weapon weap2 = Weapon("Rifle", 5, 200, 6, 50);
	Weapon weap3 = Weapon("Shotgun", 3, 300, 8, 40);
	Weapon weap4 = Weapon("Rocket Launcher", 2, 200, 12, 40);
	Weapon weap5 = Weapon("Grenade", 1, 100, 14, 30);
	weapons.push_back(weap1);
	weapons.push_back(weap2);
	weapons.push_back(weap3);
	weapons.push_back(weap4);
	weapons.push_back(weap5);

	EventManager::subscribe(PICKUP_SPAWN, this);
}

// Checks if a character has tocuhed a Pickup
void PickupManager::updatePickup() {
	// Checks every Pickup
	for (int i = 0; i < pickupList.size(); i++) {
		Pickup *pickup = pickupList[i];
		boolean removed = false;

		// Checks every Player
		for (int j = 0; j < playerManager->instance->players.size() && !removed; j++) {
			Character *character = playerManager->instance->players[j]->getCurrentCharacter();

			// Checks every weapon inside the Player's Inventory
			for (int n = 0; n < playerManager->instance->players[j]->getWeapons()->_slots.size(); n++) {
				Weapon *weapon = playerManager->instance->players[j]->getWeapons()->_slots[n];

				// If pickup collides with a player and they do not have the pickup in their inventory
				if (pickup->colliding_with_player(character->get_position())) {
					if (pickup->pickedUp()->_name == weapon->_name) {

						// Adds pickup's ammo to players weapon ammo
						weapon->_charges += pickup->pickedUp()->_charges;
						pickup->renderable = NULL;
						pickupList.erase(pickupList.begin() + i);
						removed = true;
						break;

						// Else add pickup to Player Inventory and destroy pickup from map
					}
					else {
						playerManager->instance->players[j]->addItem(*pickup); pickup->renderable = NULL;
						pickupList.erase(pickupList.begin() + i);
						removed = true;
						break;
					}
				}				
			}
		}
	}
}

// Creates a Pickup in the world
void PickupManager::spawnPickup() {
	srand(time(NULL)); 
	int randomWeap = rand() % weapons.size();
	Pickup *p = new Pickup(new Weapon(weapons[randomWeap]));
	p->mass = 75;
	p->radius = 2.5f;
	glm::vec2 generatedPos = physics->genSpawnPos(p->radius);
	p->set_position(generatedPos);

	Renderable *pSkin = new Renderable();
	pSkin->z = 0;
	pSkin->model = AssetLoader::loadModel("../Models/cube.obj");
	pSkin->color = glm::vec4(0.8f, 0.6f, 0.4f, 1.0f);
	p->setRenderable(pSkin);
	pSkin->scale = glm::vec3(5.0f);
	physics->addObject(p);

	EventManager::notify(RENDERER_ADD_TO_RENDERABLES, &TypeParam<std::shared_ptr<Renderable>>(p->renderable), false);
	pickupList.push_back(p);
}


void PickupManager::notify(EventName eventName, Param *params) {
	switch (eventName) {
	case PICKUP_SPAWN: {
		spawnPickup();
		break;
	}
	default: 
		break;
	}
}