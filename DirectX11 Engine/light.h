#ifndef LIGHT_H
#define LIGHT_H

#include <optional>
#include <cmath>

#include "NonUglyMath.h"

#include "constants.h"

struct Light
{
	vec4 dir;

	vec4 pos = {};
	float intensity = 40.0f;
	float cone = std::cos(2 * Constants::pi);
	vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};

	void renderImGui();
};

#endif //!LIGHT_H