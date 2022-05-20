#pragma once
#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__

#pragma region FORWARD DECLARATION
class CGame;
typedef CGame* pGame;
#pragma endregion

#pragma region INCLUDE
#include <string>
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

public:
	CGameObject(
		pGame game,
		unsigned int ID, std::string name, std::string source,
		float x, float y,
		int gx, int gy,
		unsigned int layer
	);
	~CGameObject();

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
};
typedef CGameObject* pGameObject;

#endif // !__GAME_OBJECT_H__
