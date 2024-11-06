/******************************************************************/
/*!
\file      Prefab.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file the class prefabs that read an json file and 
		   stores each object into the prefabs. When the function
		   is called. The prefab, would create and clone its data
		   onto the entity


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef PREFAB_H
#define PREFAB_H

#include "../ECS/ECSList.h"


namespace prefab {

	class Prefab {

	public:
		static int m_CreatePrefab(std::string prefabscene, std::string insertscene = {});

		static void m_SaveEntitytoPrefab(ecs::EntityID);

		//static void m_AssignEntitytoPrefab(std::string prefab, ecs::EntityID id);

		static void m_UpdateAllPrefabEntity(std::string prefab);

		static void m_UpdatePrefab(ecs::EntityID sceneprefabID, ecs::EntityID prefabid, bool isPrefabChild = false);

		static void m_OverWriteScenePrafab(ecs::EntityID id);

	private:

		

	};

}




#endif PREFAB_H

