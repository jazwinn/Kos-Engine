#ifndef MAT2x2_H
#define MAT2x2_H
#include <algorithm>

namespace mat2x2 {
	class Mat2x2 {
	public:
		union {
			struct {
				float m_e00, m_e01;
				float m_e10, m_e11;
			};

			float m_mat[2][2];
			float m_row[4];
		};

		Mat2x2() : m_e00(0.f), m_e10(0.f), m_e01(0.f), m_e11(0.f) {}

		Mat2x2(float e1, float e2, float e3, float e4) : m_e00{ e1 }, m_e10{ e2 }, m_e01{ e3 }, m_e11{ e4 } {}

		/*Mat2x2<T> identity(T val) const{
			return Mat2x2<T>(val, 0, 0, val);
		}*/

		void m_Mat2Swap(Mat2x2& lhs, Mat2x2& rhs);

		Mat2x2 m_Mat2Transpose() const;

		Mat2x2 m_Mat2Abs() const;

		Mat2x2 m_Mat2Adjunct() const;

		float m_Mat2Determinant() const;

		Mat2x2 m_Mat2Inverse() const;


		Mat2x2& operator=(const Mat2x2& rhs);

		Mat2x2& operator*=(const Mat2x2& rhs);

		Mat2x2& operator +=(const Mat2x2& rhs);

		Mat2x2& operator -=(const Mat2x2& rhs);
	};

	Mat2x2 operator*(const Mat2x2& lhs, const Mat2x2& rhs);

	Mat2x2 operator+(const Mat2x2& lhs, const Mat2x2& rhs);

	Mat2x2 operator-(const Mat2x2& lhs, const Mat2x2& rhs);
}


#endif

