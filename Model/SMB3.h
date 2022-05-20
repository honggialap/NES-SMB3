#pragma once
#ifndef __SMB3_H__
#define __SMB3_H__

#pragma region INCLUDE
#include "Engine/Game.h"
#pragma endregion

class CSMB3 : public CGame
{
	virtual pGameObject Create(
		unsigned int actorID, std::string name, std::string source,
		float posX, float posY,
		int gridX, int gridY,
		unsigned int layer
	);
};
typedef CSMB3* pSMB3;



#endif // !__SMB3_H__
