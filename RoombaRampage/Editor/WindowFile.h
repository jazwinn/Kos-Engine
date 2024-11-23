/******************************************************************/
/*!
\file      WindowFile.h
\author    Clarence Boey
\par       c.boey@digipen.edu
\date      Sept 30, 2024
\brief     This file contains the declaration to open the file explorer.


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#ifndef WINFILE_H
#define WINFILE_H

#include "../Config/pch.h"
#include <Windows.h>
#include <commdlg.h>

namespace file {

	class FileWindow {

	public:	

		static std::string m_OpenfileDialog(char* filepath);

		//static std::string m_BackupAndDelete(const std::filesystem::path& path);

	};



}

#endif WINFILE_H