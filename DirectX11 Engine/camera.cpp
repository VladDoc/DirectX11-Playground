#include "camera.h"

#include "imgui/imgui.h"

#include "constants.h"

mat4 Camera::getInvVP() const 
{
	mat4 view = QuatRotMat(look) * TransFromVec(pos);
	mat4 projection = PerspectiveFov(fov, aspect, Near, Far);

	return Inverse(nullptr, projection) * view;
}

vec4 Camera::getForward() const 
{
	return Vec4Trans(Vec4(0.0f, 0.0f, 1.0f, 0.0f), QuatRotMat(look));
}

vec4 Camera::getUp() const 
{
	return Vec4Trans(Vec4(0.0f, 1.0f, 0.0f, 0.0f), QuatRotMat(look));
}

vec4 Camera::getRight() const 
{
	return Vec4Trans(Vec4(1.0f, 0.0f, 0.0f, 0.0f), QuatRotMat(look));
}

void Camera::moveForward(float by) 
{
	this->pos = VecAdd(pos, VecScale(this->getForward(), by));
}

void Camera::moveRight(float by) 
{
	this->pos = VecAdd(pos, VecScale(this->getRight(), by));
}

void Camera::moveUp(float by) 
{
	this->pos = VecAdd(pos, VecScale(this->getUp(), by));
}

void Camera::rotatePitch(float angle) 
{
	look = QuatMul(QuatRotAxis(Vec4(1.0f, 0.0f, 0.0f, 1.0f), -angle), look);
}

void Camera::rotateYaw(float angle) 
{
	look = QuatMul(QuatRotAxis(Vec4(0.0f, 1.0f, 0.0f, 1.0f), angle), look);
}

void Camera::rotateRoll(float angle) 
{
	look = QuatMul(QuatRotAxis(Vec4(0.0f, 0.0f, 1.0f, 1.0f), angle), look);
}

void Camera::rotateAlongX(float angle)
{
}

void Camera::rotateAlongY(float angle)
{

}

void Camera::rotateAlongZ(float angle)
{

}

void Camera::setAspect(size_t w, size_t h) 
{
	aspect = (float)(w) / h;
}

void Camera::renderImGui()
{
	ImGui::Text("Camera");

	ImGui::SliderFloat("Near", &this->Near, 0.001f, 1.0f);
	ImGui::SliderFloat("Far", &this->Far, 2.0f, 10000.0f);

	ImGui::Separator();

	ImGui::SliderFloat("Aspect", &this->aspect, 0.001f, 4.0f);
	ImGui::SliderFloat("Fov", &this->fov, 0.0001f, Constants::pi - 0.0001f);

	ImGui::SliderFloat4("Look Quat", &this->look.m128_f32[0], -1, 1);

	ImGui::SliderFloat4("Position", &this->pos.m128_f32[0], -10000, 10000);
}