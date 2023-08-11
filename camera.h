#pragma once
#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"

class camera {
private:
	//UNIT VECTORS
	double Rx = 0;
	double	Ry = 0;
	vec3 forward = vec3::forward;
	vec3 up = vec3::up;
	vec3 right = vec3::right;
	point3 lower_left_corner;
	point3 lower_left_corner_orthogonal;
	vec3 position;

	vec3 h_up, w_right;

	double h, w;		//height, width
public:
	camera(double fov, double aspectRatio, point3 position = point3::zero)
	{
		h = tan(degrees2radians(fov / 2.0));
		w = h * aspectRatio;
		h_up = h * up;
		w_right = w * right;
		this->position = position;
		lower_left_corner = (w_right + h_up - vec3::forward) * -0.5;
	}
	void setPosition(vec3 position) { this->position = position; }
	void setDirection(double Rx, double Ry)
	{
		this->Rx = Rx;
		this->Ry = Ry;
		this->forward = vec3::rotation2UnitVector(Rx, Ry);
		if (vec3::parallel(forward, vec3::up))
		{
			right = forward.cross(vec3::forward);
		}
		else
		{
			right = forward.cross(vec3::up);
		}
		up = -forward.cross(right);
		h_up = h * up;
		w_right = w * right;
		lower_left_corner = (h_up + w_right - forward) * -0.5;
	}
	void lookat(vec3 position)
	{
		this->forward = (position - this->position).normalize();
		if (vec3::parallel(forward, vec3::up))
		{
			right = forward.cross(vec3::forward);
		}
		else
		{
			right = forward.cross(vec3::up);
		}
		up = -forward.cross(right);
		this->Ry = acos(right.x) * sign(right.z);
		this->Rx = asin(up.y) * sign(forward.y);
		h_up = h * up;
		w_right = w * right;
		lower_left_corner = (h_up + w_right - forward) * -0.5;
	}
	void addDirection(double Rx, double Ry)
	{
		setDirection(this->Rx + Rx, this->Ry + Ry);
	}
	ray get_ray(double u, double v, double t_min = RAY_T_MIN, double t_max = RAY_T_MAX) const {
		vec3 direction = lower_left_corner + u * w_right + v * h_up;
		return ray(position, direction.normalize(), t_min, t_max);
	}
	ray get_ray_orthogonal(double u, double v, double scale = 5.0, double t_min = RAY_T_MIN, double t_max = RAY_T_MAX) const {
		vec3 pos;
		pos = -0.5 * (scale * w_right + scale * h_up) + this->position;
		pos += u * scale * w_right + v * scale * h_up;
		return ray(pos, forward, t_min, t_max);
	}
	vec3 getForward() { return forward; }
	vec3 getRight() { return right; }
	vec3 getUp() { return up; }
	double getAlfa() { return Rx; }
	double getBeta() { return Ry; }
};
#endif