/*!
\file      Mat3x3.h
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      Sept 28, 2024
\brief     This file holds the class for a Matrix of 3x3 size, it's member functions and non member functions along with operator overloads

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#pragma warning (disable : 4201)
#pragma warning(disable : 26495)
#ifndef MAT2x2_H
#define MAT2x2_H
#include <algorithm>
#include <math.h>


namespace mat3x3 {
	constexpr float PI = 3.14159265359f;
	class Mat3x3 {
	public:
		union {
			struct {
				float m_e00, m_e01, m_e02;	//e00 e10 e20
				float m_e10, m_e11, m_e12;	//e01 e11 e21
				float m_e20, m_e21, m_e22;	//e02 e12 e22
			};
			[[maybe_unused]]float m_Mat[3][3];
			[[maybe_unused]] float m_Row[9];
		};

		Mat3x3() : m_e00(0.f), m_e01(0.f), m_e02(0.f), m_e10(0.f), m_e11(0.f), m_e12(0.f), m_e20(0.f), m_e21(0.f), m_e22(0.f) {}

		Mat3x3(float e1, float e2, float e3, float e4, float e5, float e6, float e7, float e8, float e9) :
			m_e00{ e1 }, m_e10{ e2 }, m_e20{ e3 },
			m_e01{ e4 }, m_e11{ e5 }, m_e21{ e6 },
			m_e02{ e7 }, m_e12{ e8 }, m_e22{ e9 } {}

		void m_Mat3Swap(Mat3x3& lhs, Mat3x3& rhs);

		Mat3x3 m_Mat3Adjunct() const;

		float m_Mat3Determinant() const;

		Mat3x3& operator=(const Mat3x3& rhs);

		Mat3x3& operator*=(const Mat3x3& rhs);

		Mat3x3& operator+=(const Mat3x3& rhs);

		Mat3x3& operator-=(const Mat3x3& rhs);

		Mat3x3 m_Mat3Abs() const;



	};

	void Mat3Identity(Mat3x3& given);

	void Mat3Inverse(const Mat3x3& givenIn, Mat3x3& givenOut);

	void Mat3Translate(Mat3x3& given, float x, float y);

	void Mat3Scale(Mat3x3& given, float x, float y);

	void Mat3RotRad(Mat3x3& given, float ang);

	void Mat3RotDeg(Mat3x3& given, float ang);

	Mat3x3 operator*(const Mat3x3& lhs, const Mat3x3& rhs);

	Mat3x3 operator+(const Mat3x3& lhs, const Mat3x3& rhs);

	Mat3x3 operator-(const Mat3x3& lhs, const Mat3x3& rhs);

	bool operator==(const Mat3x3& lhs, const Mat3x3& rhs);

	bool operator<=(const Mat3x3& lhs, const Mat3x3& rhs);

	bool operator>=(const Mat3x3& lhs, const Mat3x3& rhs);
}
#endif