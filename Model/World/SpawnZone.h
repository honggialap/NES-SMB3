#pragma once
#ifndef __SPAWN_ZONE_H__
#define __SPAWN_ZONE_H__

#pragma region INCLUDE
#include "../Engine/GameObject.h"
#pragma endregion

#pragma region DEFINE
/* SPRITE ID */
#define SPR_BBOX				999901
#pragma endregion

class CSpawnZone : public CGameObject
{
public:
	CSpawnZone(
		pGame game,
		unsigned int id, std::string name, std::string source,
		float x, float y, int gx, int gy, unsigned int layer
	) : CGameObject(game, id, name, source, x, y, gx, gy, layer) {};

public:
	virtual void Load();
	virtual void Start();
	virtual void Update(float elapsedMs);
	virtual void Render();

#pragma region LOGIC

	/* Body */
	bool _renderBody = false;
	float BODY_WIDTH = 0;
	float BODY_HEIGHT = 0;
	float BODY_OFFSETX = 0;
	float BODY_OFFSETY = 0;

	/* Spawn */
	float SPAWN_COUNTDOWN = 0;
	float _spawnCountdown = 0;
	bool _repeatable = false;
	std::string _targetName;
	bool _triggered = false;
	bool _spawned = false;

	void Spawn();

#pragma endregion

#pragma region COLLISION

	virtual int IsCollidable();
	virtual int IsBlocking();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void OnNoCollision(float elapsedMs);
	void OnCollisionWith(pCollision collision);

#pragma endregion
};
typedef CSpawnZone* pSpawnZone;

#endif // !__SPAWN_ZONE_H__
