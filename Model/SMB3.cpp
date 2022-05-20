#pragma region INCLUDE
#include "SMB3.h"

#include "World/Mario.h"
#include "World/Platform.h"

#pragma endregion

pGameObject CSMB3::Create(
	unsigned int actorID, std::string name, std::string source,
	float posX, float posY,
	int gridX, int gridY,
	unsigned int layer
) {
	switch (actorID) {

	case ACT_MARIO: {
		auto gameObject = new CMario(
			this,
			_nextGameObjectID++, name, source,
			posX, posY, gridX, gridY, layer
		);
		AddGameObject(gameObject);
		gameObject->Load();
		return gameObject;
	} break;

	case ACT_PLATFORM: {
		auto gameObject = new CPlatform(
			this,
			_nextGameObjectID++, name, source,
			posX, posY, gridX, gridY, layer
		);
		AddGameObject(gameObject);
		gameObject->Load();
		return gameObject;
	} break;

	default: {
		return nullptr;
	} break;
	}
}
