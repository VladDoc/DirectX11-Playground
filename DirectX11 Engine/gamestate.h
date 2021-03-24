#ifndef GAMESTATE
#define GAMESTATE

#include "camera.h"

#include "sphere.h"
#include "plane.h"
#include "light.h"
#include "constant_buffer.h"

struct  GameState
{
	bool done = false;

	size_t screenW;
	size_t screenH;

	float time = 0.0f;

	int frametime = 20;

	Camera camera;

	Sphere sp{{0.0f, 2.0f, 2.0f, 1.0f}, 1.0f, {Vec4(1.0f, 1.0f, 1.0f, 1.0f),
											   Vec4(0.05f, 0.05f, 0.05f, 1.0f),
											   Vec4(1.0f, 1.0f, 1.0f, 1.0f), 200.0f}};

	Plane plane{{0.0f, -1.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f},
				{Vec4(0.5f, 0.5f, 0.5f, 0.1f),
				 Vec4(0.0f, 0.0f, 0.0f, 1.0f),
				 Vec4(1.0f, 1.0f, 1.0f, 1.0f), 10.0f}};


	static const vec4 moonCoords;

	Sphere moon{moonCoords, 300.0f, {}};

	Light moon_light{Vec3Norm(moonCoords), {moonCoords}, 100000000.0f};

	static const vec4 lpCoords;

	Light pointL{Vec3Norm(lpCoords), {lpCoords}, 5.0f};

	Sphere pointS{lpCoords, 0.1f, {Vec4(0.1f, 0.1f, 0.1f, 0.1f),
								   Vec4(1.0f, 1.0f, 1.0f, 1.0f),
								   Vec4(1.0f, 1.0f, 1.0f, 1.0f), 20.0f}};

	vec4 skyColor1 = Vec4(0, 0, 1, 1);
	vec4 skyColor2 = Vec4(0, 0, 0, 1);
	vec4 fogColor  = Vec4(0.5, 0.5, 0.5, 1);

	bool fog = false;
	bool skymode = true;

	void renderImGui();

	void updateCB(ConstantBuffer& cb) const;
};
#endif //!GAMESTATE