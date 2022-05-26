#pragma region INCLUDE
#include "SpawnZone.h"

#include "../SuperMarioBros3.h"
#pragma endregion

void CSpawnZone::Load()
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

	/* Type */
	pugi::xml_node spawn = prefab.child("Prefab").child("Spawn");
	SPAWN_COUNTDOWN = spawn.attribute("SPAWN_COUNTDOWN").as_float();
	_repeatable = spawn.attribute("repeatable").as_bool();
	_targetName = spawn.attribute("target").as_string();
}

void CSpawnZone::Start()
{
	CGameObject::Start();
	_spawnCountdown = SPAWN_COUNTDOWN;
	_spawned = false;
}

void CSpawnZone::Update(float elapsedMs)
{
	if (!_start) Start();

	_triggered = false;

	std::vector<pGameObject> collidables = _game->GetLocal(_id);
	_collider->Process(elapsedMs, &collidables);

	if (_spawned) return;

	if (_spawnCountdown > 0) _spawnCountdown -= elapsedMs;
	else
	{
		if (!_triggered) return;
		Spawn();
		_spawned = true;

		if (!_repeatable) return;
		_spawnCountdown = SPAWN_COUNTDOWN;
		_spawned = false;
	}
}

void CSpawnZone::Render()
{
	if (_renderBody) _sprites[SPR_BBOX]->Render(_x, _y);
}

void CSpawnZone::Spawn()
{
	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());

	for (auto spawnable = prefab.child("Prefab").child("GameObject");
		spawnable;
		spawnable = spawnable.next_sibling("GameObject"))
	{
		if (_game->Get(_name + spawnable.attribute("name").as_string()) == nullptr)
		{
			_game->Create(
				spawnable.attribute("actor").as_int(),
				_name + spawnable.attribute("name").as_string(),
				spawnable.attribute("source").as_string(),
				spawnable.attribute("x").as_float(),
				spawnable.attribute("y").as_float(),
				spawnable.attribute("gx").as_float(),
				spawnable.attribute("gy").as_float(),
				spawnable.attribute("layer").as_uint()
			);
		}
	}
}

#pragma region COLLISION

int CSpawnZone::IsCollidable()
{
	return 1;
}

int CSpawnZone::IsBlocking()
{
	return 0;
}

void CSpawnZone::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
	right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
	top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
	bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
}

void CSpawnZone::OnNoCollision(float elapsedMs)
{
}

void CSpawnZone::OnCollisionWith(pCollision collision)
{
	if (collision->_target->GetName() == _targetName)
	{
		_triggered = true;
	}
}

#pragma endregion