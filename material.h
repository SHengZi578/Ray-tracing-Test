#pragma once
#ifndef MATERIAL_H
#define MATERIAL_H

#include "common.h"
#include "color.h"
#include "ray.h"
#include "Hittable.h"

class material
{
public:
    virtual bool scatter(
        const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered
    ) const = 0;
    virtual color emitted(const hit_record& rec) const
    {
        return color(0, 0, 0);
    } 
};

#endif 