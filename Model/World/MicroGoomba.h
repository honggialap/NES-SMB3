#pragma once
#ifndef __MICRO_GOOMBA_H__
#define __MICRO_GOOMBA_H__

#pragma region INCLUDE
#include "../Engine/GameObject.h"
#pragma endregion

#pragma region COMPONENT ID
#pragma endregion

class CMicroGoomba : public CGameObject {
public:
	CMicroGoomba(
		pGame game,
		unsigned int ID, std::string name, std::string source,
		float x, float y, int gx, int gy, unsigned int layer
	) : CGameObject(game, ID, name, source, x, y, gx, gy, layer) {};

public:
	virtual void Load();
	virtual void Start();
	virtual void Update(float elapsedMs);
	virtual void Render();

	virtual int IsCollidable();
	virtual int IsBlocking();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
typedef CMicroGoomba* pMicroGoomba;

#endif // !__MICRO_GOOMBA_H__
