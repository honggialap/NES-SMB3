#pragma region INCLUDE
#include "KoopaSensor.h"
#include "../SuperMarioBros3.h"
#pragma endregion

void CKoopaSensor::Load()
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

void CKoopaSensor::Start()
{
	CGameObject::Start();
}

void CKoopaSensor::Update(float elapsedMs)
{
	if (!_start) Start();

	_triggered = true;

	std::vector<pGameObject> collidables = _game->GetLocal(_id);
	_collider->Process(elapsedMs, &collidables);
}

void CKoopaSensor::Render()
{
	if (_renderBody) _sprites[SPR_BBOX]->Render(_x, _y);
}

#pragma region COLLISION

int CKoopaSensor::IsCollidable()
{
	return 1;
}

int CKoopaSensor::IsBlocking()
{
	return 0;
}

void CKoopaSensor::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = _x + BODY_OFFSETX - (BODY_WIDTH / 2);
	right = _x + BODY_OFFSETX + (BODY_WIDTH / 2);
	top = _y + BODY_OFFSETY + (BODY_HEIGHT / 2);
	bottom = _y + BODY_OFFSETY - (BODY_HEIGHT / 2);
}

void CKoopaSensor::OnNoCollision(float elapsedMs)
{
}

void CKoopaSensor::OnCollisionWith(pCollision collision)
{
	_triggered = false;
}

#pragma endregion