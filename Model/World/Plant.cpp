#pragma region INCLUDE
#include "Plant.h"
#include "../SuperMarioBros3.h"

#pragma endregion

void CPlant::Load()
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
	_plant = EPlant(type.attribute("plant").as_int());
	_type = EType(type.attribute("type").as_int());
	_growUp = type.attribute("up").as_bool();

	/* Sensor */
	pugi::xml_node sensor = prefab.child("Prefab").child("Sensor");
	_sensor = dynamic_cast<pPlantSensor>(
		_game->Create(
			_scene,
			sensor.attribute("actor").as_int(),
			_name + sensor.attribute("name").as_string(),
			sensor.attribute("source").as_string(),
			_x, _y, _gx, _gy, _layer
		)
		);

	/* Grow */
	pugi::xml_node grow = prefab.child("Prefab").child("Grow");
	GROW_LIMIT = grow.attribute("GROW_LIMIT").as_float();
	GROW_SPEED = grow.attribute("GROW_SPEED").as_float();
	STAND_INTERVAL = grow.attribute("STAND_INTERVAL").as_float();
	HIDE_INTERVAL = grow.attribute("HIDE_INTERVAL").as_float();

	/* Shoot */
	pugi::xml_node shoot = prefab.child("Prefab").child("Shoot");
	SHOOT_OFFSETY = shoot.attribute("SHOOT_OFFSETY").as_float();
	SHOOT_INTERVAL = shoot.attribute("SHOOT_INTERVAL").as_float();
	SHOOT_HORIZONTAL_SPEED = shoot.attribute("SHOOT_HORIZONTAL_SPEED").as_float();
	SHOOT_VERTICAL_SPEED = shoot.attribute("SHOOT_VERTICAL_SPEED").as_float();
	_targetName = shoot.attribute("targetName").as_string();

}

void CPlant::Start()
{
	CGameObject::Start();

}

void CPlant::Update(float elapsedMs)
{
	if (!_start) Start();

	HandleAction(elapsedMs);
	UpdateSensor(elapsedMs);
}

void CPlant::Render()
{
	if (_renderBody) _sprites[SPR_BBOX]->Render(_x, _y);

	switch (_action)
	{
	case CPlant::EAction::GROW:
	case CPlant::EAction::BURROW:
	{
		switch (_plant)
		{
		case CPlant::EPlant::VENUS:
		case CPlant::EPlant::PIRANHA:
		{
			switch (_type)
			{
			case CPlant::EType::GREEN:
			{
				if (_growUp)	_animations[ANI_PLANT_UP]->Render(_x, _y);
				else			_animations[ANI_PLANT_DOWN]->Render(_x, _y);
			}
			break;

			case CPlant::EType::RED:
			{
				if (_growUp)	_animations[ANI_RED_PLANT_UP]->Render(_x, _y);
				else			_animations[ANI_RED_PLANT_DOWN]->Render(_x, _y);
			}
			break;
			}
		}
		break;
		}
	}
	break;

	case CPlant::EAction::STAND:
	{
		switch (_plant)
		{
		case CPlant::EPlant::PIRANHA:
		{
			switch (_type)
			{
			case CPlant::EType::GREEN:
			{
				if (_growUp)	_sprites[SPR_PLANT_UP2]->Render(_x, _y);
				else			_sprites[SPR_PLANT_DOWN2]->Render(_x, _y);
			}
			break;

			case CPlant::EType::RED:
			{
				if (_growUp)	_sprites[SPR_RED_PLANT_UP2]->Render(_x, _y);
				else			_sprites[SPR_RED_PLANT_DOWN2]->Render(_x, _y);
			}
			break;
			}
		}
		break;

		case CPlant::EPlant::VENUS:
		{
			switch (_type)
			{
			case CPlant::EType::GREEN:
			{
				if (_growUp)
				{
					if (_up)
					{
						if (_left)	_sprites[SPR_PLANT_UP_LEFT_UP2]->Render(_x, _y);
						else		_sprites[SPR_PLANT_UP_RIGHT_UP2]->Render(_x, _y);
					}
					else
					{
						if (_left)	_sprites[SPR_PLANT_UP_LEFT_DOWN2]->Render(_x, _y);
						else		_sprites[SPR_PLANT_UP_RIGHT_DOWN2]->Render(_x, _y);
					}
				}
				else
				{
					if (_up)
					{
						if (_left)	_sprites[SPR_PLANT_DOWN_LEFT_UP2]->Render(_x, _y);
						else		_sprites[SPR_PLANT_DOWN_RIGHT_UP2]->Render(_x, _y);
					}
					else
					{
						if (_left)	_sprites[SPR_PLANT_DOWN_LEFT_DOWN2]->Render(_x, _y);
						else		_sprites[SPR_PLANT_DOWN_RIGHT_DOWN2]->Render(_x, _y);
					}
				}
			}
			break;

			case CPlant::EType::RED:
			{
				if (_growUp)
				{
					if (_up)
					{
						if (_left)	_sprites[SPR_RED_PLANT_UP_LEFT_UP2]->Render(_x, _y);
						else		_sprites[SPR_RED_PLANT_UP_RIGHT_UP2]->Render(_x, _y);
					}
					else
					{
						if (_left)	_sprites[SPR_RED_PLANT_UP_LEFT_DOWN2]->Render(_x, _y);
						else		_sprites[SPR_RED_PLANT_UP_RIGHT_DOWN2]->Render(_x, _y);
					}
				}
				else
				{
					if (_up)
					{
						if (_left)	_sprites[SPR_RED_PLANT_DOWN_LEFT_UP2]->Render(_x, _y);
						else		_sprites[SPR_RED_PLANT_DOWN_RIGHT_UP2]->Render(_x, _y);
					}
					else
					{
						if (_left)	_sprites[SPR_RED_PLANT_DOWN_LEFT_DOWN2]->Render(_x, _y);
						else		_sprites[SPR_RED_PLANT_DOWN_RIGHT_DOWN2]->Render(_x, _y);
					}
				}
			}
			break;
			}
		}
		break;
		}
	}
	break;

	case CPlant::EAction::SHOOT:
	{
		switch (_plant)
		{
		case CPlant::EPlant::VENUS:
		{
			switch (_type)
			{
			case CPlant::EType::GREEN:
			{
				if (_growUp)
				{
					if (_up)
					{
						if (_left)	_sprites[SPR_PLANT_UP_LEFT_UP1]->Render(_x, _y);
						else		_sprites[SPR_PLANT_UP_RIGHT_UP1]->Render(_x, _y);
					}
					else
					{
						if (_left)	_sprites[SPR_PLANT_UP_LEFT_DOWN1]->Render(_x, _y);
						else		_sprites[SPR_PLANT_UP_RIGHT_DOWN1]->Render(_x, _y);
					}
				}
				else
				{
					if (_up)
					{
						if (_left)	_sprites[SPR_PLANT_DOWN_LEFT_UP1]->Render(_x, _y);
						else		_sprites[SPR_PLANT_DOWN_RIGHT_UP1]->Render(_x, _y);
					}
					else
					{
						if (_left)	_sprites[SPR_PLANT_DOWN_LEFT_DOWN1]->Render(_x, _y);
						else		_sprites[SPR_PLANT_DOWN_RIGHT_DOWN1]->Render(_x, _y);
					}
				}
			}
			break;

			case CPlant::EType::RED:
			{
				if (_growUp)
				{
					if (_up)
					{
						if (_left)	_sprites[SPR_RED_PLANT_UP_LEFT_UP1]->Render(_x, _y);
						else		_sprites[SPR_RED_PLANT_UP_RIGHT_UP1]->Render(_x, _y);
					}
					else
					{
						if (_left)	_sprites[SPR_RED_PLANT_UP_LEFT_DOWN1]->Render(_x, _y);
						else		_sprites[SPR_RED_PLANT_UP_RIGHT_DOWN1]->Render(_x, _y);
					}
				}
				else
				{
					if (_up)
					{
						if (_left)	_sprites[SPR_RED_PLANT_DOWN_LEFT_UP1]->Render(_x, _y);
						else		_sprites[SPR_RED_PLANT_DOWN_RIGHT_UP1]->Render(_x, _y);
					}
					else
					{
						if (_left)	_sprites[SPR_RED_PLANT_DOWN_LEFT_DOWN1]->Render(_x, _y);
						else		_sprites[SPR_RED_PLANT_DOWN_RIGHT_DOWN1]->Render(_x, _y);
					}
				}
			}
			break;
			}
		}
		break;
		}
	}
	break;
	}
}

#pragma region STATE MACHINE

void CPlant::HandleAction(float elapsedMs)
{
	switch (_action)
	{
	case CPlant::EAction::HIDE:
		Hide(elapsedMs);
		break;

	case CPlant::EAction::GROW:
		Grow(elapsedMs);
		break;

	case CPlant::EAction::STAND:
		Stand(elapsedMs);
		break;

	case CPlant::EAction::BURROW:
		Burrow(elapsedMs);
		break;

	case CPlant::EAction::SHOOT:
		Shoot(elapsedMs);
		break;

	case CPlant::EAction::DIE:
		Die(elapsedMs);
		break;
	}
}

void CPlant::Hide(float elapsedMs)
{
	switch (_actionStage)
	{
	case CPlant::EActionStage::ENTRY:
	{
		_hideInterval = HIDE_INTERVAL;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CPlant::EActionStage::PROGRESS:
	{
		if(_sensor->_triggered) _hideInterval = HIDE_INTERVAL;

		if (_hideInterval > 0) _hideInterval -= elapsedMs;
		else SetNextAction(EAction::GROW);
	}
	break;

	case CPlant::EActionStage::EXIT:
	{

	}
	NextAction();
	break;
	}
}

void CPlant::Grow(float elapsedMs)
{
	switch (_actionStage)
	{
	case CPlant::EActionStage::ENTRY:
	{
		_growOrigin = _y;
		if (_growUp)	_growLimit = _y + GROW_LIMIT;
		else			_growLimit = _y - GROW_LIMIT;

		/* Animation Start */
		{
			if (_growUp)
			{
				_animations[ANI_PLANT_UP]->Play(true);
				_animations[ANI_RED_PLANT_UP]->Play(true);
			}
			else
			{
				_animations[ANI_PLANT_DOWN]->Play(true);
				_animations[ANI_RED_PLANT_DOWN]->Play(true);
			}
		}
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CPlant::EActionStage::PROGRESS:
	{
		/* Animation Update */
		{
			if (_growUp)
			{
				_animations[ANI_PLANT_UP]->Update(elapsedMs);
				_animations[ANI_RED_PLANT_UP]->Update(elapsedMs);
			}
			else
			{
				_animations[ANI_PLANT_DOWN]->Update(elapsedMs);
				_animations[ANI_RED_PLANT_DOWN]->Update(elapsedMs);
			}
		}

		if (_growUp)
		{
			if (_y < _growLimit)
			{
				_y += GROW_SPEED * elapsedMs;
			}
			else
			{
				_y = _growLimit;
				SetNextAction(EAction::STAND);
			}
		}
		else
		{
			if (_y > _growLimit)
			{
				_y -= GROW_SPEED * elapsedMs;
			}
			else
			{
				_y = _growLimit;
				SetNextAction(EAction::STAND);
			}
		}
	}
	break;

	case CPlant::EActionStage::EXIT:
	{
		/* Animation Stop */
		{
			if (_growUp)
			{
				_animations[ANI_PLANT_UP]->Stop();
				_animations[ANI_RED_PLANT_UP]->Stop();
			}
			else
			{
				_animations[ANI_PLANT_DOWN]->Stop();
				_animations[ANI_RED_PLANT_DOWN]->Stop();
			}
		}
	}
	NextAction();
	break;
	}
}

void CPlant::Stand(float elapsedMs)
{
	switch (_actionStage)
	{
	case CPlant::EActionStage::ENTRY:
	{
		_standInterval = STAND_INTERVAL;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CPlant::EActionStage::PROGRESS:
	{
		AcquireTarget();

		if (_standInterval > 0) _standInterval -= elapsedMs;
		else
		{
			switch (_plant)
			{
			case CPlant::EPlant::PIRANHA:
				SetNextAction(EAction::BURROW);
				break;

			case CPlant::EPlant::VENUS:
				SetNextAction(EAction::SHOOT);
				break;
			}
		}
	}
	break;

	case CPlant::EActionStage::EXIT:
	{

	}
	NextAction();
	break;
	}
}

void CPlant::Burrow(float elapsedMs)
{
	switch (_actionStage)
	{
	case CPlant::EActionStage::ENTRY:
	{
		/* Animation Start */
		{
			if (_growUp)
			{
				_animations[ANI_PLANT_UP]->Play(true);
				_animations[ANI_RED_PLANT_UP]->Play(true);
			}
			else
			{
				_animations[ANI_PLANT_DOWN]->Play(true);
				_animations[ANI_RED_PLANT_DOWN]->Play(true);
			}
		}
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CPlant::EActionStage::PROGRESS:
	{
		/* Animation Update */
		{
			if (_growUp)
			{
				_animations[ANI_PLANT_UP]->Update(elapsedMs);
				_animations[ANI_RED_PLANT_UP]->Update(elapsedMs);
			}
			else
			{
				_animations[ANI_PLANT_DOWN]->Update(elapsedMs);
				_animations[ANI_RED_PLANT_DOWN]->Update(elapsedMs);
			}
		}

		if (_growUp)
		{
			if (_y > _growOrigin)
			{
				_y -= GROW_SPEED * elapsedMs;
			}
			else
			{
				_y = _growOrigin;
				SetNextAction(EAction::HIDE);
			}
		}
		else
		{
			if (_y < _growOrigin)
			{
				_y += GROW_SPEED * elapsedMs;
			}
			else
			{
				_y = _growOrigin;
				SetNextAction(EAction::HIDE);
			}
		}
	}
	break;

	case CPlant::EActionStage::EXIT:
	{

	}
	NextAction();
	break;
	}
}

void CPlant::Shoot(float elapsedMs)
{
	switch (_actionStage)
	{
	case CPlant::EActionStage::ENTRY:
	{
		_shootInterval = SHOOT_INTERVAL;
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CPlant::EActionStage::PROGRESS:
	{
		AcquireTarget();

		if (_shootInterval > 0) _shootInterval -= elapsedMs;
		else
		{
			ShootFireball();
			SetNextAction(EAction::BURROW);
		}
	}
	break;

	case CPlant::EActionStage::EXIT:
	{

	}
	NextAction();
	break;
	}
}

void CPlant::Die(float elapsedMs)
{
	switch (_actionStage)
	{
	case CPlant::EActionStage::ENTRY:
	{
		//SpawnDeadEffect();
		_sensor->Destroy();
		Destroy();
	}
	_actionStage = EActionStage::PROGRESS;
	break;

	case CPlant::EActionStage::PROGRESS:
	{

	}
	break;

	case CPlant::EActionStage::EXIT:
	{

	}
	NextAction();
	break;
	}
}

void CPlant::UpdateSensor(float elapsedMs)
{
	if (_sensor != nullptr) _sensor->SetPosition(_x, _y);
}

void CPlant::AcquireTarget()
{
	if (_game->Get(_targetName) != nullptr)
	{
		float targetX = 0;
		float targetY = 0;
		_game->Get(_targetName)->GetPosition(targetX, targetY);

		if (targetX < _x) _left = true;
		else _left = false;

		if (_growUp)
		{
			if (targetY > _y + SHOOT_OFFSETY) _up = true;
			else _up = false;
		}
		else
		{
			if (targetY > _y - SHOOT_OFFSETY) _up = true;
			else _up = false;
		}
	}
}

void CPlant::ShootFireball()
{
}

void CPlant::Hit()
{
}

#pragma endregion

#pragma region COLLISION

int CPlant::IsCollidable()
{
	switch (_action)
	{
	case CPlant::EAction::GROW:
	case CPlant::EAction::STAND:
	case CPlant::EAction::BURROW:
	case CPlant::EAction::SHOOT:
		return 1;
		break;

	case CPlant::EAction::HIDE:
	case CPlant::EAction::DIE:
		return 0;
		break;

	default:
		return 0;
		break;
	}
}

int CPlant::IsBlocking()
{
	return 0;
}

void CPlant::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	switch (_action)
	{
	case CPlant::EAction::GROW:
	case CPlant::EAction::STAND:
	case CPlant::EAction::BURROW:
	case CPlant::EAction::SHOOT:
	{
		left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
		right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
		top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
		bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
	}
	break;
	}
}

void CPlant::OnNoCollision(float elapsedMs)
{
}

void CPlant::OnCollisionWith(pCollision collision)
{
}

#pragma endregion
