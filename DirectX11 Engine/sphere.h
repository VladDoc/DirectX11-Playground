#pragma once

#include "NonUglyMath.h"

#include "material.h"

struct Sphere
{
	vec4 pos;
	float radius;

	//SDL_Surface* texture = nullptr;

	Material material;

	Sphere() = default;
	Sphere(const vec4& p, float r, const Material& m) : pos(p), radius(r), material(m) {}

	void renderImGui();
};