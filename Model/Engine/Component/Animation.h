#pragma once
#ifndef __ANIMATION_H__
#define __ANIMATION_H__

#pragma region FORWARD DECLARATION
class CGameObject;
typedef CGameObject* pGameObject;
#pragma endregion

#pragma region INCLUDE
#include "Sprite.h"
#pragma endregion

class CAnimation
{
public:
	CAnimation(pGameObject gameObject);
	~CAnimation();

private:
	pGameObject _gameObject;
	std::vector<std::pair<unsigned int, float>> _animationFrames;
	unsigned int _frameIndex = 0;
	float _elapsedTime = 0;
	bool _play = false;
	bool _loop = false;

public:
	void AddFrame(unsigned int spriteId, float frameTime);
	
	void Play(bool loop = false);
	void Stop();
	
	void Update(float elapsedMs);
	void Render(float x, float y, bool isUI = false);
};
typedef CAnimation* pAnimation;

#endif // !__ANIMATION_H__
