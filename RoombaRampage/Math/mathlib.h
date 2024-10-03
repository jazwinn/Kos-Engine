/******************************************************************/
/*!
\file      mathlib.h
\author    Clarence Boey
\par       c.boey@digipen.edu
\date      Sept 28, 2024
\brief     Math functions with reference to CMath

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef MATHLIB_H
#define MATHLIB_H
#include <math.h>
#include <cmath>

#define PI 3.141592653589793238462643383279502884197
namespace mathlibrary {
	class mathlib {
		//all is done in degree
		//returns in radian
		//x is in radian
		double funcRadianToDegree(double x);
		float funcRadianToDegree(float x);

		double funcDegreeToRadian(double x);
		float funcDegreeToRadian(float x);

		double funcCos(double x);
		float  funcCos(float x);
		long double funcCos(long double x);

		double funcSin(double x);
		float  funcSin(float x);
		long double funcSin(long double x);

		double funcTan(double x);
		float  funcTan(float x);
		long double funcTan(long double x);

		double funcArcCos(double x);
		float funcArcCos(float x);
		long double funcArcCos(long double x);

		double funcArcSin(double x);
		float funcArcSin(float x);
		long double funcArcSin(long double x);

		double funcArcTan(double x);
		float funcArcTan(float x);
		long double funcArcTan(long double x);

		double funcArcTan2(double y, double x);
		float funcArcTan2(float y, float x);
		long double funcArcTan2(long double y, long double x);


		//power functions
		double funcPow(double base, double exponent);
		float funcPow(float base, float exponent);
		long double funcPow(long double base, long double exponent);

		double funcSqrt(double base);
		float funcSqrt(float base);
		long double funcSqrt(long double base);

		//calculating the hypotenuse of a right angle triangle
		double funcHypot(double x, double y);
		float funcHypot(float x, float y);
		long double funcHypot(long double x, long double y);

		//max and min
		double funcFmax(double x, double y);
		float funcFmax(float x, float y);
		long double funcFmax(long double x, long double y);

		double funcFmin(double x, double y);
		float funcFmin(float x, float y);
		long double funcFmin(long double x, long double y);

		double funcFabs(double x);
		float funcFabs(float x);
		long double funcFabs(long double x);

		double funcAbs(double x);
		float funcAbs(float x);
		long double funcAbs(long double x);

		//boolean
		bool funcIsGreater(double x, double y);
		bool funcIsGreater(float x, float y);
		bool funcIsGreater(long double x, long double y);

		bool funcIsGreaterEqual(double x, double y);
		bool funcIsGreaterEqual(float x, float y);
		bool funcIsGreaterEqual(long double x, long double y);

		bool funcIsLess(double x, double y);
		bool funcIsLess(float x, float y);
		bool funcIsLess(long double x, long double y);

		bool funcIsLessEqual(double x, double y);
		bool funcIsLessEqual(float x, float y);
		bool funcIsLessEqual(long double x, long double y);

	};
}
#endif MATHLIB_H