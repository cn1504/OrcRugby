#pragma once
#include <Core.h>
#include <Renderers/Renderable.h>
#include "PointCloud.h"

namespace Core
{
	namespace Assets
	{

		class Mesh
			: public Core::Renderers::Renderable
		{
		protected:
			GLsizei IndexSize;
			GLuint VertexBuffer;
			GLuint UvBuffer;
			GLuint NormalBuffer;
			GLuint ElementBuffer;

			bool hasBones;
			GLuint BoneIndexBuffer;
			GLuint BoneWeightBuffer;

			std::vector<unsigned short> indices;
			std::vector<glm::vec3> indexed_vertices;
			std::vector<glm::vec2> indexed_uvs;
			std::vector<glm::vec3> indexed_normals;
			std::vector<glm::ivec4> indexed_boneIndices;
			std::vector<glm::vec4> indexed_boneWeights;

		public:
			Mesh(std::string filename);
			Mesh(std::vector<unsigned short> indices, std::vector<glm::vec3> indexed_vertices, std::vector<glm::vec2> indexed_uvs, std::vector<glm::vec3> indexed_normals);
			Mesh(Mesh* other);
			virtual ~Mesh();
			virtual void Load();
			virtual void Update();

			virtual void Draw(Core::Renderers::Shader* shader);

			PointCloud* ToPointCloud();

			bool HasBones();

			void UpdateGeometry(std::vector<unsigned short> indices, std::vector<glm::vec3> indexed_vertices, std::vector<glm::vec3> indexed_normals);
			btConvexTriangleMeshShape* GenerateTriangleMeshShape();
		};

	}
}

