#include "../Config/pch.h"
#include "WindowFile.h"

namespace file {
	std::string FileWindow::m_OpenfileDialog([[maybe_unused]] char* filePath)
	{
        //OPENING THIS CAUSE FPS TO DROP BY HALF?????????
        OPENFILENAMEA ofn;
        CHAR fileSize[256] = { 0 };
        //CHAR directory[256] = { 0 };
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = NULL; // Handle to the owner window (can be NULL)
        ofn.lpstrFile = fileSize; // Buffer to store the file path
        ofn.lpstrFile[0] = '\0';  // Initial null-terminated string
        ofn.nMaxFile = sizeof(fileSize);
        ofn.lpstrFilter = "All\0*.*\0Text Files\0*.TXT\0";  // File filters
        ofn.nFilterIndex = 1; // Filter index
        ofn.lpstrDefExt = "json"; // Default extension
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;



        if (GetOpenFileNameA(&ofn) == TRUE) {
            return ofn.lpstrFile;
        }

		return std::string();
	}
}


