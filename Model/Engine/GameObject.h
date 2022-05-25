#pragma once
#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#pragma region FORWARD DECLARATION
class CGame;
typedef CGame* pGame;
struct CScene;
typedef CScene* pScene;
#pragma endregion

#pragma region INCLUDE
#include "Component/Sprite.h"
#include "Component/Animation.h"
#include "Framework/Audio.h"
#include "Component/Collision.h"
#include "Framework/pugixml.hpp"
#include <string>
#pragma endregion

class CGameObject
{
public:
	CGameObject(
		pGame game, pScene scene,
		unsigned int id, std::string name, std::string source,
		float x, float y, int gx, int gy, unsigned int layer
	);
	~CGameObject();

	/* Identifier */
protected:
	pGame _game = nullptr;
	pScene _scene = nullptr;
	unsigned int _id = 0;
	std::string _name;
	std::string _source;
	
public:
	pGame GetGame() { return _game; }
	pScene GetScene() { return _scene; }
	unsigned int GetId() { return _id; }
	std::string GetName() { return _name; }

	/* Lifecycle */
protected:
	bool _start = false;
	bool _destroy = false;

public:
	virtual void Load();
	virtual void Start() { _start = true; }
	virtual void Update(float elapsedMs) = 0;
	virtual void Render() = 0;
	void Destroy();
	bool IsDestroyed() { return _destroy; }

	/* Position */
protected:
	float _x = 0;
	float _y = 0;

public:
	void SetPosition(float x, float y) { _x = x; _y = y; }
	void GetPosition(float& x, float& y) { x = _x; y = _y; }

	/* Velocity */
protected:
	float _vx = 0;
	float _vy = 0;

public:
	void SetVelocity(float vx, float vy) { _vx = vx; _vy = vy; }
	void GetVelocity(float& vx, float& vy) { vx = _vx; vy = _vy; }

	/* Sprite */
protected:
	std::unordered_map<unsigned int, pSprite> _sprites;

public:
	void AddSprite(unsigned int id, pSprite sprite) { _sprites[id] = sprite; }
	pSprite GetSprite(unsigned int id) { return _sprites[id]; }

	/* Animation */
protected:
	std::unordered_map<unsigned int, pAnimation> _animations;

public:
	void AddAnimation(unsigned int id, pAnimation animation) { _animations[id] = animation; }
	pAnimation GetAnimation(unsigned int id) { return _animations[id]; }

	/* Sound clip */
protected:
	std::unordered_map<unsigned, pSoundClip> _soundClips;

public:
	void AddSoundClip(unsigned int id, pSoundClip soundClip) { _soundClips[id] = soundClip; }
	pSoundClip GetSoundClip(unsigned int id) { return _soundClips[id]; }

	/* Collision */
protected:
	pCollider _collider = nullptr;

public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) = 0;
	virtual int IsCollidable() { return 0; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(float elapsedMs) {};
	virtual void OnCollisionWith(pCollision e) {};
	
	/* Layer */
protected:
	unsigned int _layer = 0;

public:
	unsigned int GetLayer() { return _layer; }
	void SetLayer(unsigned int layer) { _layer = layer; }
	static bool CompareLayer(CGameObject* a, CGameObject* b) { return a->_layer < b->_layer; }

	/* Grid */
protected:
	int _gx = 0;
	int _gy = 0;

public:
	void GetGrid(int& gx, int& gy) { gx = _gx; gy = _gy; }
	void SetGrid(int gx, int gy) { _gx = gx; _gy = gy; }
};
typedef CGameObject* pGameObject;

#endif // !__GAMEOBJECT_H__
