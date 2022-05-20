#pragma once
#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#pragma region FORWARD DECLARATION
class CGame;
typedef CGame* pGame;
#pragma endregion

#pragma region INCLUDE
#include "Sprite.h"
#include "Animation.h"
#include "Sound.h"

#include "pugixml.hpp"
#include <string>
#include <unordered_map>
#pragma endregion

class CGameObject 
{
protected:
	pGame _game = nullptr;

	unsigned int _ID = 0;
	std::string _name;
	std::string _source;

	bool _start = false;
	bool _destroy = false;

	float _x = 0;
	float _y = 0;

	float _vx = 0;
	float _vy = 0;

	int _gx = 0;
	int _gy = 0;

	unsigned int _layer = 0;

	std::unordered_map<unsigned int, pSprite> _sprites;
	std::unordered_map<unsigned int, pAnimation> _animations;
	std::unordered_map<unsigned int, pSound> _sounds;

public:
	CGameObject(
		pGame game,
		unsigned int ID, std::string name, std::string source,
		float x, float y,
		int gx, int gy,
		unsigned int layer
	);
	~CGameObject();

	pGame GetGame() { return _game; }
	unsigned int GetID() { return _ID; }
	std::string GetName() { return _name; }

	bool IsDestroyed() { return _destroy; }

	virtual void Load();
	virtual void Start() { _start = true; }
	virtual void Update(float elapsedMs) = 0;
	virtual void Render() = 0;
	void Destroy() { _destroy = true; }

	void SetPosition(float x, float y) { _x = x; _y = y; }
	void GetPosition(float& x, float& y) { x = _x; y = _y; }

	void SetVelocity(float vx, float vy) { _vx = vx; _vy = vy; }
	void GetVelocity(float& vx, float& vy) { vx = _vx; vy = _vy; }

	void GetGrid(int& gx, int& gy) { gx = _gx; gy = _gy; }
	void SetGrid(int gx, int gy) { _gx = gx; _gy = gy; }

	unsigned int GetLayer() { return _layer; }
	void SetLayer(unsigned int layer) { _layer = layer; }
	static bool CompareLayer(CGameObject* a, CGameObject* b) { return a->_layer < b->_layer; }

	void AddSprite(unsigned int ID, pSprite sprite);
	pSprite GetSprite(unsigned int ID) { return _sprites[ID]; }

	void AddAnimation(unsigned int id, pAnimation animation);
	pAnimation GetAnimation(unsigned int id) { return _animations[id]; }

	void AddSound(unsigned int id, pSound sound);
	pSound GetSound(unsigned int id) { return _sounds[id]; }
};
typedef CGameObject* pGameObject;

#endif // !__GAME_OBJECT_H__
