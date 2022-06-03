#pragma region INCLUDE
#include "PlantFireball.h"
#pragma endregion

void CPlantFireball::Load()
{
	CGameObject::Load();

	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());

	/* Body */
	pugi::xml_node body = prefab.child("Prefab").child("Body");
	_renderBody = body.attribute("render").as_bool();
	BODY_WIDTH = body.attribute("width").as_float();
	BODY_HEIGHT = body.attribute("height").as_float();
	BODY_OFFSETX = body.attribute("offsetX").as_float();
	BODY_OFFSETY = body.attribute("offsetY").as_float();

	/* LifeSpan */
	pugi::xml_node lifeSpan = prefab.child("Prefab").child("LifeSpan");
	DECAY_INTERVAL = lifeSpan.attribute("DECAY_INTERVAL").as_float();
}

void CPlantFireball::Start()
{
	CGameObject::Start();
	_decayInterval = DECAY_INTERVAL;
	_animations[ANI_PLANT_FIREBALL_LEFT]->Play(true);
	_animations[ANI_PLANT_FIREBALL_RIGHT]->Play(true);
}

void CPlantFireball::Update(float elapsedMs)
{
	if (!_start) Start();

	if (_decayInterval > 0)
	{
		_decayInterval -= elapsedMs;

		_animations[ANI_PLANT_FIREBALL_LEFT]->Update(elapsedMs);
		_animations[ANI_PLANT_FIREBALL_RIGHT]->Update(elapsedMs);

		_x += _vx * elapsedMs;
		_y += _vy * elapsedMs;
	}
	else
	{
		_animations[ANI_PLANT_FIREBALL_LEFT]->Stop();
		_animations[ANI_PLANT_FIREBALL_RIGHT]->Stop();

		Destroy();
	}
}

void CPlantFireball::Render()
{
	if (_renderBody) _sprites[SPR_BBOX]->Render(_x, _y);
	if (_vx < 0) _animations[ANI_PLANT_FIREBALL_LEFT]->Render(_x, _y);
	else _animations[ANI_PLANT_FIREBALL_RIGHT]->Render(_x, _y);
}

int CPlantFireball::IsCollidable()
{
	return 1;
}

int CPlantFireball::IsBlocking()
{
	return 0;
}

void CPlantFireball::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
	right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
	top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
	bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
}