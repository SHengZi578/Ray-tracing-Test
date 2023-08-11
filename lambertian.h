#pragma once
#include "vec3.h"
#include "material.h"

class lambertian : public material
{
public:
	color albedo;

public:
	lambertian(const color& a) : albedo(a) {}

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
	) const override
	{
		auto scatter_direction = rec.normal + vec3::random_unit_vector();
		if (scatter_direction.near_zero())
			scatter_direction = rec.normal;

		scattered = ray(rec.pos, scatter_direction.normalize());

		//ÎüÊÕÏµÊý

		attenuation = albedo * exp(-0.25 * rec.t);
		return true;
	}
};