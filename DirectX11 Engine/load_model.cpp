#include "load_model.h"

#include "tinyobjloader/tiny_obj_loader.h"

std::vector<Vertex> LoadModel(const char* path)
{
	tinyobj::attrib_t attribute;

	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warning;
	std::string error;

	if (!tinyobj::LoadObj(&attribute, &shapes, &materials, &warning, &error, path)) {
		error += warning;
		MessageBoxA(NULL, error.c_str(), "Tough luck", MB_ICONERROR | MB_OK);
	}

	// Calculate necessary size ahead to reallocate less
	size_t vecsize = 0;
	for (const auto& s : shapes) vecsize += s.mesh.indices.size();

	std::vector<Vertex> vertices;
	vertices.reserve(vecsize);

	for (const auto& s : shapes) {
		for (const auto& index : s.mesh.indices) {
			Vertex vertex;

			vertex.position = {
				attribute.vertices[3 * index.vertex_index + 0],
				attribute.vertices[3 * index.vertex_index + 1],
				attribute.vertices[3 * index.vertex_index + 2],
				1.0f
			};

			vertex.uv = {
				attribute.texcoords[2 * index.texcoord_index + 0],
				attribute.texcoords[2 * index.texcoord_index + 1]
			};

			vertex.normal = {
				attribute.normals[3 * index.normal_index + 0],
				attribute.normals[3 * index.normal_index + 1],
				attribute.normals[3 * index.normal_index + 2]
			};

			vertices.push_back(vertex);
		}
	}

	vertices.shrink_to_fit();

	return vertices;
}