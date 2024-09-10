#ifndef MAT2x2_H
#define MAT2x2_H
#include <algorithm>

namespace Mat2x2 {
	class classMat2x2 {
	public:
		union {
			struct {
				float e00, e01;
				float e10, e11;
			};

			float mat[2][2];
			float row[4];
		};

		classMat2x2() : e00(0.f), e10(0.f), e01(0.f), e11(0.f) {}

		classMat2x2(float e1, float e2, float e3, float e4) : e00{ e1 }, e10{ e2 }, e01{ e3 }, e11{ e4 } {}

		/*Mat2x2<T> identity(T val) const{
			return Mat2x2<T>(val, 0, 0, val);
		}*/

		void mat2Swap(classMat2x2& lhs, classMat2x2& rhs);

		classMat2x2 mat2Transpose() const;

		classMat2x2 mat2Abs() const;

		classMat2x2 mat2Adjunct() const;

		float mat2Determinant() const;

		classMat2x2 mat2Inverse() const;


		classMat2x2& operator=(const classMat2x2& rhs);

		classMat2x2& operator*=(const classMat2x2& rhs);

		classMat2x2& operator +=(const classMat2x2& rhs);

		classMat2x2& operator -=(const classMat2x2& rhs);
	};

	classMat2x2 operator*(const classMat2x2& lhs, const classMat2x2& rhs);

	classMat2x2 operator+(const classMat2x2& lhs, const classMat2x2& rhs);

	classMat2x2 operator-(const classMat2x2& lhs, const classMat2x2& rhs);
}


#endif

