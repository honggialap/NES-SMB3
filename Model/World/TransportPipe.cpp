#pragma region INCLUDE
#include "TransportPipe.h"
#pragma endregion

void CTransportPipe::Load()
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
	DESTINATION_X = type.attribute("DESTINATION_X").as_float();
	DESTINATION_Y = type.attribute("DESTINATION_Y").as_float();
	CAMERA_SWITCH = type.attribute("CAMERA_SWITCH").as_int();
}

void CTransportPipe::Start()
{
	CGameObject::Start();
}

void CTransportPipe::Update(float elapsedMs)
{
	if (!_start) Start();
}

void CTransportPipe::Render()
{
	if (_renderBody)
	{
		_sprites[SPR_BBOX]->Render(_x, _y);
		_sprites[SPR_BBOX]->Render(DESTINATION_X, DESTINATION_Y);
	}
}

#pragma region COLLISION

int CTransportPipe::IsCollidable()
{
	return 1;
}

int CTransportPipe::IsBlocking()
{
	return 0;
}

void CTransportPipe::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
	right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
	top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
	bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
}

void CTransportPipe::OnNoCollision(float elapsedMs)
{
}

void CTransportPipe::OnCollisionWith(pCollision collision)
{
}

#pragma endregion