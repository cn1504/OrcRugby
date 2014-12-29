#pragma once
#include "Core.h"
#include "Renderable.h"

namespace Core
{

	class PointCloud
		: public Renderable
	{
	protected:
		GLsizei IndexSize;
		GLuint VertexBuffer;
		GLuint UvBuffer;
		GLuint NormalBuffer;
		
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> normals;

		void Subdivide(const glm::vec3& u, const glm::vec3& v, const glm::vec3& w, const glm::vec2& uv_u, const glm::vec2& uv_v, const glm::vec2& uv_w, const glm::vec3& norm_u, const glm::vec3& norm_v, const glm::vec3& norm_w);

	public:
		PointCloud();
		PointCloud(const std::string& filename);
		virtual ~PointCloud();
		virtual void Load();
		virtual void Update();

		virtual void EnableBuffers(Shader* shader);
		virtual void Render(Shader* shader);
		virtual void DisableBuffers(Shader* shader);

		void BuildTestObject();
		void BuildFromMesh(const std::vector<unsigned short>& indices, const std::vector<glm::vec3>& indexed_vertices, const std::vector<glm::vec2>& indexed_uvs, const std::vector<glm::vec3>& indexed_normals);
		void SaveToFile(const std::string& filename);
	};

}