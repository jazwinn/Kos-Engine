#ifndef TEXTCOM_H
#define TEXTCOM_H

#include <string>
#include "Component.h"


namespace ecs {


	class TextComponent : public Component {

	public:

		std::string m_text;
		unsigned int m_fontSize;

	};

}




#endif TEXTCOM_H
