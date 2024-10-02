/*!
\file      Mat2x2.h
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      Sept 28, 2024
\brief     This file holds the definition for a Matrix of 2x2 size member functions and non member functions along with operator overloads

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#include "../Math/Mat2x2.h"

namespace mat2x2 {
	/**
	 \brief		this function swaps the data of 2 matrixes
	 **/
	void Mat2x2::m_Mat2Swap(Mat2x2& lhs, Mat2x2& rhs) {
		std::swap(lhs, rhs);
	}

	/**
	 \brief		This function is a member function meant for assigning a transposed version of the callee
	 **/
	Mat2x2 Mat2x2::m_Mat2Transpose() const {
		return Mat2x2(m_e00, m_e01, m_e10, m_e11);
	}

	/**
	 \brief		this function is a member function meant for assigning a fully positive version of the callee
	 **/
	Mat2x2 Mat2x2::m_Mat2Abs() const {
		float lvAb1 = m_e00, lvAb2 = m_e10, lvAb3 = m_e01, lvAb4 = m_e11;
		lvAb1 = lvAb1 < 0.f ? -lvAb1 : lvAb1;
		lvAb2 = lvAb2 < 0.f ? -lvAb2 : lvAb2;
		lvAb3 = lvAb3 < 0.f ? -lvAb3 : lvAb3;
		lvAb4 = lvAb4 < 0.f ? -lvAb4 : lvAb4;
		return Mat2x2(lvAb1, lvAb2, lvAb3, lvAb4);
	}

	/**
	 \brief		this function is a member function meant for assigning an adjunct version of the callee
	 **/
	Mat2x2 Mat2x2::m_Mat2Adjunct() const {
		return Mat2x2{ m_e11, -m_e10, -m_e01, m_e00 };
	}

	/**
	 \brief		this function is a member function meant for getting the determinant of the callee
	 **/
	float Mat2x2::m_Mat2Determinant() const {
		return ((m_e00 * m_e11) - (m_e10 * m_e01));
	}
	
	/**
	 \brief		this function is a member function meant for assigning an inverse version of the callee
	 **/
	Mat2x2 Mat2x2::m_Mat2Inverse() const {
		Mat2x2 lvRet;
		Mat2x2 lvAdj = this->m_Mat2Adjunct();
		float lvDet = this->m_Mat2Determinant();
		if (lvDet == 0) {
			return lvRet;
		}
		float lvMul = 1 / lvDet;
		lvRet.m_e00 = lvAdj.m_e00 * lvMul;
		lvRet.m_e01 = lvAdj.m_e01 * lvMul;
		lvRet.m_e10 = lvAdj.m_e10 * lvMul;
		lvRet.m_e11 = lvAdj.m_e11 * lvMul;
		return lvRet;
	}

	/**
	 \brief		this function is a member operator overload for the = sign also known as the copy assignment
	 **/
	Mat2x2& Mat2x2::operator=(const Mat2x2& rhs) {
		Mat2x2 lvTemp(rhs);
		m_Mat2Swap(*this, lvTemp);
		return *this;
	}

	/**
	 \brief		this function is a member operator overload ofthe *= sign used to multiply a Mat2x2 with the callee
	 **/
	Mat2x2& Mat2x2::operator*=(const Mat2x2& rhs) {
		Mat2x2 lvTemp;
		lvTemp.m_e00 = m_e00 * rhs.m_e00 + m_e01 * rhs.m_e10;
		lvTemp.m_e10 = m_e10 * rhs.m_e00 + m_e11 * rhs.m_e10;
		lvTemp.m_e01 = m_e00 * rhs.m_e01 + m_e01 * rhs.m_e11;
		lvTemp.m_e11 = m_e10 * rhs.m_e01 + m_e11 * rhs.m_e11;
		*this = lvTemp;
		return *this;
	}

	/**
	 \brief		this function is a member operator overload of the += sign used to add another matrix with the callee
	 **/
	Mat2x2& Mat2x2::operator +=(const Mat2x2& rhs) {
		m_e00 += rhs.m_e00;
		m_e01 += rhs.m_e01;
		m_e10 += rhs.m_e10;
		m_e11 += rhs.m_e11;
		return *this;
	}

	/**
	 \brief		this function is a member operator overload of the -= sign used to subtract another matrix with the callee
	 **/
	Mat2x2& Mat2x2::operator -=(const Mat2x2& rhs) {
		m_e00 -= rhs.m_e00;
		m_e01 -= rhs.m_e01;
		m_e10 -= rhs.m_e10;
		m_e11 -= rhs.m_e11;
		return *this;
	}

	/**
	 \brief		this function is a operator overload of the * sign used to multiply 2 matrixes together without changing the matrixes themselves
	 **/
	Mat2x2 operator*(const Mat2x2& lhs, const Mat2x2& rhs) {
		Mat2x2 lvTemp(lhs);
		lvTemp *= rhs;
		return lvTemp;
	}

	/**
	 \brief		this function is a operator overload of the + sign used to add 2 matrixes together without changing the matrixes themselves
	 **/
	Mat2x2 operator+(const Mat2x2& lhs, const Mat2x2& rhs) {
		Mat2x2 lvTemp(lhs);
		lvTemp += rhs;
		return lvTemp;
	}

	/**
	 \brief		this function is a operator overload of the - sign used to subtract 1 matrix from another without changing the matrixes themselves
	 **/
	Mat2x2 operator-(const Mat2x2& lhs, const Mat2x2& rhs) {
		Mat2x2 lvTemp(lhs);
		lvTemp -= rhs;
		return lvTemp;
	}

	/**
	 \brief		this function is a operator overload of the == sign used to compare 2 matrixes and return true if they are equal
	 **/
	bool operator==(const Mat2x2& lhs, const Mat2x2& rhs) {
		bool lvMe00 = false, lvMe01 = false, lvMe10 = false, lvMe11 = false;
		if (lhs.m_e00 == rhs.m_e00) {
			lvMe00 = true;
		}
		if (lhs.m_e01 == rhs.m_e01) {
			lvMe01 = true;
		}
		if (lhs.m_e10 == rhs.m_e10) {
			lvMe10 = true;
		}
		if (lhs.m_e11 == rhs.m_e11) {
			lvMe11 = true;
		}

		return lvMe00 && lvMe01 && lvMe10 && lvMe11;

	}

	/**
	 \brief		this function is a operator overload of the >= sign used to compare 2 matrixes and return true if the left matrix is more than or equal to the right matrix
	 **/
	bool operator>=(const Mat2x2& lhs, const Mat2x2& rhs) {
		bool lvMe00 = false, lvMe01 = false, lvMe10 = false, lvMe11 = false;
		if (lhs.m_e00 >= rhs.m_e00) {
			lvMe00 = true;
		}
		if (lhs.m_e01 >= rhs.m_e01) {
			lvMe01 = true;
		}
		if (lhs.m_e10 >= rhs.m_e10) {
			lvMe10 = true;
		}
		if (lhs.m_e11 >= rhs.m_e11) {
			lvMe11 = true;
		}

		return lvMe00 && lvMe01 && lvMe10 && lvMe11;
	}

	/**
	 \brief		this function is a operator overload of the <= sign used to compare 2 matrixes and return true if the left matrix is less than or equal to the right matrix
	 **/
	bool operator<=(const Mat2x2& lhs, const Mat2x2& rhs) {
		bool lvMe00 = false, lvMe01 = false, lvMe10 = false, lvMe11 = false;
		if (lhs.m_e00 <= rhs.m_e00) {
			lvMe00 = true;
		}
		if (lhs.m_e01 <= rhs.m_e01) {
			lvMe01 = true;
		}
		if (lhs.m_e10 <= rhs.m_e10) {
			lvMe10 = true;
		}
		if (lhs.m_e11 <= rhs.m_e11) {
			lvMe11 = true;
		}

		return lvMe00 && lvMe01 && lvMe10 && lvMe11;
	}

}