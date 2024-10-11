#include "Font.h"
#include <iostream>

namespace fontmanager {
    // Constructor to initialize the font
    Font::Font(const std::string& fontPath, unsigned int fontSize)
        : face(nullptr)
    {
        // Initialize FreeType Library
        if (FT_Init_FreeType(&ft)) {
            std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
            return;
        }

        // Load font as face
        if (FT_New_Face(ft, fontPath.c_str(), 0, &face)) {
            std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
            return;
        }

        // Load the ASCII characters (LOAD ASCII Characters)
        LoadCharacters(fontSize);
    }

    // Destructor to clean up the font resources
    Font::~Font() {
        // Delete textures
        for (auto& charPair : Characters) {
            glDeleteTextures(1, &charPair.second.textureID);
        }

        // Clean up FreeType resources
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
    }

    // Helper function to load ASCII characters
    void Font::LoadCharacters(unsigned int fontSize) {
        FT_Set_Pixel_Sizes(face, 0, fontSize);  // Set the font size (MAKE SURE ITS AN INT)

        // Load first 128 ASCII characters
        for (unsigned char c = 0; c < 128; c++) {
            // Load character glyph --> This one is to check if the font load properly
            if (FT_Load_Char(face, c, FT_LOAD_TARGET_MONO)) {
                std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
                continue;
            }

            // Generate texture for the character
            GLuint texture;
            glGenTextures(1, &texture);
            glBindTexture(GL_TEXTURE_2D, texture);
            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                face->glyph->bitmap.width,
                face->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
            );

            // Set texture options
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

            // Store character in Characters map
            Character character = {
                texture,
                //TOCHECK
                vector2::Vec2(static_cast<float>(face->glyph->bitmap.width), static_cast<float>(face->glyph->bitmap.rows)),
                vector2::Vec2(static_cast<float>(face->glyph->bitmap_left), static_cast<float>(face->glyph->bitmap_top)),
                static_cast<GLuint>(face->glyph->advance.x)
            };

            Characters.insert(std::pair<char, Character>(c, character));
        }
    }
}


