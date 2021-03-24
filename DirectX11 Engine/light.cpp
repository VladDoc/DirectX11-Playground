#include "light.h"

#include "imgui/imgui.h"

void Light::renderImGui()
{
	ImGui::Text("Light");

	ImGui::SliderFloat4("Direction", &this->dir.m128_f32[0], -1, 1);
	dir = Vec3Norm(dir);
	ImGui::ColorEdit4("Color", &this->color.m128_f32[0]);

	ImGui::SliderFloat4("Pos", &this->pos.m128_f32[0], -10000, 10000);
	ImGui::SliderFloat("Intensity", &this->intensity, 0, 1e8);
	ImGui::SliderFloat("Cone", &this->cone, -1.0f, 1.0f);
}