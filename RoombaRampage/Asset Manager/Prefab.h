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

#include <vector>
#include <unordered_map>
#include <string>
#include "../ECS/ECSList.h"


namespace prefab {

	class Prefab {

	public:
		static void m_CreatePrefab(std::string prefabscene, std::string insertscene = {});

		static void m_SaveEntitytoPrefab(ecs::EntityID);

		static void m_AssignEntitytoPrefab(std::string prefab, ecs::EntityID id);

		static std::unordered_map < std::string, std::vector<ecs::EntityID>> m_prefabMap;

	private:

		

	};

}




#endif PREFAB_H

