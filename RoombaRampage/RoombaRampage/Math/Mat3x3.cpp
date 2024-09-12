#include "../Math/Mat3x3.h"

namespace Mat3x3{
	void classMat3x3::mat3Swap(classMat3x3 & lhs, classMat3x3 & rhs) {
		std::swap(lhs, rhs);
	}

	classMat3x3 classMat3x3::mat3Adjunct() const {
		classMat3x3 lvRet;

		lvRet.e00 = e11 * e22 - e21 * e12;
		lvRet.e01 = e02 * e21 - e01 * e22;
		lvRet.e02 = e01 * e12 - e02 * e11;

		lvRet.e10 = e12 * e20 - e10 * e22;
		lvRet.e11 - e00 * e22 - e02 * e20;
		lvRet.e12 = e10 * e02 - e00 * e12;

		lvRet.e20 = e10 * e21 - e20 * e11;
		lvRet.e21 = e20 * e01 - e00 * e21;
		lvRet.e22 = e00 * e11 - e10 * e01;

		return lvRet;
	}

	float classMat3x3::mat3Determinant() const {
		float lvRet;
		lvRet = (e00 * (e11 * e22 - e21 * e12)) - (e10 * (e01 * e22 - e02 * e21)) + (e20 * (e01 * e12 - e02 * e11));
		return lvRet;
	}

	classMat3x3 & classMat3x3::operator=(const classMat3x3 & rhs) {
		classMat3x3 lvTemp(rhs);
		mat3Swap(*this, lvTemp);
		return *this;
	}

	classMat3x3 & classMat3x3::operator*=(const classMat3x3 & rhs) {
		classMat3x3 lvTemp;
		lvTemp.e00 = (e00 * rhs.e00) + (e01 * rhs.e10) + (e02 * rhs.e20);
		lvTemp.e01 = (e10 * rhs.e00) + (e11 * rhs.e10) + (e12 * rhs.e20);
		lvTemp.e02 = (e20 * rhs.e00) + (e21 * rhs.e10) + (e22 * rhs.e20);

		lvTemp.e10 = (e00 * rhs.e01) + (e01 * rhs.e11) + (e02 * rhs.e21);
		lvTemp.e11 = (e10 * rhs.e01) + (e11 * rhs.e11) + (e12 * rhs.e21);
		lvTemp.e12 = (e20 * rhs.e01) + (e21 * rhs.e11) + (e22 * rhs.e21);

		lvTemp.e20 = (e00 * rhs.e02) + (e01 * rhs.e12) + (e02 * rhs.e22);
		lvTemp.e21 = (e10 * rhs.e02) + (e11 * rhs.e12) + (e12 * rhs.e22);
		lvTemp.e22 = (e20 * rhs.e02) + (e21 * rhs.e12) + (e22 * rhs.e22);
		*this = lvTemp;
		return *this;
	}

	classMat3x3& classMat3x3::operator+=(const classMat3x3& rhs) {
		e00 += rhs.e00;
		e01 += rhs.e01;
		e02 += rhs.e02;

		e10 += rhs.e10;
		e11 += rhs.e11;
		e12 += rhs.e12;

		e20 += rhs.e20;
		e21 += rhs.e21;
		e22 += rhs.e22;

		return *this;
	}

	classMat3x3& classMat3x3::operator-=(const classMat3x3& rhs) {
		e00 -= rhs.e00;
		e01 -= rhs.e01;
		e02 -= rhs.e02;

		e10 -= rhs.e10;
		e11 -= rhs.e11;
		e12 -= rhs.e12;

		e20 -= rhs.e20;
		e21 -= rhs.e21;
		e22 -= rhs.e22;

		return *this;
	}

	classMat3x3 classMat3x3::mat3Abs() const {
		float lvAb1 = e00, lvAb2 = e10, lvAb3 = e20;
		float lvAb4 = e01, lvAb5 = e11, lvAb6 = e21;
		float lvAb7 = e02, lvAb8 = e12, lvAb9 = e22;
		lvAb1 = lvAb1 < 0.f ? -lvAb1 : lvAb1;
		lvAb2 = lvAb2 < 0.f ? -lvAb2 : lvAb2;
		lvAb3 = lvAb3 < 0.f ? -lvAb3 : lvAb3;
		lvAb4 = lvAb4 < 0.f ? -lvAb4 : lvAb4;
		lvAb5 = lvAb5 < 0.f ? -lvAb5 : lvAb5;
		lvAb6 = lvAb6 < 0.f ? -lvAb6 : lvAb6;
		lvAb7 = lvAb7 < 0.f ? -lvAb7 : lvAb7;
		lvAb8 = lvAb8 < 0.f ? -lvAb8 : lvAb8;
		lvAb9 = lvAb9 < 0.f ? -lvAb9 : lvAb9;
		return classMat3x3(lvAb1,lvAb2,lvAb3,lvAb4,lvAb5,lvAb6,lvAb7,lvAb8,lvAb9);
	}

	void mat3Identity(classMat3x3 & given) {
		given.e00 = 1;
		given.e01 = 0;
		given.e02 = 0;

		given.e10 = 0;
		given.e11 = 1;
		given.e12 = 0;

		given.e20 = 0;
		given.e21 = 0;
		given.e22 = 1;
	}

	void mat3Inverse(const classMat3x3 & givenIn, classMat3x3 & givenOut) {
		float lvDet = givenIn.mat3Determinant();
		if (lvDet == 0) {
			return;
		}
		float lvMul = 1 / lvDet;

		classMat3x3 lvAdj = givenIn.mat3Adjunct();
		givenOut.e00 = lvAdj.e00 * lvMul;
		givenOut.e01 = lvAdj.e01 * lvMul;
		givenOut.e02 = lvAdj.e02 * lvMul;

		givenOut.e10 = lvAdj.e10 * lvMul;
		givenOut.e11 = lvAdj.e11 * lvMul;
		givenOut.e12 = lvAdj.e12 * lvMul;

		givenOut.e20 = lvAdj.e20 * lvMul;
		givenOut.e21 = lvAdj.e21 * lvMul;
		givenOut.e22 = lvAdj.e22 * lvMul;
	}

	void mat3Translate(classMat3x3 & given, float x, float y) {
		mat3Identity(given);
		given.e02 = x;
		given.e12 = y;
	}

	void mat3Scale(classMat3x3 & given, float x, float y) {
		mat3Identity(given);
		given.e00 = x;
		given.e11 = y;
	}

	void mat3RotRad(classMat3x3 & given, float ang) {
		mat3Identity(given);
		given.e00 = cosf(ang);
		given.e01 = -sinf(ang);
		given.e10 = sinf(ang);
		given.e11 = cosf(ang);
	}

	void mat3RotDeg(classMat3x3 & given, float ang) {
		float lvRad = (ang * (PI / 180.f));
		mat3RotRad(given, ang);
	}

	classMat3x3 operator*(const classMat3x3 & lhs, const classMat3x3 & rhs) {
		classMat3x3 lvTemp(lhs);
		lvTemp *= rhs;
		return lvTemp;
	}


	classMat3x3 operator+(const classMat3x3 & lhs, const classMat3x3 & rhs) {
		classMat3x3 lvTemp(lhs);
		lvTemp += rhs;
		return lvTemp;
	}

	classMat3x3 operator-(const classMat3x3 & lhs, const classMat3x3 & rhs) {
		classMat3x3 lvTemp(lhs);
		lvTemp -= rhs;
		return lvTemp;
	}
}