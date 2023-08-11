#pragma once
#include "material.h"
#include "vec3.h"

class emission : public material
{
public:
    color lighting;
public:
    emission(color intensity) : lighting(intensity) {}

    virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override
    {
        return false;
    }

    virtual color emitted(const hit_record& rec) const
    {
        return lighting /** exp(-0.25 * rec.t)*/;
    }
}; 