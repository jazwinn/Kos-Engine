/*!
\file      Mat3x3.h
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      Sept 28, 2024
\brief     This file holds the definition Matrix of 3x3 size member functions and non member functions along with operator overloads

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#include "../Math/Mat3x3.h"

namespace mat3x3{
	/**
	 \brief		this function swaps the data of 2 matrixes
	 **/
	void Mat3x3::m_Mat3Swap(Mat3x3& lhs, Mat3x3 & rhs) {
		std::swap(lhs, rhs);
	}

	/**
	 \brief		this function is a member function meant for assigning an adjunct version of the callee
	 **/
	Mat3x3 Mat3x3::m_Mat3Adjunct() const {
		Mat3x3 lvRet;

		lvRet.m_e00 = m_e11 * m_e22 - m_e21 * m_e12;
		lvRet.m_e01 = m_e02 * m_e21 - m_e01 * m_e22;
		lvRet.m_e02 = m_e01 * m_e12 - m_e02 * m_e11;

		lvRet.m_e10 = m_e12 * m_e20 - m_e10 * m_e22;
		lvRet.m_e11 = m_e00 * m_e22 - m_e02 * m_e20;
		lvRet.m_e12 = m_e10 * m_e02 - m_e00 * m_e12;

		lvRet.m_e20 = m_e10 * m_e21 - m_e20 * m_e11;
		lvRet.m_e21 = m_e20 * m_e01 - m_e00 * m_e21;
		lvRet.m_e22 = m_e00 * m_e11 - m_e10 * m_e01;

		return lvRet;
	}

	/**
	 \brief		this function is a member function meant for getting the determinant of the callee
	 **/
	float Mat3x3::m_Mat3Determinant() const {
		float lvRet;
		lvRet = (m_e00 * (m_e11 * m_e22 - m_e21 * m_e12)) - (m_e10 * (m_e01 * m_e22 - m_e02 * m_e21)) + (m_e20 * (m_e01 * m_e12 - m_e02 * m_e11));
		return lvRet;
	}

	/**
	 \brief		this function is a member function overload for the = sign also known as the copy assignment
	 **/
	Mat3x3 & Mat3x3::operator=(const Mat3x3 & rhs) {
		Mat3x3 lvTemp(rhs);
		m_Mat3Swap(*this, lvTemp);
		return *this;
	}

	/**
	 \brief		this function is a member function overload ofthe *= sign used to multiply a Mat2x2 with the callee
	 **/
	Mat3x3 & Mat3x3::operator*=(const Mat3x3 & rhs) {
		Mat3x3 lvTemp;
		lvTemp.m_e00 = (m_e00 * rhs.m_e00) + (m_e01 * rhs.m_e10) + (m_e02 * rhs.m_e20);
		lvTemp.m_e01 = (m_e10 * rhs.m_e00) + (m_e11 * rhs.m_e10) + (m_e12 * rhs.m_e20);
		lvTemp.m_e02 = (m_e20 * rhs.m_e00) + (m_e21 * rhs.m_e10) + (m_e22 * rhs.m_e20);

		lvTemp.m_e10 = (m_e00 * rhs.m_e01) + (m_e01 * rhs.m_e11) + (m_e02 * rhs.m_e21);
		lvTemp.m_e11 = (m_e10 * rhs.m_e01) + (m_e11 * rhs.m_e11) + (m_e12 * rhs.m_e21);
		lvTemp.m_e12 = (m_e20 * rhs.m_e01) + (m_e21 * rhs.m_e11) + (m_e22 * rhs.m_e21);

		lvTemp.m_e20 = (m_e00 * rhs.m_e02) + (m_e01 * rhs.m_e12) + (m_e02 * rhs.m_e22);
		lvTemp.m_e21 = (m_e10 * rhs.m_e02) + (m_e11 * rhs.m_e12) + (m_e12 * rhs.m_e22);
		lvTemp.m_e22 = (m_e20 * rhs.m_e02) + (m_e21 * rhs.m_e12) + (m_e22 * rhs.m_e22);
		*this = lvTemp;
		return *this;
	}

	/**
	 \brief		this function is a member function overload of the += sign used to add another matrix with the callee
	 **/
	Mat3x3& Mat3x3::operator+=(const Mat3x3& rhs) {
		m_e00 += rhs.m_e00;
		m_e01 += rhs.m_e01;
		m_e02 += rhs.m_e02;

		m_e10 += rhs.m_e10;
		m_e11 += rhs.m_e11;
		m_e12 += rhs.m_e12;

		m_e20 += rhs.m_e20;
		m_e21 += rhs.m_e21;
		m_e22 += rhs.m_e22;

		return *this;
	}

	/**
	 \brief		this function is a member function overload of the -= sign used to subtract another matrix with the callee
	 **/
	Mat3x3& Mat3x3::operator-=(const Mat3x3& rhs) {
		m_e00 -= rhs.m_e00;
		m_e01 -= rhs.m_e01;
		m_e02 -= rhs.m_e02;

		m_e10 -= rhs.m_e10;
		m_e11 -= rhs.m_e11;
		m_e12 -= rhs.m_e12;

		m_e20 -= rhs.m_e20;
		m_e21 -= rhs.m_e21;
		m_e22 -= rhs.m_e22;

		return *this;
	}

	/**
	 \brief		this function is a member function meant for assigning a fully positive version of the callee
	 **/
	Mat3x3 Mat3x3::m_Mat3Abs() const {
		float lvAb1 = m_e00, lvAb2 = m_e10, lvAb3 = m_e20;
		float lvAb4 = m_e01, lvAb5 = m_e11, lvAb6 = m_e21;
		float lvAb7 = m_e02, lvAb8 = m_e12, lvAb9 = m_e22;
		lvAb1 = lvAb1 < 0.f ? -lvAb1 : lvAb1;
		lvAb2 = lvAb2 < 0.f ? -lvAb2 : lvAb2;
		lvAb3 = lvAb3 < 0.f ? -lvAb3 : lvAb3;
		lvAb4 = lvAb4 < 0.f ? -lvAb4 : lvAb4;
		lvAb5 = lvAb5 < 0.f ? -lvAb5 : lvAb5;
		lvAb6 = lvAb6 < 0.f ? -lvAb6 : lvAb6;
		lvAb7 = lvAb7 < 0.f ? -lvAb7 : lvAb7;
		lvAb8 = lvAb8 < 0.f ? -lvAb8 : lvAb8;
		lvAb9 = lvAb9 < 0.f ? -lvAb9 : lvAb9;
		return Mat3x3(lvAb1,lvAb2,lvAb3,lvAb4,lvAb5,lvAb6,lvAb7,lvAb8,lvAb9);
	}

	/**
	 \brief		this function is a meant for changing a given Matrix into an identity matrix
	 **/
	void Mat3Identity(Mat3x3 & given) {
		given.m_e00 = 1;
		given.m_e01 = 0;
		given.m_e02 = 0;

		given.m_e10 = 0;
		given.m_e11 = 1;
		given.m_e12 = 0;

		given.m_e20 = 0;
		given.m_e21 = 0;
		given.m_e22 = 1;
	}

	/**
	 \brief		this function is a meant for inversing a given matrix and changing the value of the given resultant matrix
	 **/
	void mat3Inverse(const Mat3x3 & givenIn, Mat3x3 & givenOut) {
		float lvDet = givenIn.m_Mat3Determinant();
		if (lvDet == 0) {
			return;
		}
		float lvMul = 1 / lvDet;

		Mat3x3 lvAdj = givenIn.m_Mat3Adjunct();
		givenOut.m_e00 = lvAdj.m_e00 * lvMul;
		givenOut.m_e01 = lvAdj.m_e01 * lvMul;
		givenOut.m_e02 = lvAdj.m_e02 * lvMul;

		givenOut.m_e10 = lvAdj.m_e10 * lvMul;
		givenOut.m_e11 = lvAdj.m_e11 * lvMul;
		givenOut.m_e12 = lvAdj.m_e12 * lvMul;

		givenOut.m_e20 = lvAdj.m_e20 * lvMul;
		givenOut.m_e21 = lvAdj.m_e21 * lvMul;
		givenOut.m_e22 = lvAdj.m_e22 * lvMul;
	}

	/**
	 \brief		this function is a meant for changing a given Matrix into a translation matrix with x and y
	 **/
	void Mat3Translate(Mat3x3 & given, float x, float y) {
		Mat3Identity(given);
		given.m_e02 = x;
		given.m_e12 = y;
	}

	/**
	 \brief		this function is a meant for changing a given Matrix into a scale matrix with x and y
	 **/
	void Mat3Scale(Mat3x3 & given, float x, float y) {
		Mat3Identity(given);
		given.m_e00 = x;
		given.m_e11 = y;
	}

	/**
	 \brief		this function is a meant for changing a given Matrix into a rotation matrix with a given angle in radian
	 **/
	void Mat3RotRad(Mat3x3 & given, float ang) {
		Mat3Identity(given);
		given.m_e00 = cosf(ang);
		given.m_e01 = -sinf(ang);
		given.m_e10 = sinf(ang);
		given.m_e11 = cosf(ang);
	}

	/**
	 \brief		this function is a meant for changing a given Matrix into a rotation matrix by converting an angle in degree to radian
	 **/
	void Mat3RotDeg(Mat3x3 & given, float ang) {
		float lvRad = (ang * (PI / 180.f));
		Mat3RotRad(given, lvRad);
	}

	/**
	 \brief		this is a operator overload of the * sign used to multiply 2 matrixes together without changing the matrixes themselves
	 **/
	Mat3x3 operator*(const Mat3x3 & lhs, const Mat3x3 & rhs) {
		Mat3x3 lvTemp(lhs);
		lvTemp *= rhs;
		return lvTemp;
	}

	/**
	 \brief		this function is a operator overload of the + sign used to add 2 matrixes together without changing the matrixes themselves
	 **/
	Mat3x3 operator+(const Mat3x3 & lhs, const Mat3x3 & rhs) {
		Mat3x3 lvTemp(lhs);
		lvTemp += rhs;
		return lvTemp;
	}

	/**
	 \brief		this function is a operator overload of the - sign used to subtract 1 matrix from another without changing the matrixes themselves
	 **/
	Mat3x3 operator-(const Mat3x3 & lhs, const Mat3x3 & rhs) {
		Mat3x3 lvTemp(lhs);
		lvTemp -= rhs;
		return lvTemp;
	}
}