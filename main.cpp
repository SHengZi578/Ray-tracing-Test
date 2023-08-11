#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include "vec3.h"
#include "color.h"
#include "ray.h"
#include "common.h"
#include "camera.h"
#include "allMaterial.h"
#include "allHittable.h"
#include "Image.h"
#include "windows.h"
#include <conio.h>

color ray_color(const ray& r, Hittable& world, int depth = 64)
{
	if (depth <= 0)
	{
		return Color::white;
	}
	hit_record rec;
	if (world.hit(r, rec))
	{
		color attenuation;
		color emit;
		ray scattered;
		emit = rec.mat_ptr->emitted(rec);
		if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
		{
			return emit;
		}
		else
		{
			return emit + attenuation * ray_color(scattered, world, depth - 1);
		}
	}
	auto t = 0.5 * (r.direction.y + 1.0);
	return (1.0 - t) * Color::white + t  * 1 * color(0.4, 0.6, 1);
}

int main(int argc, char* argv[])
{
	srand(static_cast <unsigned> (time(0)));

	POINT pt = { 0, 0 };
	// Image Settings
	Image ima("test8", Image::format::BMP, 1920, 1080);
	const double aspect_ratio = static_cast<double>(ima.width) / ima.height;

	// World

	HittableList world;
	auto material_ground = make_shared<lambertian>(color(0.6, 0.6, 0.2));
	auto material_center = make_shared<emission>(Color::white * 10);
	auto material_left = make_shared<dielectric>(color(1, 0.5, 0.5), 1.5, 0);
	auto material_right = make_shared<metal_phong>(color(0.99, 0.99, 0.99), 0.1);
	auto material_dounut = make_shared<lambertian>(color(1,0.8,0.1));
	auto material_mirror = make_shared<metal>(Color::white);

	world.add(make_shared<plane>(point3(0.0, -3, 0), vec3::up, material_ground));
	//world.add(make_shared<circle>(point3(1, -1.1, -1.5), 0.5, vec3::forward, material_dounut));
	//world.add(make_shared<circle>(point3(1, -1.1, -1.5), 0.5, vec3::rotation2UnitVector( degrees2radians(0), degrees2radians(45)), material_left));
	world.add(make_shared<dounut>(point3(0, 0, -1.5), 0.4, 0.1, degrees2radians(90), degrees2radians(0), material_right));
	world.add(make_shared<dounut>(point3(0, 0, -1.5), 0.4, 0.1, degrees2radians(0), degrees2radians(0), material_right));
	//world.add(make_shared<sphere>(point3(0, 0, -1.5), 0.2, material_center));
	//world.add(make_shared<sphere>(point3(0, 0.3, -3), 0.8, material_dounut));
	//world.add(make_shared<sphere>(point3(0.0, -0.5, -1), 1, material_left));
	//world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), -0.49, material_left));
	//world.add(make_shared<sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
	//world.add(make_shared<sphere>(point3(1.0, 0.0, -1.0), 0.5, material_right));

	// Camera

	camera cam(90, aspect_ratio, vec3(0, 0, 0));
	//cam.setDirection(degrees2radians(1), degrees2radians(0));
	const int samples_per_pixel = 2;

	// Render
	clock_t start_time = clock();


	color* pixel_colors = new color[ima.width * ima.height];
	double deltaTime = 0;
#pragma omp parallel for omp_get_thread_num()
	for (int j = 0; j < ima.height; j++)
	{
		clock_t dual_time = clock();
		auto time = (double)(dual_time - start_time) / CLOCKS_PER_SEC;
		std::cerr << "\rScanlines remaining: " << (j + 1) * 100 / ima.height << "% "  << "still need: " << (ima.height - j) * deltaTime << "s" << std::flush;
#pragma omp parallel for omp_get_thread_num()
		for (int i = 0; i < ima.width; i++)
		{
			for (int s = 0; s < samples_per_pixel; s++)
			{
				auto u = static_cast<double>(i * samples_per_pixel + s) / (ima.width * samples_per_pixel - 1);
				for (int y = 0; y < samples_per_pixel; y++)
				{
					auto v = static_cast<double>(j * samples_per_pixel + y) / (ima.height * samples_per_pixel - 1);
					//ray r = cam.get_ray_orthogonal(u, v, 5);
					ray r = cam.get_ray(u, v);
					pixel_colors[i + j * ima.width] += ray_color(r, world);
				}
			}

			//pixel_colors[i + j * image_width] = color(double(i) / (image_width - 1), double(j) / (image_height - 1), 0.25);
		}
		deltaTime = (clock() - static_cast<double>(dual_time)) / CLOCKS_PER_SEC;
	}
	
	//std::cerr << "The run time is: " << (double)(dual_time - start_time) / CLOCKS_PER_SEC << "s" << std::flush;
	ima.write_colors(pixel_colors, ima.width * ima.height, samples_per_pixel);

	std::cerr << "\nDone.\n";
	std::cerr << "The render time is: " << (double)(clock() - start_time) / CLOCKS_PER_SEC << "s\n";
	ima.open();

	delete[] pixel_colors;
	return 0;
}