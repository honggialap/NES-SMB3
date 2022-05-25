#pragma once
#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__

#pragma region INCLUDE
#include "../Engine/GameObject.h"
#pragma endregion

#pragma region COMPONENT ID
#pragma endregion

class CBackground : public CGameObject {
public:
	CBackground(
		pGame game,
		unsigned int id, std::string name, std::string source,
		float x, float y, int gx, int gy, unsigned int layer
	) : CGameObject(game, id, name, source, x, y, gx, gy, layer) {};

	/* Tiled Map */
	unsigned int _mapWidth = 0;
	unsigned int _mapHeight = 0;
	float _cellWidth = 0;
	float _cellHeight = 0;
	std::vector<unsigned int> _cells;

public:
	virtual void Load();
	virtual void Start();
	virtual void Update(float elapsedMs);
	virtual void Render();

	virtual int IsCollidable();
	virtual int IsBlocking();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
};
typedef CBackground* pBackground;

#endif // !__BACKGROUND_H__
