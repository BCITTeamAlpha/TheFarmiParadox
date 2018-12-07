#include "BulletoManager.h"


BulletoManager::BulletoManager(std::vector<Character*> playerList, std::vector<Bulleto*> bulletList, PhysicsManager *physics) {
	this->playerListPtr = playerList;
	this->bulletListPtr = bulletList;
	this->physicsManagerPointer = physics;
}

void BulletoManager::UpdateBullet() {

	
	for each (auto &bullet in bulletListPtr) { //update bullet position
		//bullet->set_position(glm::vec2(bullet->get_position().x + tempSpeed, bullet->get_position().y)); tempSpeed += 0.0001f;
		//bullet->velocity = glm::vec2(20.0f, 0.0f);
	}
	

	for (int i = 0; i < bulletListPtr.size(); i++) {	//For every bullet in the game, check if they have hit any of the players 

		Bulleto *bullet = bulletListPtr[i]; 

		for (int j = 0; j < playerListPtr.size();j++) { 

			Character *character = playerListPtr[j];

			if (bullet->colliding_with_player(character->get_position())) {
				character->health -= this->bulletDamage;
				printf("A player got hit! health remaining: %d\n", character->health);
				if (character->health < 0) {
					character->renderable = NULL;
					playerListPtr.erase(playerListPtr.begin() + j);
				}
				//std::cout << bullet->renderable.use_count() << "\n";
				bullet->renderable = NULL; //delete bullet's renderable share pointer
				bulletListPtr.erase(bulletListPtr.begin() + i);
				break;
			}

		}
	}

}


void BulletoManager::SpawnBulleto(int x, int y) {


	Bulleto *bullet = new Bulleto(10.0f);
	bullet->mass = 15;
	bullet->radius = 2.0f;
	bullet->set_position(glm::vec2(10, 10)); 	//bullet->set_position(physics->genSpawnPos(bullet->radius));

	bullet->velocity = glm::vec2(40.0f, 10.0f);

	Renderable *pSkin = new Renderable();
	pSkin->z = 0;
	pSkin->model = AssetLoader::loadModel("../Models/cube.obj");
	pSkin->color = glm::vec4(0.8f, 0.6f, 0.4f, 1.0f);
	bullet->setRenderable(pSkin);
	pSkin->scale = glm::vec3(5);
	physicsManagerPointer->addObject(bullet);
	bulletListPtr.push_back(bullet);
	//std::cout << bullet->get_position().x << " y: " << bullet->get_position().y << std::endl;

	EventManager::notify(RENDERER_ADD_TO_RENDERABLES, &TypeParam<std::shared_ptr<Renderable>>(bullet->renderable), false);
}