#ifndef MAT2x2_H
#define MAT2x2_H
#include <algorithm>
#include <math.h>


namespace Mat3x3 {
	constexpr float PI = 3.14159265359f;
	class classMat3x3 {
	public:
		union {
			struct {
				float e00, e01, e02;	//e00 e10 e20
				float e10, e11, e12;	//e01 e11 e21
				float e20, e21, e22;	//e02 e12 e22
			};
			float mat[3][3];
			float row[9];
		};

		classMat3x3() : e00(0.f), e01(0.f), e02(0.f), e10(0.f), e11(0.f), e12(0.f), e20(0.f), e21(0.f), e22(0.f) {}

		classMat3x3(float e1, float e2, float e3, float e4, float e5, float e6, float e7, float e8, float e9) :
			e00{ e1 }, e10{ e2 }, e20{ e3 },
			e01{ e4 }, e11{ e5 }, e21{ e6 },
			e02{ e7 }, e12{ e8 }, e22{ e9 } {}

		void mat3Swap(classMat3x3& lhs, classMat3x3& rhs);

		classMat3x3 mat3Adjunct() const;

		float mat3Determinant() const;

		classMat3x3& operator=(const classMat3x3& rhs);

		classMat3x3& operator*=(const classMat3x3& rhs);

		classMat3x3& operator+=(const classMat3x3& rhs);

		classMat3x3& operator-=(const classMat3x3& rhs);

		classMat3x3 mat3Abs() const;



	};

	void mat3Identity(classMat3x3& given);

	void mat3Inverse(const classMat3x3& givenIn, classMat3x3& givenOut);

	void mat3Translate(classMat3x3& given, float x, float y);

	void mat3Scale(classMat3x3& given, float x, float y);

	void mat3RotRad(classMat3x3& given, float ang);

	void mat3RotDeg(classMat3x3& given, float ang);

	classMat3x3 operator*(const classMat3x3& lhs, const classMat3x3& rhs);

	classMat3x3 operator+(const classMat3x3& lhs, const classMat3x3& rhs);

	classMat3x3 operator-(const classMat3x3& lhs, const classMat3x3& rhs);
}
#endif