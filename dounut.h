#pragma once
#include "Hittable.h"
#include "vec3.h"
class dounut : public Hittable
{
public:
	point3 center;
	double radius1, radius2;
	double Rx, Ry;
	dounut(const point3& center, double radius1, double radius2, double Rx, double Ry, shared_ptr<material> mat)
		: center(center), radius1(radius1), radius2(radius2), Rx(Rx), Ry(Ry)
	{
		this->mat_ptr = mat;
	}
	dounut(const point3& center, double radius1, double radius2, const vec3& forward, shared_ptr<material> mat)
		: center(center), radius1(radius1), radius2(radius2)
	{
		this->mat_ptr = mat;
		auto rotation = vec3::unitVector2Rotation(forward);
		this->Rx = rotation.x;
		this->Ry = rotation.y;
	}
	virtual bool hit(
		const ray& r, hit_record& rec) const override
	{
		ray ray = r;
		ray.origin = ray.origin - center;


		auto& s = ray.origin;
		auto& v = ray.direction;

		if (fabs(v.z) < 1e-8)
			return false;

		auto sumRadius2 = square(radius1 + radius2);

		auto half_b = s.dot(v);
		auto hit_c = s.magnitute2() - sumRadius2;

		auto discriminant = square(half_b) - hit_c;
		if (discriminant < 0)
			return false;
		
		

		auto imagine1 = (radius2 - s.z) / v.z;
		if (square(imagine1 * v.x + s.x) + square(imagine1 * v.y + s.y) > sumRadius2)
		{
			imagine1 = (-radius2 - s.z) / v.z;
			if (square(imagine1 * v.x + s.x) + square(imagine1 * v.y + s.y) > sumRadius2)
				return false;
		}

		double sqrtd = sqrt(discriminant);

		auto imagine2 = (-half_b - sqrtd);
		auto imagine = std::max(imagine1, imagine2);

		/*rec.t = root;
		rec.pos = r.at(rec.t);
		vec3 normal = (rec.pos - center) / (radius1 + radius2);
		rec.set_face_normal(r, normal);
		rec.mat_ptr = mat_ptr;*/
		//return true;

		auto sheng = square(radius1) - square(radius2);
		auto a = 4 * s.dot(v);

		auto b =
			2 * (s.magnitute2() + sheng)
			- 4 * square(radius1) * (1.0 - square(v.z))
			+ 4 * square(s.dot(v));

		auto c =
			4 * s.dot(v) * (s.magnitute2() + sheng)
			- 8 * square(radius1) * (v.x * s.x + v.y * s.y);

		auto d =
			square(s.magnitute2())
			+ 2 * sheng * s.magnitute2()
			- 4 * square(radius1) * (square(s.x) + square(s.y))
			+ square(sheng);

		auto root = findRoot4(a, b, c, d, 0, 100);
		//std::cerr << "a: " << a << "\tb: " << b << "\tc: " << c << "\td: " << d << "\troot: " << root << '\n';
		if (isnan(root))
		{
			return false;
		}
		if (root < r.t_min || root > r.t_max)
			return false;

		rec.t = root;
		rec.pos = r.at(rec.t);
		auto rawPos = ray.at(rec.t);
		auto newVec = vec3(rawPos.x, rawPos.y, 0.0);
		newVec = newVec.normalize() * radius1;
		//newVec *= sqrt(square(radius1) / newVec.magnitute2());
		vec3 normal = (rawPos - newVec) / radius2;
		//vec3::rotate(normal, Rx, Ry);
		//vec3 normal = (rec.pos - center) / radius;
		rec.set_face_normal(r, normal);
		rec.mat_ptr = mat_ptr;
		return true;
	}
};
