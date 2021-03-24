#include "sphere.h"

#include "imgui/imgui.h"

void Sphere::renderImGui()
{
	ImGui::Text("Sphere");

	ImGui::SliderFloat("Radius", &this->radius, 0.0f, 10000.0f);
	ImGui::SliderFloat4("Pos", &this->pos.m128_f32[0], -10000.0f, 10000.0f);
	ImGui::Separator();

	this->material.renderImGui();
}