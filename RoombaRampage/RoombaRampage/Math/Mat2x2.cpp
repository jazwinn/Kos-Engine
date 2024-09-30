#include "../Math/Mat2x2.h"

namespace mat2x2 {
	void Mat2x2::m_Mat2Swap(Mat2x2& lhs, Mat2x2& rhs) {
		std::swap(lhs, rhs);
	}

	Mat2x2 Mat2x2::m_Mat2Transpose() const {
		return Mat2x2(m_e00, m_e01, m_e10, m_e11);
	}

	Mat2x2 Mat2x2::m_Mat2Abs() const {
		float lvAb1 = m_e00, lvAb2 = m_e10, lvAb3 = m_e01, lvAb4 = m_e11;
		lvAb1 = lvAb1 < 0.f ? -lvAb1 : lvAb1;
		lvAb2 = lvAb2 < 0.f ? -lvAb2 : lvAb2;
		lvAb3 = lvAb3 < 0.f ? -lvAb3 : lvAb3;
		lvAb4 = lvAb4 < 0.f ? -lvAb4 : lvAb4;
		return Mat2x2(lvAb1, lvAb2, lvAb3, lvAb4);
	}

	Mat2x2 Mat2x2::m_Mat2Adjunct() const {
		return Mat2x2{ m_e11, -m_e10, -m_e01, m_e00 };
	}

	float Mat2x2::m_Mat2Determinant() const {
		return ((m_e00 * m_e11) - (m_e10 * m_e01));
	}

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


	Mat2x2& Mat2x2::operator=(const Mat2x2& rhs) {
		Mat2x2 lvTemp(rhs);
		m_Mat2Swap(*this, lvTemp);
		return *this;
	}

	Mat2x2& Mat2x2::operator*=(const Mat2x2& rhs) {
		Mat2x2 lvTemp;
		lvTemp.m_e00 = m_e00 * rhs.m_e00 + m_e01 * rhs.m_e10;
		lvTemp.m_e10 = m_e10 * rhs.m_e00 + m_e11 * rhs.m_e10;
		lvTemp.m_e01 = m_e00 * rhs.m_e01 + m_e01 * rhs.m_e11;
		lvTemp.m_e11 = m_e10 * rhs.m_e01 + m_e11 * rhs.m_e11;
		*this = lvTemp;
		return *this;
	}


	Mat2x2& Mat2x2::operator +=(const Mat2x2& rhs) {
		m_e00 += rhs.m_e00;
		m_e01 += rhs.m_e01;
		m_e10 += rhs.m_e10;
		m_e11 += rhs.m_e11;
		return *this;
	}

	Mat2x2& Mat2x2::operator -=(const Mat2x2& rhs) {
		m_e00 -= rhs.m_e00;
		m_e01 -= rhs.m_e01;
		m_e10 -= rhs.m_e10;
		m_e11 -= rhs.m_e11;
		return *this;
	}

	Mat2x2 operator*(const Mat2x2& lhs, const Mat2x2& rhs) {
		Mat2x2 lvTemp(lhs);
		lvTemp *= rhs;
		return lvTemp;
	}

	Mat2x2 operator+(const Mat2x2& lhs, const Mat2x2& rhs) {
		Mat2x2 lvTemp(lhs);
		lvTemp += rhs;
		return lvTemp;
	}


	Mat2x2 operator-(const Mat2x2& lhs, const Mat2x2& rhs) {
		Mat2x2 lvTemp(lhs);
		lvTemp -= rhs;
		return lvTemp;
	}
}