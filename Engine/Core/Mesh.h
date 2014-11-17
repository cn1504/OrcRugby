#pragma once
#include "Core.h"
#include "Renderable.h"

namespace Core
{

	class Mesh
		: public Renderable
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

		virtual void EnableBuffers(Shader* shader);
		virtual void Render(Shader* shader);
		virtual void DisableBuffers(Shader* shader);

		bool HasBones();

		void UpdateGeometry(std::vector<unsigned short> indices, std::vector<glm::vec3> indexed_vertices, std::vector<glm::vec3> indexed_normals);
		btConvexTriangleMeshShape* GenerateTriangleMeshShape();
	};

}

