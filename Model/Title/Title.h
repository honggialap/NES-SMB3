#pragma once
#ifndef __TITLE_H__
#define __TITLE_H__

#pragma region INCLUDE
#include "../Engine/GameObject.h"
#pragma endregion

class CTitle : public CGameObject
{
public:
	CTitle(
		pGame game,
		unsigned int ID, std::string name, std::string source,
		float x, float y,
		int gx, int gy,
		unsigned int layer
	) : CGameObject(game, ID, name, source, x, y, gx, gy, layer) {};
	~CTitle() {};

public:
	virtual void Load();
	virtual void Start();
	virtual void Update(float elapsedMs);
	virtual void Render();

	virtual int IsCollidable();
	virtual int IsBlocking();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
typedef CTitle* pTitle;

#endif // !__TITLE_H__
