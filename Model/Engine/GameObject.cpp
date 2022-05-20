#pragma region INCLUDE
#include "GameObject.h"
#pragma endregion

CGameObject::CGameObject(
	pGame game, 
	unsigned int ID, std::string name, std::string source, 
	float x, float y, 
	unsigned int layer
)
{
	_game = game;
	_ID = ID;
	_name = name;
	_source = source;
	_x = x;
	_y = y;
	_layer = layer;
}

CGameObject::~CGameObject()
{
}

void CGameObject::Load()
{
}
