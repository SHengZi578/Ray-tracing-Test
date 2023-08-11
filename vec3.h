#pragma once
#include <cmath>
#include "common.h"

struct vec3
{
#define GLOBAL_VEC3 static const vec3
	double x, y, z;
	GLOBAL_VEC3 zero;
	GLOBAL_VEC3 up;
	GLOBAL_VEC3 down;
	GLOBAL_VEC3 forward;
	GLOBAL_VEC3 back;
	GLOBAL_VEC3 right;
	GLOBAL_VEC3 left;
	vec3()
	{
		x = y = z = 0;
	}

	vec3(double x, double y, double z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	inline vec3 operator + (const vec3& v) const
	{
		return vec3
		(
			x + v.x,
			y + v.y,
			z + v.z
		);
	}

	inline vec3 operator - (const vec3& v) const
	{
		return vec3
		(
			x - v.x,
			y - v.y,
			z - v.z
		);
	}

	inline vec3 operator * (double k) const
	{
		return vec3
		(
			k * x,
			k * y,
			k * z
		);
	}
	inline vec3 operator * (const vec3& v) const
	{
		return vec3
		(
			v.x * x,
			v.y * y,
			v.z * z
		);
	}
	inline vec3 operator / (double k) const
	{
		return vec3
		(
			x / k,
			y / k,
			z / k
		);
	}

	inline vec3& operator += (const vec3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	inline vec3& operator -= (const vec3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	inline vec3& operator *= (double k)
	{
		x *= k;
		y *= k;
		z *= k;
		return *this;
	}

	inline vec3& operator /= (double k)
	{
		x /= k;
		y /= k;
		z /= k;
		return *this;
	}

	inline vec3 operator - ()
	{
		return vec3(-x, -y, -z);
	}

	inline double magnitute2() const
	{
		return x * x + y * y + z * z;
	}

	inline double magnitute() const
	{
		return sqrt(magnitute2());
	}

	inline vec3 normalize()
	{
		double mag = magnitute();
		(*this) /= mag;
		return *this;
	}

	//doesn't change vector, just returns normalized version, hence the name
	inline vec3 normalized() const
	{
		double mag = magnitute();
		return *this / mag;
	}

	inline bool operator==(const vec3& other) const
	{
		return x == other.x && y == other.y && z == other.z;
	}
	bool near_zero(double allowable = 1e-8) const
	{
		// Return true if the vector is close to zero in all dimensions.
		return (fabs(x) < allowable) && (fabs(y) < allowable) && (fabs(z) < allowable);
	}
	inline vec3 cross(const vec3& v) const
	{
		return vec3
		(
			y * v.z - z * v.y,
			z * v.x - x * v.z,
			x * v.y - y * v.x
		);
	}
	inline double dot(const vec3& v) const
	{
		return z * v.z + y * v.y + x * v.x;
	}
	inline static vec3 random()
	{
		return vec3(random_double(), random_double(), random_double());
	}

	inline static vec3 random(double min, double max)
	{
		return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}
	inline static vec3 random_unit_vector()
	{
		auto beta = random_double(0, 2 * PI);
		auto alfa = random_double(0, 2 * PI);
		auto cb = cos(beta);
		return vec3(cb * cos(alfa), cb * sin(alfa), sin(beta));
	}

	inline static vec3 random_in_unit_sphere()
	{
		return random_unit_vector() * random_double();
	}

	static bool parallel(const vec3& v1, const vec3& v2, double allowable = 1e-8)
	{
		return (v1.cross(v2)).near_zero(allowable);
	}
	static void rotate(vec3& v, double Rx, double Ry)
	{
		auto x = v.x;
		auto y = v.y;
		auto z = v.z;
		v = vec3(x * cos(Rx), y * cos(Rx) - z * sin(Rx), x * sin(Ry) + y * sin(Rx) * cos(Ry) + z * cos(Rx) * cos(Ry));
	}
	//return (Rx, Ry, 0)
	static vec3 unitVector2Rotation(vec3 unitV)
	{
		vec3 vRight;
		if (vec3::parallel(unitV, vec3::up))
		{
			vRight = unitV.cross(vec3::forward);
		}
		else
		{
			vRight = unitV.cross(vec3::up);
		}
		auto vUp = unitV.cross(vRight);
		return vec3(asin(vUp.y) * sign(unitV.y), acos(vRight.x) * sign(vRight.z), 0);
	}
	static vec3 rotation2UnitVector(double Rx, double Ry)
	{
		auto cosx = cos(Rx);
		return vec3(cosx * sin(Ry), sin(Rx), -cosx * cos(Ry));
	}
};

inline vec3 operator * (double k, const vec3& v)
{
	return v * k;
}
inline vec3 reflect(const vec3& v, const vec3& n)
{
	return v - 2 * v.dot(n) * n;
}
inline void toSymmetry(vec3& v, const vec3& n)
{
	v = 2 * v.dot(n) * n - v;
}
inline vec3 operator - (const vec3& v)
{
	return vec3(-v.x, -v.y, -v.z);
}
inline vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat)
{
	auto cos_theta = fmin(-n.dot(uv), 1.0);
	vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
	vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.magnitute2())) * n;
	return r_out_perp + r_out_parallel;
}

_EXPORT_STD template <class _Traits>
std::basic_ostream<char, _Traits>& operator << (std::basic_ostream<char, _Traits>& _Ostr, const vec3& v)
{
	std::string str = "vec3(" + std::to_string(v.x) + ", " + std::to_string(v.y) + ", " + std::to_string(v.z) + ')';
	_Ostr << str;
	return _Ostr;
}

const vec3 vec3::zero = vec3(0, 0, 0);
const vec3 vec3::up = vec3(0, 1, 0);
const vec3 vec3::down = vec3(0, -1, 0);
const vec3 vec3::forward = vec3(0, 0, -1);
const vec3 vec3::back = vec3(0, 0, 1);
const vec3 vec3::right = vec3(1, 0, 0);
const vec3 vec3::left = vec3(-1, 0, 0);
