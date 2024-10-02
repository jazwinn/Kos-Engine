#ifndef TEXTCOM_H
#define TEXTCOM_H

#include <string>
#include "Component.h"


namespace ecs {


	class TextComponent : public Component {

	public:

		std::string m_text{"Insert Text"};
		float m_fontSize{ 1 };

		float m_red{}, m_green{}, m_blue{};

	};

}




#endif TEXTCOM_H
