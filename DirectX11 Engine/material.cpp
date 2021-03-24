#include "material.h"

#include "imgui/imgui.h"

void Material::renderImGui()
{
	ImGui::Text("Material");
	ImGui::SliderFloat("Shininess", &this->shininess, 0, 10000);
	ImGui::SliderFloat4("Ambient",  &this->ambient.m128_f32[0], 0, 1);
	ImGui::SliderFloat4("Diffuse",  &this->diffuse.m128_f32[0], 0, 1);
	ImGui::SliderFloat4("Specular", &this->specular.m128_f32[0], 0, 1);
}