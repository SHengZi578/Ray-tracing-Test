#pragma once
#include "vec3.h"
#include <iostream>
#include <string>
#include "common.h"
using color = vec3;

namespace Color {
#define GLOBAL_COLOR static const color
	GLOBAL_COLOR red = color(1, 0, 0);
	GLOBAL_COLOR green = color(0, 1, 0);
	GLOBAL_COLOR blue = color(0, 0, 1);
	GLOBAL_COLOR black = color(0, 0, 0);
	GLOBAL_COLOR white = color(1, 1, 1);


}