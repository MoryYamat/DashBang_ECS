#include "CgltfImporter.hpp"

#include <iostream>

namespace Engine::Graphics::Model
{
	namespace MD = Engine::Graphics::Model;

	MD::ModelData CgltfImporter::Import(const std::string& path)
	{
		MD::ModelData model;

		cgltf_options options{};
		cgltf_data* g = nullptr;
		// meta情報をパース -> cgltf_dataツリーを組立てる(meshes/primitives/attributes/accessors/bufferViews/materials...)
		if (cgltf_parse_file(&options, path.c_str(), &g) != cgltf_result_success || !g)
		{
			std::cout << "[CgltfImporter]: Parse information creation failure\n";
			return model;
		}
		// cgltf_dataが参照しているbufferを実際に読み込んでbuffer->dataに展開する
		if (cgltf_load_buffers(&options, g, path.c_str()) != cgltf_result_success)
		{
			std::cout << "[CgltfImporter]: Failed to read data from file\n";
			cgltf_free(g);
			return model;
		}

		//std::cout << "Meshes: " << g->meshes_count << std::endl;
		//std::cout << "Nodes: " << g->nodes_count<< std::endl;
		//std::cout << "Buffers: " << g->buffers_count << std::endl;

		for (cgltf_size mi = 0; mi < g->meshes_count; ++mi)
		{
			const cgltf_mesh& mesh = g->meshes[mi];
			for (cgltf_size pi = 0; pi < mesh.primitives_count; ++pi)
			{
				const cgltf_primitive& prim = mesh.primitives[pi];
				if (prim.type != cgltf_primitive_type_triangles) continue;

				// 属性をひろう
				const cgltf_accessor* acc_pos = nullptr;
				const cgltf_accessor* acc_nrm = nullptr;
				const cgltf_accessor* acc_uv0 = nullptr;
				const cgltf_accessor* acc_tan = nullptr;

				for (cgltf_size ai = 0; ai < prim.attributes_count/*5*/; ++ai)
				{
					const cgltf_attribute& a = prim.attributes[ai];
					
					switch (a.type)
					{
					case cgltf_attribute_type_position: acc_pos = a.data; break;
					case cgltf_attribute_type_normal: acc_nrm = a.data; break;
					case cgltf_attribute_type_texcoord: if (a.index == 0) acc_uv0 = a.data; break;
					case cgltf_attribute_type_tangent: acc_tan = a.data; break;
					default: break;
					}
				}
				if (!acc_pos) continue;// 位置がなければスキップ

				MD::MeshData out{};
				const cgltf_size vcount = acc_pos->count;
				out.vertices.resize(static_cast<size_t>(vcount));

				float tmp[4] = { 0,0,0,0 };

				// 必要な値を取り出してVertexデータにコピー
				for (cgltf_size i = 0; i < vcount; ++i)
				{
					MD::VertexData v{};

					cgltf_accessor_read_float(acc_pos, i, tmp, 3);
					v.position = { tmp[0], tmp[1], tmp[2] };

					if (acc_nrm)
					{
						cgltf_accessor_read_float(acc_nrm, i, tmp, 3);
						v.normal = { tmp[0], tmp[1], tmp[2] };
					}
					else
					{
						v.normal = { 0,1,0 };
					}
					if (acc_uv0)
					{
						cgltf_accessor_read_float(acc_uv0, i, tmp, 2);
						v.texCoords = { tmp[0], tmp[1] };
					}
					else
					{
						v.texCoords = { 0,0 };
					}
					if (acc_tan && acc_nrm)
					{
						cgltf_accessor_read_float(acc_tan, i, tmp, 4);
						glm::vec3 T(tmp[0], tmp[1], tmp[2]);
						float sign = tmp[3];

						// Nに直交化(念のためGram-Schmidt)
						T = glm::normalize(T - v.normal * glm::dot(T, v.normal));
						v.tangent = T;

						// glTF規定: B = sign * normalize(cross(N, T))
						v.bitangent = glm::normalize(glm::cross(v.normal, v.tangent)) * sign;
					}
					else
					{
						v.tangent = glm::vec3(0.0f);
						v.bitangent = glm::vec3(0.0f);
					}

					out.vertices[(size_t)i] = v;

					model.min = glm::min(model.min, v.position);
					model.max = glm::max(model.max, v.position);
				}

				// index
				if (prim.indices)
				{
					out.indices.resize((size_t)prim.indices->count);
					for (cgltf_size i = 0; i < prim.indices->count; ++i)
					{
						cgltf_size v = cgltf_accessor_read_index(prim.indices, i);
						out.indices[(size_t)i] = (unsigned int)v;
					}
					out.hasIndices = true;
				}
				else
				{
					out.indices.resize((size_t)vcount);
					for (cgltf_size i = 0; i < vcount; ++i)
					{
						out.indices[(size_t)i] = (unsigned int)i;
						out.hasIndices = true;
					}
				}

				// 色だけ
				if (prim.material)
				{
					const auto& pbr = prim.material->pbr_metallic_roughness;
					out.materialData.baseColor =
					{
						pbr.base_color_factor[0], pbr.base_color_factor[1], pbr.base_color_factor[2]
					};
				}

				model.meshes.emplace_back(std::move(out));
			}
		}
		cgltf_free(g);
		return model;
		// 最初のメッシュ & 最初の TRIANGLES primitive を探す
		//const cgltf_mesh* tgt_mesh = nullptr;
		//const cgltf_primitive* tgt_prim = nullptr;

		//for (cgltf_size mi = 0; mi < g->meshes_count; ++mi)
		//{
		//	const cgltf_mesh& mesh = g->meshes[mi];
		//	for (cgltf_size pi = 0; pi < mesh.primitives_count; ++pi)
		//	{
		//		const cgltf_primitive& prim = mesh.primitives[pi];
		//		if (prim.type == cgltf_primitive_type_triangles)
		//		{
		//			tgt_mesh = &mesh;
		//			tgt_prim = &prim;
		//			break;
		//		}
		//	}
		//}

		//if (!tgt_prim)
		//{
		//	std::cerr << "No TRIANGLES primitive found." << std::endl;
		//	cgltf_free(g);
		//	return model;
		//}

		//// 属性アクセサのポインタをとる
		//const cgltf_accessor* acc_pos = nullptr;
		//const cgltf_accessor* acc_nrm = nullptr;
		//const cgltf_accessor* acc_uv0 = nullptr;

		//for (cgltf_size ai = 0; ai < tgt_prim->attributes_count; ++ai)
		//{
		//	const cgltf_attribute& attr = tgt_prim->attributes[ai];
		//	switch (attr.type)
		//	{
		//	case cgltf_attribute_type_position: acc_pos = attr.data; break;
		//	case cgltf_attribute_type_normal: acc_nrm = attr.data; break;
		//	case cgltf_attribute_type_texcoord: 
		//		if (attr.index == 0) acc_uv0 = attr.data;
		//		break;
		//	default: break;
		//	}
		//}

		//if (!acc_pos)
		//{
		//	std::cerr << "POSITION not found in first triangles primitives." << std::endl;
		//	cgltf_free(g);
		//	return model;
		//}

		//// 頂点数 / インデックス数をログ
		////const cgltf_size vcount = acc_pos->count;
		////const cgltf_size icount = tgt_prim->indices ? tgt_prim->indices->count : vcount;

		////std::cout << "[Probe] mesh=" << (tgt_mesh - g->meshes)
		////	<< " vcount=" << vcount
		////	<< " icount=" << icount
		////	<< " hasNormals=" << (acc_nrm ? "Y" : "N")
		////	<< " hasUV0=" << (acc_uv0 ? "Y" : "N")
		////	<< std::endl;

		//// 4) （任意）AABB を軽く計算してみる（POSITIONだけ読み出し）
		////glm::vec3 min(FLT_MAX), max(-FLT_MAX);
		////float tmp[4] = { 0,0,0,0 };
		////for (cgltf_size i = 0; i < vcount; ++i) {
		////	cgltf_accessor_read_float(acc_pos, i, tmp, 3);
		////	glm::vec3 p(tmp[0], tmp[1], tmp[2]);
		////	min = glm::min(min, p);
		////	max = glm::max(max, p);
		////}
		////std::cout << "[Probe] AABB min=(" << min.x << "," << min.y << "," << min.z << ") "
		////	<< "max=(" << max.x << "," << max.y << "," << max.z << ")\n";

		//// MeshData を作る
		//MD::MeshData mesh{};
		//const cgltf_size vcount = acc_pos->count;
		//mesh.vertices.resize(static_cast<size_t>(vcount));

		//float tmp[4] = { 0,0,0,0 };
		//for (cgltf_size i = 0; i < vcount; ++i)
		//{
		//	MD::VertexData v{};
		//	cgltf_accessor_read_float(acc_pos, i, tmp, 3);
		//	v.position = { tmp[0], tmp[1], tmp[2] };

		//	if (acc_nrm)
		//	{
		//		cgltf_accessor_read_float(acc_nrm, i, tmp, 3);
		//		v.normal = { tmp[0], tmp[1], tmp[2] };
		//	}
		//	else
		//	{
		//		v.normal = { 0,1,0 };
		//	}

		//	if (acc_uv0)
		//	{
		//		cgltf_accessor_read_float(acc_uv0, i, tmp, 2);
		//		v.texCoords = { tmp[0], tmp[1] };
		//	}
		//	else
		//	{
		//		v.texCoords = { 0,0 };
		//	}

		//	mesh.vertices[static_cast<size_t>(i)] = v;

		//	// AABB 更新
		//	model.min = glm::min(model.min, v.position);
		//	model.max = glm::max(model.max, v.position);
		//}

		//// インデックス
		//if (tgt_prim->indices)
		//{
		//	mesh.indices.resize(static_cast<size_t>(tgt_prim->indices->count));
		//	for (cgltf_size i = 0; i < tgt_prim->indices->count; ++i)
		//	{
		//		cgltf_size v = cgltf_accessor_read_index(tgt_prim->indices, i);
		//		mesh.indices[i] = static_cast<unsigned int>(v);
		//	}
		//	mesh.hasIndices = true;
		//}
		//else
		//{
		//	// 非インデックス→0..vcount-1
		//	mesh.indices.resize(static_cast<size_t>(vcount));
		//	for (cgltf_size i = 0; i < vcount; ++i) mesh.indices[i] = static_cast<unsigned int>(i);
		//	mesh.hasIndices = true;
		//}

		//// --- 5) マテリアル（まずは baseColorFactor のみ）
		//mesh.materialData.baseColor = [](const cgltf_material* m) {
		//	if (!m) return glm::vec3(1.0f);
		//	const auto& pbr = m->pbr_metallic_roughness;
		//	return glm::vec3(pbr.base_color_factor[0], pbr.base_color_factor[1], pbr.base_color_factor[2]);
		//	}(tgt_prim->material);

		//model.meshes.emplace_back(std::move(mesh));


	}
}