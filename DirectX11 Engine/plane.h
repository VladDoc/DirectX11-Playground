#ifndef PLANE_H
#define PLANE_H

#include "NonUglyMath.h"

#include "material.h"

struct Plane
{
	vec4 norm;
	vec4 p0;

	//MipmapTex texture;

	Material material;

	Plane() = default;
	Plane(const vec4& n, const vec4& p, const Material& mat) : norm(n), p0(p), material(mat) {}

	/*
	inline Uint32 toPixel(const HitData& h) const
	{
		using namespace glm;
		vec3 e1 = normalize(cross(this->norm, vec3(1, 0, 0)));

		//If normal and (1,0,0) are parallel, change e1
		if (e1 == vec3(0, 0, 0)) {
			e1 = normalize(cross(this->norm, vec3(0, 0, 1)));
		}

		vec3 e2 = normalize(cross(this->norm, e1));

		vec3 scaled = util::leaveFraction(h.pos);

		float u = (dot(e1, scaled) + 1.0f) / 2.0f;
		float v = (dot(e2, scaled) + 1.0f) / 2.0f;

		return uvToPixelFiltered(chooseMip(texture, h.dist), u, v);
	}
	*/
	void renderImGui();
};

#endif // !PLANE_H
