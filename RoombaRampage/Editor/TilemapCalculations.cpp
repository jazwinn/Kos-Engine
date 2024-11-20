#include "../Config/pch.h"
#include "TilemapCalculations.h"
#include <algorithm>

namespace Tilemap
{
	void resizeTiles(ecs::TilemapComponent* tilemap, int rowLength, int colLength)
	{
		if (rowLength <= 0)
		{
			tilemap->m_rowLength = 1;
			return;
		}
		if (colLength <= 0)
		{
			tilemap->m_columnLength = 1;
			return;
		}
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
	void setIndividualTile(vector2::Vec2& originCoordinates, const vector2::Vec2& tileCoordinates, ecs::TilemapComponent* tilemap)
	{
		if ((tileCoordinates.m_x) > originCoordinates.m_x + tilemap->m_rowLength) // If selected tile is longer than current tileset
		{
			LOGGING_INFO("Extending row length by %d", static_cast<int>(ceil(tileCoordinates.m_x - originCoordinates.m_x)));
			resizeTiles(tilemap, ceil(tileCoordinates.m_x - originCoordinates.m_x), tilemap->m_columnLength);
		}
		if (tileCoordinates.m_x < floor(originCoordinates.m_x))
		{
			resizeTiles(tilemap, tilemap->m_rowLength + ceil(originCoordinates.m_x - tileCoordinates.m_x), tilemap->m_columnLength); // Change Row Length

			for (std::vector<int>& row : tilemap->m_tilePictureIndex)
			{
				std::rotate(row.rbegin(),
					row.rbegin() + (ceil(originCoordinates.m_x - tileCoordinates.m_x)),
					row.rend()); // Rotate Indexes
			}
			
			originCoordinates.m_x = floor(tileCoordinates.m_x); //Set to center of tile
		}
		if (tileCoordinates.m_y < (originCoordinates.m_y + 1 - tilemap->m_columnLength)) // If selected tile is longer than current tileset
		{
			resizeTiles(tilemap, tilemap->m_rowLength , ceil(originCoordinates.m_y + 1 - tileCoordinates.m_y));
		}
		if (tileCoordinates.m_y > floor(originCoordinates.m_y + 1))
		{
			resizeTiles(tilemap, tilemap->m_rowLength, tilemap->m_columnLength + ceil(tileCoordinates.m_y - (originCoordinates.m_y + 1))); //Change Column Length
			std::rotate(tilemap->m_tilePictureIndex.rbegin(),
						tilemap->m_tilePictureIndex.rbegin() + (ceil(tileCoordinates.m_y - (originCoordinates.m_y + 1))),
						tilemap->m_tilePictureIndex.rend()); // Rotate Indexes
			originCoordinates.m_y = floor(tileCoordinates.m_y); //Set to center of tile
		}

		int indexX = floor(tileCoordinates.m_x - originCoordinates.m_x);
		int indexY = floor(originCoordinates.m_y - tileCoordinates.m_y) + 1.f;

		if (indexX >= tilemap->m_tilePictureIndex[0].size() || indexY >= tilemap->m_tilePictureIndex.size())
		{
			LOGGING_WARN("Clicked out of range: indexX=%d, indexY=%d", indexX, indexY);
			return;
		}

		tilemap->m_tilePictureIndex[indexY][indexX] = tilemap->m_tileIndex;

	}
	void debugTileIndex(ecs::TilemapComponent* tilemap)
	{
		for (int i = 0; i < tilemap->m_tilePictureIndex.size(); ++i)
		{
			std::ostringstream rowStream;

			rowStream << "Row " << i << ": ";
			for (int j = 0; j < tilemap->m_tilePictureIndex[i].size(); ++j)
			{
				rowStream << tilemap->m_tilePictureIndex[i][j] << ' ';
			}
			LOGGING_INFO("%s", rowStream.str().c_str());
		}
	}
}

