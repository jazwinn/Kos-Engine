#include "../Config/pch.h"
#include "TilemapCalculations.h"
#include <algorithm>

namespace Tilemap
{
	void resizeTiles(ecs::TilemapComponent* tilemap, int rowLength, int colLength)
	{
		tilemap->m_rowLength = rowLength;
		tilemap->m_columnLength = colLength;
		if (tilemap->m_tilePictureIndex.size() != tilemap->m_columnLength)
		{
			tilemap->m_tilePictureIndex.resize(colLength);
		}
		for (std::vector<int>& row : tilemap->m_tilePictureIndex)
		{
			row.resize(rowLength);
		}
	}
	void setIndividualTile(vector2::Vec2& originCoordinates, const vector2::Vec2& tileCoordinates, ecs::TilemapComponent* tilemap, int spriteIndex)
	{
		if (tileCoordinates.m_x > floor(originCoordinates.m_x + tilemap->m_rowLength)) // If selected tile is longer than current tileset
		{
			resizeTiles(tilemap, tilemap->m_rowLength + floor(tileCoordinates.m_x - floor(originCoordinates.m_x + tilemap->m_rowLength)), tilemap->m_columnLength);
		}
		if (tileCoordinates.m_x < floor(originCoordinates.m_x))
		{
			
			resizeTiles(tilemap, tilemap->m_rowLength + floor(originCoordinates.m_x - tileCoordinates.m_x), tilemap->m_columnLength); // Change Row Length

			for (std::vector<int> row : tilemap->m_tilePictureIndex)
			{
				std::rotate(row.begin(),
					row.begin() + (floor(tileCoordinates.m_y - originCoordinates.m_y)),
					row.end()); // Rotate Indexes
			}
			
			originCoordinates.m_x = floor(tileCoordinates.m_x) + 0.5f; //Set to center of tile
		}
		if (tileCoordinates.m_y < floor(originCoordinates.m_y + tilemap->m_columnLength)) // If selected tile is longer than current tileset
		{
			resizeTiles(tilemap, tilemap->m_rowLength , tilemap->m_columnLength + floor(tileCoordinates.m_y - floor(originCoordinates.m_y + tilemap->m_columnLength)));
		}
		if (tileCoordinates.m_y > floor(originCoordinates.m_y))
		{
			resizeTiles(tilemap, tilemap->m_rowLength, tilemap->m_columnLength + floor(originCoordinates.m_y - tileCoordinates.m_y)); //Change Column Length
			
			for (int i = 0; i < tilemap->m_tilePictureIndex.size(); ++i)
			{
				std::rotate(tilemap->m_tilePictureIndex[i].begin(),
							tilemap->m_tilePictureIndex[i].begin() + (floor(originCoordinates.m_x - tileCoordinates.m_x)),
							tilemap->m_tilePictureIndex[i].end()); // Rotate Indexes
			}

			originCoordinates.m_y = floor(tileCoordinates.m_y) + 0.5f; //Set to center of tile
		}

		int indexX = tileCoordinates.m_x - originCoordinates.m_x;
		int indexY = originCoordinates.m_y - tileCoordinates.m_y;

		tilemap->m_tilePictureIndex[indexY][indexX] = spriteIndex;

	}
	void debugTileIndex(ecs::TilemapComponent* tilemap)
	{
		for (int i = 0; i < tilemap->m_tilePictureIndex.size(); ++i)
		{
			std::cout << "Row " << i << ": ";
			for (int j = 0; j < tilemap->m_tilePictureIndex[i].size(); ++j)
			{
				std::cout << tilemap->m_tilePictureIndex[i][j] << ' ';
			}
			std::cout << std::endl;
		}
	}
}

