#pragma once
#include "ray.h"

class material; 

struct hit_record {
	point3 pos;
	vec3 normal;
	double t = 0.0;
	shared_ptr<material> mat_ptr; 
	bool front_face = true;

	inline void set_face_normal(const ray& r, const vec3& outward_normal)
	{
		front_face = r.direction.dot(outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}

};

class Hittable {
public:
	shared_ptr<material> mat_ptr;
	virtual bool hit(const ray& r, hit_record& rec) const = 0;
};
