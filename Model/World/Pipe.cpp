#pragma region INCLUDE
#include "Pipe.h"
#pragma endregion

void CPipe::Load()
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
	_direction = EDirection(type.attribute("direction").as_int());
}

void CPipe::Start()
{
	CGameObject::Start();
}

void CPipe::Update(float elapsedMs)
{
	if (!_start) Start();
}

void CPipe::Render()
{
	if (_renderBody) _sprites[SPR_BBOX]->Render(_x, _y);
	switch (_type)
	{
	case CPipe::EType::GREEN:
	{
		switch (_direction)
		{
		case CPipe::EDirection::LEFT:
			_sprites[SPR_PIPE_GREEN_LEFT]->Render(_x, _y);
			break;

		case CPipe::EDirection::UP:
			_sprites[SPR_PIPE_GREEN_UP]->Render(_x, _y);
			break;

		case CPipe::EDirection::RIGHT:
			_sprites[SPR_PIPE_GREEN_RIGHT]->Render(_x, _y);
			break;

		case CPipe::EDirection::DOWN:
			_sprites[SPR_PIPE_GREEN_DOWN]->Render(_x, _y);
			break;
		}
	}
	break;

	case CPipe::EType::BLACK:
	{
		switch (_direction)
		{
		case CPipe::EDirection::LEFT:
			_sprites[SPR_PIPE_BLACK_LEFT]->Render(_x, _y);
			break;

		case CPipe::EDirection::UP:
			_sprites[SPR_PIPE_BLACK_UP]->Render(_x, _y);
			break;

		case CPipe::EDirection::RIGHT:
			_sprites[SPR_PIPE_BLACK_RIGHT]->Render(_x, _y);
			break;

		case CPipe::EDirection::DOWN:
			_sprites[SPR_PIPE_BLACK_DOWN]->Render(_x, _y);
			break;
		}
	}
	break;
	}
}

#pragma region COLLISION

int CPipe::IsCollidable()
{
	return 1;
}

int CPipe::IsBlocking()
{
	return 0;
}

void CPipe::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
	right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
	top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
	bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
}

void CPipe::OnNoCollision(float elapsedMs)
{
}

void CPipe::OnCollisionWith(pCollision collision)
{
}

#pragma endregion