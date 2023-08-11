#pragma once
#include "material.h"
#include "vec3.h"

class metal_phong :
	public material
{
public:
	color albedo;
	double fuzz;
public:
	metal_phong(const color& a, const double fuzz = 0.5) : albedo(a), fuzz(fuzz) {}

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
	) const override
	{
		vec3 originRlected = reflect(r_in.direction, rec.normal);
		vec3 reflected = (originRlected + fuzz * vec3::random_unit_vector()).normalize();
		if (reflected.dot(rec.normal) < 0)
		{
			toSymmetry(reflected, originRlected);
		}
		scattered = ray(rec.pos, reflected);
		attenuation = albedo * exp(-0.25 * rec.t);
		return reflected.dot(rec.normal) > 0;
	}
};
