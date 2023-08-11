#pragma once
#include "Hittable.h"

class circle : public Hittable
{
public:
	vec3 center;
	vec3 normal;
	double radius;
	circle(const vec3 center = vec3::zero, double radius = 1, const vec3 normal = vec3::up, shared_ptr<material> mat = nullptr)
		: center(center), radius(radius), normal(normal)
	{
		mat_ptr = mat;
	}
	bool hit(const ray& r, hit_record& rec) const override
	{
		auto cosa = r.direction.dot(normal);

		if (fabs(cosa) < 1e-6)
			return false;
		auto root = (center - r.origin).dot(normal) / cosa;
		if (root < r.t_min || root > r.t_max)
			return false;
		auto pos = r.at(root);
		if ((pos - center).magnitute2() > square(radius))
			return false;
		rec.t = root;
		rec.pos = pos;
		rec.normal = cosa < 0 ? normal : -normal;
		rec.mat_ptr = mat_ptr;
		return true;
	}
};