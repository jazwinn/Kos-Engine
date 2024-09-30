#ifndef FONT_H
#define FONT_H
#include <../Freetype_Font/include/ft2build.h>
#include <../Freetype_Font/include/freetype/freetype.h>
#include <../GLEW/include/GL/glew.h>
#include <../Math/vector2.h>
#include <vector>
#include <map>
#include <string>
#include <memory>

namespace fontmanager {

	// Character structure definition using our custom Vec2 class
	struct Character {
		GLuint textureID;       // ID handle of the glyph texture
		vector2::Vec2 size;     // Size of the glyph
		vector2::Vec2 bearing;  // Offset from baseline to left/top of glyph
		GLuint advance;         // Offset to advance to the next glyph
	};

	// Font class definition
	class Font {
	public:
		std::map<char, Character> Characters;  // Map to store characters and their properties
		FT_Library ft;
		FT_Face face;

		Font(const std::string& fontPath, unsigned int fontSize);  // Constructor
		~Font();  // Destructor to clean up

	private:
		void LoadCharacters(unsigned int fontSize);  // Helper function to load ASCII characters
	};
}
#endif