#pragma once
#include "material.h"
class dielectric : public material
{
public:
    color albedo;
    double ir; // Index of Refraction
    double fuzz;

public:
    dielectric(const color& a, double index_of_refraction, double f) : albedo(a), ir(index_of_refraction), fuzz(f) {}

    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const override
    {
        attenuation = albedo * exp(-0.25 * rec.t);
        double refraction_ratio = rec.front_face ? (1.0 / ir) : ir;

        

        double cos_theta = fmin(-r_in.direction.dot( rec.normal), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
        vec3 direction;
        if (refraction_ratio * sin_theta > 1.0 || reflectance(cos_theta, refraction_ratio) > random_double())
        {
            // Must Reflect
            direction = reflect(r_in.direction, rec.normal) + fuzz * vec3::random_unit_vector();
        }
        else
        {
            // Can Refract
            direction = refract(r_in.direction, rec.normal, refraction_ratio) + fuzz * vec3::random_unit_vector();
        }

        scattered = ray(rec.pos, direction.normalize());
        return true;
    }

private:
    static double reflectance(double cosine, double ref_idx)
    {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
};