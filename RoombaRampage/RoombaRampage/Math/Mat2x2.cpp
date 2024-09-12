#include "../Math/Mat2x2.h"

namespace Mat2x2 {
	void classMat2x2::mat2Swap(classMat2x2& lhs, classMat2x2& rhs) {
		std::swap(lhs, rhs);
	}

	classMat2x2 classMat2x2::mat2Transpose() const {
		return classMat2x2(e00, e01, e10, e11);
	}

	classMat2x2 classMat2x2::mat2Abs() const {
		float lvAb1 = e00, lvAb2 = e10, lvAb3 = e01, lvAb4 = e11;
		lvAb1 = lvAb1 < 0.f ? -lvAb1 : lvAb1;
		lvAb2 = lvAb2 < 0.f ? -lvAb2 : lvAb2;
		lvAb3 = lvAb3 < 0.f ? -lvAb3 : lvAb3;
		lvAb4 = lvAb4 < 0.f ? -lvAb4 : lvAb4;
		return classMat2x2(lvAb1, lvAb2, lvAb3, lvAb4);
	}

	classMat2x2 classMat2x2::mat2Adjunct() const {
		return classMat2x2{ e11, -e10, -e01, e00 };
	}

	float classMat2x2::mat2Determinant() const {
		return ((e00 * e11) - (e10 * e01));
	}

	classMat2x2 classMat2x2::mat2Inverse() const {
		classMat2x2 lvRet;
		classMat2x2 lvAdj = this->mat2Adjunct();
		float lvDet = this->mat2Determinant();
		if (lvDet == 0) {
			return lvRet;
		}
		float lvMul = 1 / lvDet;
		lvRet.e00 = lvAdj.e00 * lvMul;
		lvRet.e01 = lvAdj.e01 * lvMul;
		lvRet.e10 = lvAdj.e10 * lvMul;
		lvRet.e11 = lvAdj.e11 * lvMul;
		return lvRet;
	}


	classMat2x2& classMat2x2::operator=(const classMat2x2& rhs) {
		classMat2x2 lvTemp(rhs);
		mat2Swap(*this, lvTemp);
		return *this;
	}

	classMat2x2& classMat2x2::operator*=(const classMat2x2& rhs) {
		classMat2x2 lvTemp;
		lvTemp.e00 = e00 * rhs.e00 + e01 * rhs.e10;
		lvTemp.e10 = e10 * rhs.e00 + e11 * rhs.e10;
		lvTemp.e01 = e00 * rhs.e01 + e01 * rhs.e11;
		lvTemp.e11 = e10 * rhs.e01 + e11 * rhs.e11;
		*this = lvTemp;
		return *this;
	}


	classMat2x2& classMat2x2::operator +=(const classMat2x2& rhs) {
		e00 += rhs.e00;
		e01 += rhs.e01;
		e10 += rhs.e10;
		e11 += rhs.e11;
		return *this;
	}

	classMat2x2& classMat2x2::operator -=(const classMat2x2& rhs) {
		e00 -= rhs.e00;
		e01 -= rhs.e01;
		e10 -= rhs.e10;
		e11 -= rhs.e11;
		return *this;
	}

	classMat2x2 operator*(const classMat2x2& lhs, const classMat2x2& rhs) {
		classMat2x2 lvTemp(lhs);
		lvTemp *= rhs;
		return lvTemp;
	}

	classMat2x2 operator+(const classMat2x2& lhs, const classMat2x2& rhs) {
		classMat2x2 lvTemp(lhs);
		lvTemp += rhs;
		return lvTemp;
	}


	classMat2x2 operator-(const classMat2x2& lhs, const classMat2x2& rhs) {
		classMat2x2 lvTemp(lhs);
		lvTemp -= rhs;
		return lvTemp;
	}
}