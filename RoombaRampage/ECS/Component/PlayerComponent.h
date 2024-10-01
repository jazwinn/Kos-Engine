#ifndef INPUTCOM_H
#define INPUTCOM_H

#include "Component.h"


namespace ecs {


	class PlayerComponent : public Component {

	public:

		bool m_Control{true};

	};

}




#endif INPUTCOM_H
