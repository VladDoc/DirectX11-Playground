#pragma once

#include "NonUglyMath.h"

struct Camera {
	mat4 getInvVP() const;

	// These rotate along SUBJECTIVE pitch and yaw, etc.
	void rotatePitch(float angle);
	void rotateYaw(float angle);
	void rotateRoll(float angle);

	// While these do along real axis 
	void rotateAlongX(float angle);
	void rotateAlongY(float angle);
	void rotateAlongZ(float angle);
	
	vec4 pos = Vec4(0.0f, 1.0f, 0.0f, 1.0f);
	void moveForward(float l);
	void moveRight(float l);
	void moveUp(float l);

	vec4 look = Vec4(0.0f, 0.0f, 0.0f, 1.0f);
	vec4 getForward() const;
	vec4 getUp() const;
	vec4 getRight() const;

	float fov{};
	float aspect{};
	float Near = 1.0f;
	float Far = 2.0f;
	void setAspect(size_t w, size_t h);

	void renderImGui();
};