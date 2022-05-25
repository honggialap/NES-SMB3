#pragma region INCLUDE
#include "Relay.h"

#include "../SuperMarioBros3.h"

#include "Brick.h"
#include "Coin.h"
#pragma endregion

void CRelay::Load()
{
	CGameObject::Load();

	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());

	/* Body */
	pugi::xml_node body = prefab.child("Prefab").child("Body");
	_renderBody = body.attribute("render").as_bool();
	BODY_WIDTH = body.attribute("width").as_float();
	BODY_HEIGHT = body.attribute("height").as_float();
	BODY_OFFSETX = body.attribute("offsetX").as_float();
	BODY_OFFSETY = body.attribute("offsetY").as_float();

	/* Transform */
	pugi::xml_node transform = prefab.child("Prefab").child("Transform");
	SPAWN_OFFSETY = transform.attribute("SPAWN_OFFSETY").as_float();
	TRANSFORM_TIMEOUT = transform.attribute("TRANSFORM_TIMEOUT").as_float();
}

void CRelay::Start()
{
	CGameObject::Start();
}

void CRelay::Update(float elapsedMs)
{
	if (!_start) Start();
	HandleAction(elapsedMs);
}

void CRelay::Render()
{
	if (_renderBody) _sprites[SPR_BBOX]->Render(_x, _y);
	switch (_action)
	{
	case CRelay::EAction::SPAWN:
		break;

	case CRelay::EAction::IDLE:
		_animations[ANI_RELAY]->Render(_x, _y);
		break;

	case CRelay::EAction::TRIGGERED:
		_sprites[SPR_RELAY_TRIGGERED]->Render(_x, _y);
		break;
	}
}

#pragma region STATE MACHINE

void CRelay::HandleAction(float elapsedMs)
{
	switch (_action)
	{
	case CRelay::EAction::SPAWN:
		Spawn(elapsedMs);
		break;

	case CRelay::EAction::IDLE:
		Idle(elapsedMs);
		break;

	case CRelay::EAction::TRIGGERED:
		Triggered(elapsedMs);
		break;
	}
}

void CRelay::Spawn(float elapsedMs)
{
	switch (_actionStage)
	{
	case CRelay::EActionStage::ENTRY:
	{
		_y += SPAWN_OFFSETY;
		//Spawn Effect
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CRelay::EActionStage::PROGRESS:
	{
		SetNextAction(EAction::IDLE);
	}
	break;

	case CRelay::EActionStage::EXIT:
	{

	}
	NextAction();
	break;
	}
}

void CRelay::Idle(float elapsedMs)
{
	switch (_actionStage)
	{
	case CRelay::EActionStage::ENTRY:
	{
		/* Animation Start */
		_animations[ANI_RELAY]->Play(true);
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CRelay::EActionStage::PROGRESS:
	{
		/* Animation Update */
		_animations[ANI_RELAY]->Update(elapsedMs);
	}
	break;

	case CRelay::EActionStage::EXIT:
	{
		/* Animation Stop */
		_animations[ANI_RELAY]->Stop();
	}
	NextAction();
	break;
	}
}

void CRelay::Triggered(float elapsedMs)
{
	switch (_actionStage)
	{
	case CRelay::EActionStage::ENTRY:
	{
		_triggered = false;
		_timeOut = false;
		_transformTimeout = TRANSFORM_TIMEOUT;
		Transform();
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CRelay::EActionStage::PROGRESS:
	{
		if (_triggered) break;
		
		if (_transformTimeout > 0) 
			_transformTimeout -= elapsedMs;
		else
		{
			_triggered = true;
			_timeOut = true;
			Transform();
		}
	}
	break;

	case CRelay::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CRelay::Transform()
{
	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());

	auto coin = prefab.child("Prefab").child("Coin");
	auto brick = prefab.child("Prefab").child("Brick");

	auto actives = _game->GetActives();
	for (auto gameObject : actives)
	{
		if (dynamic_cast<pCoin>(_game->Get(gameObject)))
		{
			auto activeCoin = dynamic_cast<pCoin>(_game->Get(gameObject));
			if (!activeCoin->IsDestroyed())
			{
				float x = 0;
				float y = 0;
				activeCoin->GetPosition(x, y);

				int gx = 0;
				int gy = 0;
				activeCoin->GetGrid(gx, gy);

				_game->Create(
					_scene,
					brick.attribute("actor").as_uint(),
					activeCoin->GetName() + brick.attribute("name").as_string(),
					brick.attribute("source").as_string(),
					x, y, gx, gy, activeCoin->GetLayer()
				);

				activeCoin->Destroy();
			}
		}
		else if (dynamic_cast<pBrick>(_game->Get(gameObject)))
		{
			auto activeBrick = dynamic_cast<pBrick>(_game->Get(gameObject));
			if (!activeBrick->IsDestroyed())
			{
				float x = 0;
				float y = 0;
				activeBrick->GetPosition(x, y);

				int gx = 0;
				int gy = 0;
				activeBrick->GetGrid(gx, gy);

				auto spawnedCoin = dynamic_cast<pCoin>(
					_game->Create(
						_scene,
						coin.attribute("actor").as_uint(),
						activeBrick->GetName() + coin.attribute("name").as_string(),
						coin.attribute("source").as_string(),
						x, y, gx, gy, activeBrick->GetLayer()
					)
					);

				if (_timeOut)	spawnedCoin->_spin = true;
				else			spawnedCoin->_spin = false;

				activeBrick->Destroy();
			}
		}
	}
}

#pragma endregion

#pragma region COLLISION

int CRelay::IsCollidable()
{
	switch (_action)
	{
	case CRelay::EAction::IDLE:
		return 1;
		break;

	case CRelay::EAction::SPAWN:
	case CRelay::EAction::TRIGGERED:
		return 0;
		break;

	default:
		return 0;
		break;
	}
}

int CRelay::IsBlocking()
{
	return 0;
}

void CRelay::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	switch (_action)
	{
	case CRelay::EAction::IDLE:
		left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
		right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
		top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
		bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
		break;

	case CRelay::EAction::SPAWN:
	case CRelay::EAction::TRIGGERED:
		break;
	}
}

#pragma endregion
