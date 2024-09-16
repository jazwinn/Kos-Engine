#ifndef VECTOR2_H
#define VECTOR2_H
#include <cstddef>
#include "mathlib.h"
namespace Vector2 {
    class Vec2 {
    public:
        struct {
            float x, y;
        };
        float m[2]{};

        //constructor
        Vec2() : x(0.0f), y(0.0f) {};
        Vec2(float _x, float _y);

        //copy constructor
        //Vec2& operator=(const Vec2& rhs);
        //Vec2(const Vec2 & rhs);

        Vec2& operator += (const Vec2& rhs);
        Vec2& operator -= (const Vec2& rhs);
        Vec2& operator *= (float rhs);
        Vec2& operator /= (float rhs);
        Vec2 operator -() const;

        static void funcVec2Normalize(Vec2& pResult, const Vec2& pVec0);

        static float funcVec2DLength(const Vec2& pVec0);

        static float funcVec2DSquareLength(const Vec2& pVec0);

        static float funcVec2DDistance(const Vec2& pVec0, const Vec2& pVec1);

        static float funcVec2DSquareDistance(const Vec2& pVec0, const Vec2& pVec1);

        static float funcVec2DDotProduct(const Vec2& pVec0, const Vec2& pVec1);

        static float funcVec2CrossProduct(const Vec2& pVec0, const Vec2& pVec1);

    };

    Vec2 operator + (const Vec2& lhs, const Vec2& rhs);
    Vec2 operator - (const Vec2& lhs, const Vec2& rhs);
    Vec2 operator * (const Vec2& lhs, float rhs);
    Vec2 operator * (float lhs, const Vec2& rhs);
    Vec2 operator / (const Vec2& lhs, float rhs);
    Vec2 operator * (const Vec2& lhs, const Vec2& rhs); //dot product

}
#endif VECTOR2_H