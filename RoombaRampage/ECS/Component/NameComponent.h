#ifndef NAMECOM_H
#define NAMECOM_H

#include <string>
#include "Component.h"


namespace ecs {


	class NameComponent : public Component {

	public:

		std::string m_entityName;

	};

}




#endif NAMECOM_H