#pragma once
#include <cmath>
#include <limits>
#include <memory>

// Usings

using std::shared_ptr;
using std::make_shared;
using std::sqrt;

// Constants

inline const double infinity = std::numeric_limits<double>::infinity();
inline const double NaN = std::numeric_limits<double>::quiet_NaN();
inline const double PI = 3.1415926535897932385;

// Utility Functions
inline double square(double num)
{
	return num * num;
}
inline int sign(const double num)
{
	return num >= 0.0 ? 1 : 0;
}
inline double degrees2radians(double degrees)
{
	return degrees * PI / 180.0;
}
// Returns a random real in [0,1).
inline double random_double()
{
	return rand() / (RAND_MAX + 1.0);
}
double findRoot4(double a, double b, double c, double d, double imagine, const size_t allCompute = 250)
{
	double root = imagine;
	double newRoot;
	double root2 = root * root;
	double root3 = root2 * root;
	double root4 = root3 * root;
	for (size_t i = 0; i < allCompute; i++)
	{
		newRoot = root - (root4 + a * root3 + b * root2 + c * root + d) / (4 * root3 + 3 * a * root2 + 2 * b * root + c);
		if (fabs(newRoot - root) < 1e-6)
		{
			return newRoot;
		}
		root = newRoot;
		root2 = root * root;
		root3 = root2 * root;
		root4 = root3 * root;
	}
	return NaN;
}
inline double random_double(double min, double max)
{
	// Returns a random real in [min,max).
	return min + (max - min) * random_double();
}
inline double clamp(double x, double min, double max) {
	return x > min ? x < max ? x : max : min;
}
// Common Headers
