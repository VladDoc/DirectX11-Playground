#pragma once

#include "NonUglyMath.h"
#include "sphere.h"
#include "plane.h"
#include "light.h"

struct alignas(16) ConstantBuffer
{
	int screenW;
	int screenH;

	float time;

	int frametime;

	Sphere sp;

	Plane plane;

	Sphere moon;

	Light moon_light;

	Light pointL;

	Sphere pointS;

	vec4 cameraPos;

	bool skymode;
	vec4 skyColor1;
	vec4 skyColor2;
	vec4 fogColor;

	bool fog;

	mat4 invVP;
};