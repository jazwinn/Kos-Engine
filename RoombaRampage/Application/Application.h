/******************************************************************/
/*!
\file      Application.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains the declaration to the main update 
		   loop of the engine. It initializes the various major 
		   systems and call upon them in the update loop.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef APP_H
#define APP_H

#include "Window.h"
#include "../Assets/Audio.h"
/******************************************************************/
/*!
\class     Application
\brief     The Papplciation class is responsible for the entire engine.
		   It calls onto all the systems.
*/
/******************************************************************/
namespace Application {



	class Application {

	public:

		/******************************************************************/
		/*!
		\fn        Init()
		\brief     Initialzie all systems, managers and pipeline
		*/
		/******************************************************************/
		static int Init();

		/******************************************************************/
		/*!
		\fn        Run()
		\brief     Contains the main update loop, calls onto all the programs
				   update functions.
		*/
		/******************************************************************/
		static int Run();
		/******************************************************************/
		/*!
		\fn        Cleanup()
		\brief     Calls upon the closing of the application. Calls the cleanup
				   functions of all the other systems.
		*/
		/******************************************************************/
		static int Cleanup();

		static AppWindow lvWindow;

	private:
		static gui::ImGuiHandler imgui_manager; //should only remain in Application


	};
}


#endif APP_H


