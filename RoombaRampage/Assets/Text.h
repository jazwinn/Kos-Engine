#ifndef FONT_H
#define FONT_H

#include <glm.hpp>
#include <glew.h>
#include <string>
#include <../Freetype_Font/include/ft2build.h>
#include <../Freetype_Font/include/freetype/freetype.h>

namespace text {

	class CharacterData
	{
	public:
		GLuint m_textureID;       // ID handle of the glyph texture
		glm::ivec2 m_size;     // Size of the glyph
		glm::ivec2 m_bearing;  // Offset from baseline to left/top of glyph
		GLuint m_advance;         // Offset to advance to the next glyph
	};

	class Font {

	public:

		static void LoadFont(std::string file, unsigned int fontsize);
	};

}




#endif FONT_H
