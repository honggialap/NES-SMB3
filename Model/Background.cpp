#pragma region INCLUDE
#include "Background.h"
#pragma endregion

void CBackground::Load()
{
	CGameObject::Load();

	/* Read file */
	pugi::xml_document prefab;
	prefab.load_file(_source.c_str());

	/* Tile map */
	pugi::xml_node tileMapNode = prefab.child("Prefab").child("TileMap");
	_mapWidth = tileMapNode.attribute("mapWidth").as_uint();
	_mapHeight = tileMapNode.attribute("mapHeight").as_uint();
	_cellWidth = tileMapNode.attribute("cellWidth").as_float();
	_cellHeight = tileMapNode.attribute("cellHeight").as_float();
	for (auto cellNode = tileMapNode.child("Cell");
		cellNode;
		cellNode = cellNode.next_sibling("Cell"))
	{
		_cells.push_back(
			cellNode.attribute("spriteId").as_uint()
		);
	}
}

void CBackground::Start()
{
}

void CBackground::Update(float elapsedMs)
{
}

void CBackground::Render()
{
	for (unsigned int i = 0; i < _cells.size(); i++)
	{
		float renderX = _x + (i % _mapWidth) * _cellWidth;
		float renderY = _y - (i / _mapWidth) * _cellHeight;
		_sprites[_cells.at(i)]->Render(renderX, renderY);
	}
}

int CBackground::IsCollidable()
{
	return 0;
}

int CBackground::IsBlocking()
{
	return 0;
}

void CBackground::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
}
