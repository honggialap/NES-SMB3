#pragma region INCLUDE
#include "DeadZone.h"
#pragma endregion

void CDeadZone::Load()
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
}

void CDeadZone::Start()
{
	CGameObject::Start();
}

void CDeadZone::Update(float elapsedMs)
{
	if (!_start) Start();
}

void CDeadZone::Render()
{
	if (_renderBody) _sprites[SPR_BBOX]->Render(_x, _y);
}

#pragma region COLLISION

int CDeadZone::IsCollidable()
{
	return 1;
}

int CDeadZone::IsBlocking()
{
	return 0;
}

void CDeadZone::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
	right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
	top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
	bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
}

void CDeadZone::OnNoCollision(float elapsedMs)
{
}

void CDeadZone::OnCollisionWith(pCollision collision)
{
}

#pragma endregion