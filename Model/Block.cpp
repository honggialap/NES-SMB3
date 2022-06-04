#pragma region INCLUDE
#include "Block.h"

#include "SuperMarioBros3.h"

#include "Coin.h"
#include "SuperMushroom.h"
#include "SuperLeaf.h"
#include "FireFlower.h"
#include "ExtraLifeMushroom.h"
#include "Relay.h"

#include "Mario.h"
#pragma endregion

void CBlock::Load()
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

	/* Type */
	pugi::xml_node type = prefab.child("Prefab").child("Type");
	_type = EType(type.attribute("type").as_int());
	_spawn = ESpawn(type.attribute("spawn").as_int());
	_spawnCount = type.attribute("spawnCount").as_int();

	/* Bounce */
	pugi::xml_node bounce = prefab.child("Prefab").child("Bounce");
	BOUNCE_LIMIT = bounce.attribute("BOUNCE_LIMIT").as_float();
	BOUNCE_SPEED = bounce.attribute("BOUNCE_SPEED").as_float();
	_targetName = bounce.attribute("target").as_string();
}

void CBlock::Start()
{
	CGameObject::Start();
}

void CBlock::Update(float elapsedMs)
{
	if (!_start) Start();
	HandleAction(elapsedMs);
}

void CBlock::Render()
{
	if (_renderBody) _sprites[SPR_BBOX]->Render(_x, _y);
	switch (_action)
	{
	case CBlock::EAction::IDLE:
	case CBlock::EAction::BOUNCE:
	{
		switch (_type)
		{
		case CBlock::EType::QUESTION:
			_animations[ANI_QUESTION_BLOCK]->Render(_renderX, _renderY);
			break;

		case CBlock::EType::BRICK:
			_animations[ANI_BRICK_BLOCK]->Render(_renderX, _renderY);
			break;
		}
	}
	break;

	case CBlock::EAction::EMPTY:
	{
		_sprites[SPR_EMPTY_BLOCK]->Render(_renderX, _renderY);
	}
	break;
	}
}

#pragma region STATE MACHINE

void CBlock::HandleAction(float elapsedMs)
{
	switch (_action)
	{
	case CBlock::EAction::IDLE:
		Idle(elapsedMs);
		break;

	case CBlock::EAction::BOUNCE:
		Bounce(elapsedMs);
		break;

	case CBlock::EAction::EMPTY:
		Empty(elapsedMs);
		break;
	}
}

void CBlock::Idle(float elapsedMs)
{
	switch (_actionStage)
	{
	case CBlock::EActionStage::ENTRY:
	{
		/* Animation Start */
		switch (_type)
		{
		case CBlock::EType::QUESTION:
			_animations[ANI_QUESTION_BLOCK]->Play(true);
			break;

		case CBlock::EType::BRICK:
			_animations[ANI_BRICK_BLOCK]->Play(true);
			break;
		}

		_renderX = _x;
		_renderY = _y;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CBlock::EActionStage::PROGRESS:
	{
		/* Animation Update */
		switch (_type)
		{
		case CBlock::EType::QUESTION:
			_animations[ANI_QUESTION_BLOCK]->Update(elapsedMs);
			break;

		case CBlock::EType::BRICK:
			_animations[ANI_BRICK_BLOCK]->Update(elapsedMs);
			break;
		}
	}
	break;

	case CBlock::EActionStage::EXIT:
	{
		/* Animation Stop */
		switch (_type)
		{
		case CBlock::EType::QUESTION:
			_animations[ANI_QUESTION_BLOCK]->Stop();
			break;

		case CBlock::EType::BRICK:
			_animations[ANI_BRICK_BLOCK]->Stop();
			break;
		}
	}
	NextAction();
	break;
	}
}

void CBlock::Bounce(float elapsedMs)
{
	switch (_actionStage)
	{
	case CBlock::EActionStage::ENTRY:
	{
		/* Animation Start */
		switch (_type)
		{
		case CBlock::EType::QUESTION:
			_animations[ANI_QUESTION_BLOCK]->Play(true);
			break;

		case CBlock::EType::BRICK:
			_animations[ANI_BRICK_BLOCK]->Play(true);
			break;
		}

		_return = false;

		if (_topBounce)
		{
			_bounceOrigin = _y;
			_bounceLimit = _y + BOUNCE_LIMIT;
			_renderX = _x;
			_renderY = _bounceOrigin;
		}
		else
		{
			if (_leftBounce)
			{
				_bounceOrigin = _x;
				_bounceLimit = _x - (BOUNCE_LIMIT/2);
				_renderX = _bounceOrigin;
				_renderY = _y;
			}
			else
			{
				_bounceOrigin = _x;
				_bounceLimit = _x + (BOUNCE_LIMIT/2);
				_renderX = _bounceOrigin;
				_renderY = _y;
			}
		}
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CBlock::EActionStage::PROGRESS:
	{
		/* Animation Update */
		switch (_type)
		{
		case CBlock::EType::QUESTION:
			_animations[ANI_QUESTION_BLOCK]->Update(elapsedMs);
			break;

		case CBlock::EType::BRICK:
			_animations[ANI_BRICK_BLOCK]->Update(elapsedMs);
			break;
		}

		if (_topBounce)
		{
			if (!_return)
			{
				if (_renderY < _bounceLimit)
				{
					_renderY += BOUNCE_SPEED * elapsedMs;
				}
				else
				{
					_renderY = _bounceLimit;
					_return = true;
					if (_spawnCount > 0)
					{
						Spawn();
						_spawnCount--;
					}
				}
			}
			else
			{
				if (_renderY > _bounceOrigin)
				{
					_renderY -= BOUNCE_SPEED * elapsedMs;
				}
				else
				{
					_renderY = _bounceOrigin;
					_return = false;
					
					if (_spawnCount == 0)
					{
						SetNextAction(EAction::EMPTY);
						break;
					}
					SetNextAction(EAction::IDLE);
					break;
				}
			}
		}
		else
		{
			if (_leftBounce)
			{
				if (!_return)
				{
					if (_renderX > _bounceLimit)
					{
						_renderX -= BOUNCE_SPEED * elapsedMs;
					}
					else
					{
						_renderX = _bounceLimit;
						_return = true;

						if (_spawnCount > 0)
						{
							Spawn();
							_spawnCount--;

						}
					}
				}
				else
				{
					if (_renderX < _bounceOrigin)
					{
						_renderX += BOUNCE_SPEED * elapsedMs;
					}
					else
					{
						_renderX = _bounceOrigin;
						_return = false;
						
						if (_spawnCount == 0)
						{
							SetNextAction(EAction::EMPTY);
							break;
						}
						SetNextAction(EAction::IDLE);
						break;
					}
				}
			}
			else
			{
				if (!_return)
				{
					if (_renderX < _bounceLimit)
					{
						_renderX += BOUNCE_SPEED * elapsedMs;
					}
					else
					{
						_renderX = _bounceLimit;
						_return = true;
						if (_spawnCount > 0)
						{
							Spawn();
							_spawnCount--;
						}
					}
				}
				else
				{
					if (_renderX > _bounceOrigin)
					{
						_renderX -= BOUNCE_SPEED * elapsedMs;
					}
					else
					{
						_renderX = _bounceOrigin;
						_return = false;

						if (_spawnCount == 0)
						{
							SetNextAction(EAction::EMPTY);
							break;
						}
						SetNextAction(EAction::IDLE);
						break;
					}
				}
			}
		}
	}
	break;

	case CBlock::EActionStage::EXIT:
	{
		/* Animation Stop */
		switch (_type)
		{
		case CBlock::EType::QUESTION:
			_animations[ANI_QUESTION_BLOCK]->Stop();
			break;

		case CBlock::EType::BRICK:
			_animations[ANI_BRICK_BLOCK]->Stop();
			break;
		}
	}
	NextAction();
	break;
	}
}

void CBlock::Empty(float elapsedMs)
{
	switch (_actionStage)
	{
	case CBlock::EActionStage::ENTRY:
	{
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CBlock::EActionStage::PROGRESS:
	{
	}
	break;

	case CBlock::EActionStage::EXIT:
	{
	}
	NextAction();
	break;
	}
}

void CBlock::Spawn()
{
	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());

	switch (_spawn)
	{
	case CBlock::ESpawn::COIN:
	{
		pugi::xml_node coin = prefab.child("Prefab").child("Coin");
		auto spawnedCoin = dynamic_cast<pCoin>(
			_game->Create(
				coin.attribute("actor").as_int(),
				_name + coin.attribute("name").as_string(),
				coin.attribute("source").as_string(),
				_x, _y, _gx, _gy, _layer - 1
			)
			);
		spawnedCoin->SetAction(CCoin::EAction::CONSUMED);
	}
	break;

	case CBlock::ESpawn::POWER_UP:
	{
		if (_game->Get(_targetName) != nullptr)
		{
			auto mario = dynamic_cast<pMario>(_game->Get(_targetName));
			if (mario->_power == CMario::EPower::SMALL)
			{
				pugi::xml_node superMushroom = prefab.child("Prefab").child("SuperMushroom");
				auto spawnedSuperMushroom = dynamic_cast<pSuperMushroom>(
					_game->Create(
						superMushroom.attribute("actor").as_int(),
						_name + superMushroom.attribute("name").as_string(),
						superMushroom.attribute("source").as_string(),
						_x, _y, _gx, _gy, _layer - 1
					)
					);
				spawnedSuperMushroom->SetAction(CSuperMushroom::EAction::SPAWN);
			}
			else
			{
				pugi::xml_node superLeaf = prefab.child("Prefab").child("SuperLeaf");
				auto spawnedSuperLeaf = dynamic_cast<pSuperLeaf>(
					_game->Create(
						superLeaf.attribute("actor").as_int(),
						_name + superLeaf.attribute("name").as_string(),
						superLeaf.attribute("source").as_string(),
						_x, _y, _gx, _gy, _layer - 1
					)
					);
				spawnedSuperLeaf->SetAction(CSuperLeaf::EAction::SPAWN);
			}
		}
	}
	break;

	case CBlock::ESpawn::FIRE_FLOWER:
	{
		pugi::xml_node fireFlower = prefab.child("Prefab").child("FireFlower");
		auto spawnedFireFlower = dynamic_cast<pFireFlower>(
			_game->Create(
				fireFlower.attribute("actor").as_int(),
				_name + fireFlower.attribute("name").as_string(),
				fireFlower.attribute("source").as_string(),
				_x, _y, _gx, _gy, _layer - 1
			)
			);
		spawnedFireFlower->SetAction(CFireFlower::EAction::SPAWN);
	}
	break;

	//case CBlock::ESpawn::STARMAN:
	//{
	//	pugi::xml_node starman = prefab.child("Prefab").child("Starman");
	//	auto spawnedStarman = dynamic_cast<pStarman>(
	//		_game->Create(
	//			starman.attribute("actor").as_int(),
	//			_name + starman.attribute("name").as_string(),
	//			starman.attribute("source").as_string(),
	//			_x, _y, _gx, _gy, _layer - 1
	//		)
	//		);
	//	spawnedStarman->SetAction(CStarman::EAction::SPAWN);
	//}
	//break;

	case CBlock::ESpawn::EXTRA_LIFE:
	{
		pugi::xml_node extraLifeMushroom = prefab.child("Prefab").child("ExtraLifeMushroom");
		auto spawnedExtraLifeMushroom = dynamic_cast<pExtraLifeMushroom>(
			_game->Create(
				extraLifeMushroom.attribute("actor").as_int(),
				_name + extraLifeMushroom.attribute("name").as_string(),
				extraLifeMushroom.attribute("source").as_string(),
				_x, _y, _gx, _gy, _layer - 1
			)
			);
		spawnedExtraLifeMushroom->SetAction(CExtraLifeMushroom::EAction::SPAWN);
	}
	break;

	case CBlock::ESpawn::RELAY:
	{
		pugi::xml_node relay = prefab.child("Prefab").child("Relay");
		auto spawnedRelay = dynamic_cast<pRelay>(
			_game->Create(
				relay.attribute("actor").as_int(),
				_name + relay.attribute("name").as_string(),
				relay.attribute("source").as_string(),
				_x, _y, _gx, _gy, _layer - 1
			)
			);
		//spawnedRelay->SetAction(CRelay::EAction::SPAWN);
	}
	break;
	}
}

void CBlock::HitBottom()
{
	if (_action == EAction::IDLE)
	{
		_topBounce = true;
		SetNextAction(EAction::BOUNCE);
	}
}

void CBlock::HitSide(bool left)
{
	if (_action == EAction::IDLE)
	{
		_topBounce = false;
		_leftBounce = left;
		SetNextAction(EAction::BOUNCE);
	}
}

#pragma endregion

#pragma region COLLISION

int CBlock::IsCollidable()
{
	return 1;
}

int CBlock::IsBlocking()
{
	return 1;
}

void CBlock::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
	right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
	top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
	bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
}

void CBlock::OnNoCollision(float elapsedMs)
{
}

void CBlock::OnCollisionWith(pCollision collision)
{
}

#pragma endregion
