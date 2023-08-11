#pragma once
#include "Hittable.h"
#include "vec3.h"
#include "material.h"
class sphere : public Hittable
{
public:
	point3 center;
	double radius;

	sphere() :center(vec3(0, 0, 0)), radius(5) {}
	sphere(point3 cen, double r, shared_ptr<material> mat) : center(cen), radius(r)
	{
		mat_ptr = mat;
	}

	virtual bool hit(
		const ray& r, hit_record& rec) const override;
};

bool sphere::hit(const ray& r, hit_record& rec) const
{
	vec3 oc = r.origin - center;
	auto half_b = oc.dot(r.direction);
	auto c = oc.magnitute2() - radius * radius;
	//
	auto discriminant = half_b * half_b - c;
	if (discriminant < 0) return false;
	double sqrtd = sqrt(discriminant);

	// Find the nearest root that lies in the acceptable range.
	double root = (-half_b - sqrtd);
	if (root < r.t_min || r.t_max < root)
	{
		root = (-half_b + sqrtd);
		if (root < r.t_min || r.t_max < root)
			return false;
	}
	rec.t = root;

	rec.pos = r.at(rec.t);
	vec3 normal = (rec.pos - center) / radius;
	rec.set_face_normal(r, normal);
	rec.mat_ptr = mat_ptr;
	return true;
}