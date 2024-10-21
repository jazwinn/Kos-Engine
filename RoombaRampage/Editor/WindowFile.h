#ifndef WINFILE_H
#define WINFILE_H

#include <Windows.h>
#include <commdlg.h>
#include <string>

namespace file {

	class FileWindow {

	public:	

		static std::string m_OpenfileDialog(char* filepath);



	};



}

#endif WINFILE_H