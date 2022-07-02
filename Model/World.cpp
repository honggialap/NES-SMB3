#pragma region INCLUDE
#include "World.h"

#include "SuperMarioBros3.h"
#pragma endregion



void CWorld::Load()
{
	CGameObject::Load();

	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());

	pugi::xml_node worldMapID = prefab.child("Prefab").child("WorldMapID");
	WORLD_MAP_ID = worldMapID.attribute("WORLD_MAP_ID").as_uint();
}

void CWorld::Start()
{
	_start = true;
	SetCamera(0);
}

void CWorld::Update(float elapsedMs)
{
	if (!_start) Start();
	if (_camTarget != nullptr)
	{
		float gameCameraX = 0;
		float gameCameraY = 0;
		_game->GetCamera(gameCameraX, gameCameraY);

		float centerX = gameCameraX + (_game->GetGraphics()->GetBackBufferWidth() / 2);
		float centerY = gameCameraY - (_game->GetGraphics()->GetBackBufferHeight() / 2);
		float trackLeft = centerX + _camTrackOffsetX - (_camTrackWidth / 2);
		float trackTop = centerY + _camTrackOffsetY + (_camTrackHeight / 2);
		float trackRight = centerX + _camTrackOffsetX + (_camTrackWidth / 2);
		float trackBottom = centerY + _camTrackOffsetY - (_camTrackHeight / 2);

		float targetX = 0;
		float targetY = 0;
		_camTarget->GetPosition(targetX, targetY);

		if (targetX < trackLeft)
		{
			_game->MoveCameraBy(targetX - trackLeft, 0);
		}
		else if (targetX > trackRight)
		{
			_game->MoveCameraBy(targetX - trackRight, 0);
		}

		if (targetY > trackTop)
		{
			_game->MoveCameraBy(0, targetY - trackTop);
		}
		else if (targetY < trackBottom)
		{
			_game->MoveCameraBy(0, targetY - trackBottom);
		}

		_game->GetCamera(gameCameraX, gameCameraY);

		if (gameCameraX < _camBoundLeft)
		{
			gameCameraX = _camBoundLeft;
		}
		else if (gameCameraX > _camBoundRight)
		{
			gameCameraX = _camBoundRight;
		}

		if (gameCameraY > _camBoundTop)
		{
			gameCameraY = _camBoundTop;
		}
		else if (gameCameraY < _camBoundBottom)
		{
			gameCameraY = _camBoundBottom;
		}
		_game->MoveCameraTo(gameCameraX, gameCameraY);
	}
}

void CWorld::Render()
{
}


void CWorld::SetCamera(unsigned int cameraID)
{
	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());

	for (auto cameraNode = prefab.child("Prefab").child("Camera");
		cameraNode;
		cameraNode = cameraNode.next_sibling("Camera"))
	{
		if (cameraID == cameraNode.attribute("id").as_uint())
		{
			_camBoundLeft = cameraNode.attribute("leftBound").as_float();
			_camBoundTop = cameraNode.attribute("topBound").as_float();
			_camBoundRight = cameraNode.attribute("rightBound").as_float();
			_camBoundBottom = cameraNode.attribute("bottomBound").as_float();
			_camTrackWidth = cameraNode.attribute("trackWidth").as_float();
			_camTrackHeight = cameraNode.attribute("trackHeight").as_float();
			_camTrackOffsetX = cameraNode.attribute("trackOffsetX").as_float();
			_camTrackOffsetY = cameraNode.attribute("trackOffsetY").as_float();
			_camTarget = _game->Get(cameraNode.attribute("targetName").as_string());
		}
	}
}

void CWorld::BackToWorldMap()
{
	_game->PlayScene(WORLD_MAP_ID);
}

int CWorld::IsCollidable()
{
	return 0;
}

int CWorld::IsBlocking()
{
	return 0;
}

void CWorld::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}
