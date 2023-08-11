#pragma once
#include "vec3.h"
using point3 = vec3;
#define RAY_T_MIN 0.0001
#define RAY_T_MAX 100.0
class ray
{
public:
	point3 origin;
	vec3 direction;
	double t_min, t_max;

public:
	ray() :origin(vec3::zero), direction(vec3::forward), t_min(RAY_T_MIN), t_max(RAY_T_MAX) {}
	ray(const point3& origin, const vec3& direction, double t_min = RAY_T_MIN, double t_max = RAY_T_MAX)
		: origin(origin), direction(direction), t_min(t_min), t_max(t_max) {	}

	//point3 origin() const { return origin; }
	//vec3 direction() const { return direction; }

	point3 at(double t) const
	{
		return origin + t * direction;
	}

	static bool parallel(const ray& ray1, const ray& ray2, double allowable = 1e-8)
	{
		return vec3::parallel(ray1.direction, ray2.direction, allowable);
	}
	static void rotate(ray& ray, double Rx, double Ry)
	{
		vec3::rotate(ray.origin, Rx, Ry);
		vec3::rotate(ray.direction, Rx, Ry);
	}
	bool intersect(const ray& ray)
	{
		auto normal = this->direction.cross(ray.direction);
		return fabs((origin - ray.origin).dot(normal)) < 1e-6;
	}
	double getDistance(const vec3& pos)
	{
		return	(this->at(direction.dot(pos - origin)) - pos).magnitute();
	}
	double getDistance(const vec3& pos, double& t)
	{
		t = direction.dot(pos - origin);
		return	(this->at(t) - pos).magnitute();
	}
	double getDistance(const ray& ray, double& t, double& s)
	{
		auto& A = this->origin;
		auto& B = this->direction;
		auto& C = ray.origin;
		if (parallel(*this, ray))
		{
			return getDistance(C);
		}
		auto& D = ray.direction;
		auto DB = D.dot(B);
		auto CB = C.dot(B);
		auto AD = A.dot(D);
		auto CD = C.dot(D);
		auto AB = A.dot(B);
		s = (AB * DB + CD - AD - CB * DB) / (DB * DB - 1);
		return	getDistance(ray.at(s), t);
	}
	double getDistance(const ray& ray, double& t)
	{
		double s;
		return getDistance(ray, t, s);
	}
	double getDistance(const ray& ray)
	{
		double t, s;
		return getDistance(ray, t, s);
	}
	bool operator==(const ray& other) const
	{
		return origin == other.origin && direction == other.direction;
	}
};
