/******************************************************************/
/*!
\file      Text.h
\author    Clarence Boey
\par       c.boey@digipen.edu
\date      Sept 29, 2024
\brief     This file contains the declaration of the Font and
           CharacterData classes, which provide functionality for
           loading and handling font data using FreeType and OpenGL.

           The CharacterData class holds information about individual
           glyphs, including texture ID, size, bearing, and advance.
           The Font class contains functionality to load fonts and
           store character data in an efficient manner for rendering
           text in OpenGL.

           This file uses the FreeType library for loading font files
           and converting them into textures.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#ifndef FONT_H
#define FONT_H

#include <glm.hpp>
#include <glew.h>
#include <string>
#include <../Freetype_Font/include/ft2build.h>
#include <../Freetype_Font/include/freetype/freetype.h>

namespace text {

    /******************************************************************/
    /*!
    \class   CharacterData
    \brief   Stores information about individual glyphs used for rendering text.

             This class holds various properties of a character glyph,
             including its texture ID, size, bearing (offset from baseline),
             and advance (distance to the next glyph).
    */
    /******************************************************************/
    class CharacterData
    {
    public:
        GLuint m_textureID;       ///< ID handle of the glyph texture
        glm::ivec2 m_size;        ///< Size of the glyph (width and height)
        glm::ivec2 m_bearing;     ///< Offset from the baseline to the left/top of the glyph
        GLuint m_advance;         ///< Offset to advance to the next glyph
    };

    /******************************************************************/
    /*!
    \class   Font
    \brief   Provides functionality to load fonts and manage glyph data.

             The Font class is responsible for loading font files using
             the FreeType library. It converts font glyphs into OpenGL
             textures that can be used for rendering text. The loaded
             glyphs are stored as instances of the CharacterData class.
    */
    /******************************************************************/
    class Font {

    public:

        /******************************************************************/
        /*!
        \fn      static void Font::LoadFont(std::string file)
        \brief   Loads a font from the specified file and prepares the
                 font data for use in rendering text.

        \param   file - The path to the font file to load.
        */
        /******************************************************************/
        static void LoadFont(std::string file);
    };

}

#endif // FONT_H
