#include "BulletoManager.h"


BulletoManager::BulletoManager(std::vector<Character*> playerList, std::vector<Bulleto*> bulletList, PhysicsManager *physics) {
	this->playerListPtr = playerList;
	this->bulletListPtr = bulletList;
	this->physicsManagerPointer = physics;
}

void BulletoManager::UpdateBullet() {


	for each (auto &bullet in bulletListPtr) { //update bullet position
		bullet->set_position(glm::vec2(bullet->get_position().x + tempSpeed, bullet->get_position().y)); tempSpeed += 0.00001f;
	}
	

	for (int i = 0; i < bulletListPtr.size(); i++) {	//for each bullet

		Bulleto *bullet = bulletListPtr[i]; //get the bullet object from pointer

		for (int j = 0; j < playerListPtr.size();j++) { //look at every player and see if the bullet comes close

			Character *character = playerListPtr[j];

			if (bullet->colliding_with_player(character->get_position())) {
				character->health -= this->bulletDamage;
				printf("A player got hit! health remaining: %d\n", character->health);
				//std::cout << bullet->renderable.use_count() << "\n";
				bullet->renderable = NULL; //delete bullet's renderable share pointer
				bulletListPtr.erase(bulletListPtr.begin() + i);
				tempSpeed = 0.0005f;
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