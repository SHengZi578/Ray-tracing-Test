#pragma once
#include "Hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class HittableList : public Hittable
{
public:
	std::vector<shared_ptr<Hittable>> objects;

public:
	HittableList() {}
	HittableList(shared_ptr<Hittable> object) { add(object); }

	void clear() { objects.clear(); }
	void add(shared_ptr<Hittable> object) { objects.push_back(object); }

	virtual bool hit(const ray& r, hit_record& rec) const
	{
		hit_record temp_rec;
		bool hit_anything = false;

		ray ray = r;

		for (const auto& object : objects)
		{
			if (object->hit(ray, temp_rec))
			{
				hit_anything = true;
				ray.t_max = temp_rec.t;
				rec = temp_rec;
			}
		}

		return hit_anything;
	}
	
};

