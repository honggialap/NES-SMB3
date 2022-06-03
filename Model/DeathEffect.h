#pragma once
#ifndef __DEATH_EFFECT_H__
#define __DEATH_EFFECT_H__

#pragma region INCLUDE
#include "Engine/GameObject.h"
#pragma endregion

#pragma region DEFINE
/* SPRITE ID */
/* ANIMATION ID */
/* SOUNDCLIP ID */
#pragma endregion

class CDeathEffect : public CGameObject
{
public:
	CDeathEffect(
		pGame game,
		unsigned int id, std::string name, std::string source,
		float x, float y, int gx, int gy, unsigned int layer
	) : CGameObject(game, id, name, source, x, y, gx, gy, layer) {};

public:
	virtual void Load();
	virtual void Start();
	virtual void Update(float elapsedMs);
	virtual void Render();

	virtual int IsCollidable();
	virtual int IsBlocking();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
typedef CDeathEffect* pDeathEffect;

#endif // !__DEATH_EFFECT_H__
