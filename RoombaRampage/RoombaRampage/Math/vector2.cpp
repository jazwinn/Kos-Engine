#include "../Math/vector2.h"


Vector2::Vec2::Vec2(float _x, float _y) {
    x = _x;
    y = _y;
}


Vector2::Vec2& Vector2::Vec2::operator += (const Vec2& rhs) {
    x = x + rhs.x;
    y = y + rhs.y;
    return *this;
}

Vector2::Vec2& Vector2::Vec2::operator -= (const Vec2& rhs) {
    x = x - rhs.x;
    y = y - rhs.y;
    return *this;
}

Vector2::Vec2& Vector2::Vec2::operator *= (float rhs) {
    x = x * rhs;
    y = y * rhs;
    return *this;
}

Vector2::Vec2& Vector2::Vec2::operator /= (float rhs) {
    x = x / rhs;
    y = y / rhs;
    return *this;
}

Vector2::Vec2 Vector2::Vec2::operator - () const {
    return Vec2(-x, -y);
}

Vector2::Vec2 Vector2::operator + (const Vector2::Vec2& lhs, const Vector2::Vec2& rhs) {
    float tempX = lhs.x + rhs.x;
    float tempY = lhs.y + rhs.y;
    return Vector2::Vec2(tempX, tempY);
}

Vector2::Vec2 Vector2::operator - (const Vector2::Vec2& lhs, const Vector2::Vec2& rhs) {
    float tempX = lhs.x - rhs.x;
    float tempY = lhs.y - rhs.y;
    return Vector2::Vec2(tempX, tempY);
}

Vector2::Vec2 Vector2::operator * (const Vector2::Vec2& lhs, float rhs) {
    float tempX = lhs.x * rhs;
    float tempY = lhs.y * rhs;
    return Vector2::Vec2(tempX, tempY);
}

Vector2::Vec2 Vector2::operator * (float lhs, const Vector2::Vec2& rhs) {
    float tempX = rhs.x * lhs;
    float tempY = rhs.y * lhs;
    return Vector2::Vec2(tempX, tempY);
}

Vector2::Vec2 Vector2::operator * (const Vec2& lhs, const Vec2& rhs) {
    float tempX = lhs.x * rhs.x;
    float tempY = lhs.y * rhs.y;
    return Vector2::Vec2(tempX, tempY);
}

Vector2::Vec2 Vector2::operator / (const Vector2::Vec2& lhs, float rhs) {
    float tempX = lhs.x / rhs;
    float tempY = lhs.y / rhs;
    return Vector2::Vec2(tempX, tempY);
}


void Vector2::Vec2::funcVec2Normalize(Vector2::Vec2& pResult, const Vector2::Vec2& pVec0) {
    Vector2::Vec2 temp = pVec0;
    float distance = Vector2::Vec2::funcVec2DLength(temp);
    pResult.x = temp.x / distance;
    pResult.y = temp.y / distance;
}

float  Vector2::Vec2::funcVec2DLength(const Vec2& pVec0) {
    float tempX = pVec0.x;
    float tempY = pVec0.y;

    float lengthSquare = tempX * tempX + tempY * tempY;
    return sqrtf(lengthSquare);

}

float  Vector2::Vec2::funcVec2DSquareLength(const Vec2& pVec0) {
    float tempX = pVec0.x;
    float tempY = pVec0.y;
    float lengthSquare = tempX * tempX + tempY * tempY;
    return lengthSquare;
}

float  Vector2::Vec2::funcVec2DDistance(const Vec2& pVec0, const Vec2& pVec1) {
    float tempDiffX = pVec0.x - pVec1.x;
    float tempDiffY = pVec0.x - pVec1.y;
    float lengthSquare = tempDiffX * tempDiffX + tempDiffY * tempDiffY;
    return sqrtf(lengthSquare);
}

float  Vector2::Vec2::funcVec2DSquareDistance(const Vec2& pVec0, const Vec2& pVec1) {
    float tempDiffX = pVec0.x - pVec1.x;
    float tempDiffY = pVec0.x - pVec1.y;
    float lengthSquare = tempDiffX * tempDiffX + tempDiffY * tempDiffY;
    return lengthSquare;
}

float  Vector2::Vec2::funcVec2DDotProduct(const Vec2& pVec0, const Vec2& pVec1) {
    float tempX = pVec0.x * pVec1.x;
    float tempY = pVec0.y * pVec1.y;
    float result = tempX + tempY;
    return result;
}

float Vector2::Vec2::funcVec2CrossProduct(const Vec2& pVec0, const Vec2& pVec1) {
    float temp1 = pVec0.x * pVec1.y;
    float temp2 = pVec0.y * pVec1.x;
    float result = temp1 - temp2;
    return result;
}