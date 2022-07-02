#pragma region INCLUDE
#include "Title.h"

#include "SuperMarioBros3.h"
#pragma endregion

void CTitle::Load()
{
	CGameObject::Load();

	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());

	pugi::xml_node command = prefab.child("Prefab").child("Command");
	SPACE = command.attribute("SPACE").as_uint();

	pugi::xml_node camera = prefab.child("Prefab").child("Camera");
	CAMERA_ORIGIN_X = camera.attribute("x").as_float();
	CAMERA_ORIGIN_Y = camera.attribute("y").as_float();

	pugi::xml_node background = prefab.child("Prefab").child("Background");
	_backgroundX = background.attribute("x").as_float();
	_backgroundY = background.attribute("y").as_float();

	pugi::xml_node logo = prefab.child("Prefab").child("Logo");
	_logoX = logo.attribute("x").as_float();
	_logoY = logo.attribute("y").as_float();

	pugi::xml_node label = prefab.child("Prefab").child("Label");
	_labelX = label.attribute("x").as_float();
	_labelY = label.attribute("y").as_float();
	BLINK_INTERVAL = label.attribute("BLINK_INTERVAL").as_float();
	_blinkInterval = BLINK_INTERVAL;
	_blink = false;

	pugi::xml_node startGame = prefab.child("Prefab").child("StartGame");
	NEXT_SCENE = startGame.attribute("NEXT_SCENE").as_uint();
}

void CTitle::Start()
{
	CGameObject::Start();
	_game->MoveCameraTo(CAMERA_ORIGIN_X, CAMERA_ORIGIN_Y);
	_animations[ANI_LOGO]->Play(true);
}

void CTitle::Update(float elapsedMs)
{
	if (!_start) Start();

	_animations[ANI_LOGO]->Update(elapsedMs);
	if (_blinkInterval > 0) {
		_blinkInterval -= elapsedMs;
	}
	else {
		_blink = !_blink;
		_blinkInterval = BLINK_INTERVAL;
	}
	if (_game->IsKeyPressed(SPACE)) {
		_game->PlayScene(NEXT_SCENE);
	}
}

void CTitle::Render()
{
	_sprites[SPR_TITLE_BACKGROUND]->Render(_backgroundX, _backgroundY);
	if (_blink) _sprites[SPR_LABEL]->Render(_labelX, _labelY);
	_animations[ANI_LOGO]->Render(_logoX, _logoY);
}

int CTitle::IsCollidable()
{
	return 0;
}

int CTitle::IsBlocking()
{
	return 0;
}

void CTitle::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}
