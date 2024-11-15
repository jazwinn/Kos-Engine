#ifndef TILEMAPCALC_H
#define TILEMAPCALC_H

#include "EditorCamera.h"
#include "../Graphics/GraphicsPipe.h"
#include "../ECS/ECS.h"
#include "../Math/vector2.h"

namespace Tilemap

{
	void resizeTiles(ecs::TilemapComponent *tilemap, int rowLength, int colLength);
	void setIndividualTile(vector2::Vec2& originCoordinates, const vector2::Vec2& tileCoordinates, ecs::TilemapComponent* tilemap);
	void debugTileIndex(ecs::TilemapComponent* tilemap);
}


#endif TILEMAPCALC_H
