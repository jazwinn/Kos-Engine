/******************************************************************/
/*!
\file      vector2.h
\author    Rayner Tan, raynerweichen.tan , 2301449
\par       raynerweichen.tan@digipen.edu
\date      Sept 28, 2024
\brief     Vector2 functions

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef VECTOR2_H
#define VECTOR2_H
#include <cstddef>
#include "mathlib.h"
namespace vector2 {
    class Vec2 {
    public:
        float m_x{};
        float m_y{};

        //constructor
        Vec2() : m_x(0.0f), m_y(0.0f) {};
        Vec2(float _x, float _y);

        Vec2& operator += (const Vec2& rhs);
        Vec2& operator -= (const Vec2& rhs);
        Vec2& operator *= (float rhs);
        Vec2& operator /= (float rhs);
        Vec2 operator -() const;

        static void  m_funcVec2Normalize(Vec2& pResult, const Vec2& pVec0);

        static float m_funcVec2DLength(const Vec2& pVec0);

        static float m_funcVec2DSquareLength(const Vec2& pVec0);

        static float m_funcVec2DDistance(const Vec2& pVec0, const Vec2& pVec1);

        static float m_funcVec2DSquareDistance(const Vec2& pVec0, const Vec2& pVec1);

        static float m_funcVec2DDotProduct(const Vec2& pVec0, const Vec2& pVec1);

        static float m_funcVec2CrossProduct(const Vec2& pVec0, const Vec2& pVec1);

    };

    Vec2 operator + (const Vec2& lhs, const Vec2& rhs);
    Vec2 operator - (const Vec2& lhs, const Vec2& rhs);
    Vec2 operator * (const Vec2& lhs, float rhs);
    Vec2 operator * (float lhs, const Vec2& rhs);
    Vec2 operator / (const Vec2& lhs, float rhs);
    Vec2 operator * (const Vec2& lhs, const Vec2& rhs); //dot product

}
#endif VECTOR2_H
