#ifndef TILEMAPCALC_H
#define TILEMAPCALC_H


#include "../Graphics/GraphicsPipe.h"
#include "../ECS/ECS.h"
#include "../Math/vector2.h"

namespace Tilemap

{
	/******************************************************************/
	/*!
	\fn      void Tilemap::resizeTiles(ecs::TilemapComponent* tilemap, int rowLength, int colLength)
	\brief   Resizes the tiles in the tilemap to the specified row and column lengths.
	\param   tilemap - Pointer to the tilemap component to be resized.
	\param   rowLength - The new number of rows in the tilemap.
	\param   colLength - The new number of columns in the tilemap.
	\details Adjusts the size of the tilemap grid by modifying its rows and columns.
	*/
	/******************************************************************/
	void resizeTiles(ecs::TilemapComponent* tilemap, int rowLength, int colLength);

	/******************************************************************/
	/*!
	\fn      void Tilemap::setIndividualTile(vector2::Vec2& originCoordinates, const vector2::Vec2& tileCoordinates, ecs::TilemapComponent* tilemap)
	\brief   Sets an individual tile in the tilemap based on given coordinates.
	\param   originCoordinates - Reference to the origin coordinates of the tile to be set.
	\param   tileCoordinates - The coordinates of the tile within the tilemap.
	\param   tilemap - Pointer to the tilemap component.
	\details Updates the tilemap with a specific tile based on the provided coordinates.
	*/
	/******************************************************************/
	void setIndividualTile(vector2::Vec2& originCoordinates, const vector2::Vec2& tileCoordinates, ecs::TilemapComponent* tilemap);

	/******************************************************************/
	/*!
	\fn      void Tilemap::debugTileIndex(ecs::TilemapComponent* tilemap)
	\brief   Outputs debugging information for a specific tile in the tilemap.
	\param   tilemap - Pointer to the tilemap component to debug.
	\details Prints the tilemap index and relevant data for debugging purposes.
	*/
	/******************************************************************/
	void debugTileIndex(ecs::TilemapComponent* tilemap);

}


#endif TILEMAPCALC_H
