#include "loadshapes.h"


vector<Triangle*> loadshapes(attrib_t attrib, vector<shape_t> objshapes, vector<material_t> materials, string texpath) {
	vector<Triangle*> shapes;
	for (auto& objshape : objshapes) {
		size_t index_offset = 0;
		for (size_t f = 0; f < objshape.mesh.num_face_vertices.size(); f++) {
			int fv = objshape.mesh.num_face_vertices[f];
			// if triangles
			if (fv == 3) {
				vec3 vertices[3];
				vec3 normals[3];
				vec2 uvs[3];
				
				for (size_t v = 0; v < fv; v++) {
					index_t idx = objshape.mesh.indices[index_offset + v];
					vertices[v] = vec3(
						attrib.vertices[3 * idx.vertex_index + 0],
						attrib.vertices[3 * idx.vertex_index + 1],
						attrib.vertices[3 * idx.vertex_index + 2]);

					if (!attrib.normals.empty()) {
						normals[v] = vec3(
							attrib.normals[3 * idx.normal_index + 0],
							attrib.normals[3 * idx.normal_index + 1],
							attrib.normals[3 * idx.normal_index + 2]
						);
					}

					if (!attrib.texcoords.empty() && idx.texcoord_index >= 0) {
						uvs[v] = vec2(
							attrib.texcoords[2 * idx.texcoord_index + 0],
							attrib.texcoords[2 * idx.texcoord_index + 1]
						);
					}
				}
				material_t material;
				string diffuse_texpath;
				if (!materials.empty() && objshape.mesh.material_ids[f] >= 0) {
					material = materials[objshape.mesh.material_ids[f]];
					diffuse_texpath = texpath + '/' + material.diffuse_texname;
				}
				Triangle* triangle = new Triangle(vertices[0], vertices[1], vertices[2], normals[0], normals[1], normals[2], uvs[0], uvs[1], uvs[2], diffuse_texpath, material);
				shapes.push_back(triangle);
				index_offset += fv;
			}
		}
	}

	return shapes;
}