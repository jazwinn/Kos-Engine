/******************************************************************/
/*!
\file      Helper.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains varaibles that is called and used anywhere
		   in the application.


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef HELP_H
#define HELP_H

#include <memory>

namespace Helper {
/******************************************************************/
/*!
\class     Helpers
\brief     Contains varaibles that can be called in anywhere
*/
/******************************************************************/
	class Helpers {

	private:


	public:
		//singleton
		static Helpers* GetInstance() {
			if (!InstancePtr) {
				InstancePtr.reset(new Helpers{});
			}
			return InstancePtr.get();
		}

		float m_windowWidth;

		float m_windowHeight;

		float m_deltaTime;

		float m_fpsCap;

		float m_fps;

	private:
		static std::unique_ptr<Helpers> InstancePtr;

	};

}


#endif // !HELP_H



