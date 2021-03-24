#ifndef MATERIAL_H
#define MATERIAL_H

#include "NonUglyMath.h"

struct Material
{
	vec4 diffuse;
	vec4 ambient;
	vec4 specular;
	float shininess;
	Material() = default;
	Material(const vec4& dif, const vec4& amb,
			 const vec4& spec, float shine) : diffuse(dif), ambient(amb), specular(spec), shininess(shine){}

	void renderImGui();
};

#endif // !MATERIAL_H
