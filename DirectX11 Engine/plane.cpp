#include "plane.h"

#include "imgui/imgui.h"

void Plane::renderImGui()
{
	ImGui::Text("Plane");

	ImGui::SliderFloat4("normal", &this->norm.m128_f32[0], -1, 1);
	norm = Vec3Norm(norm);

	ImGui::SliderFloat4("p0", &this->p0.m128_f32[0], -10000, 10000);

	this->material.renderImGui();
}