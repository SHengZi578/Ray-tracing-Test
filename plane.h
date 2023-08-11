#pragma once
#include "Hittable.h"

class plane : public Hittable
{
public:
	vec3 orgin;
	vec3 normal;

	plane(const vec3 orgin = vec3::zero, const vec3 normal = vec3::up, shared_ptr<material> mat = nullptr)
		: orgin(orgin), normal(normal) 
	{
		mat_ptr = mat;
	}
	bool hit(const ray& r, hit_record& rec) const override
	{
		auto cosa = r.direction.dot(normal);

		if (fabs(cosa) < 1e-6)
			return false;
		auto root = (orgin - r.origin).dot(normal) / cosa;
		if (root < r.t_min || root > r.t_max)
			return false;
		rec.t = root;
		rec.pos = r.at(rec.t);
		rec.normal = cosa < 0 ? normal : -normal;
		rec.mat_ptr = mat_ptr;
		return true;
	}
};