#pragma once
#include "material.h"
#include "vec3.h"
class metal :
	public material
{
public:
	color albedo;

public:
	metal(const color& a) : albedo(a) {}

	virtual bool scatter(
		const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
	) const override
	{
		vec3 reflected = reflect(r_in.direction, rec.normal);
		scattered = ray(rec.pos, reflected);
		attenuation = albedo  ;
		return (scattered.direction.dot(rec.normal) > 0);
	}
};
