#pragma once
#include <Core.h>
#include <Renderers/Renderable.h>

namespace Core
{
	namespace Assets
	{

		struct PointCloudDetailLevel
		{
			float Resolution = 0.01f;

			GLsizei IndexSize = 0;
			GLuint VertexBuffer = 0;
			GLuint UvBuffer = 0;
			GLuint NormalBuffer = 0;

			std::vector<glm::vec3> vertices;
			std::vector<glm::vec2> uvs;
			std::vector<glm::vec3> normals;
		};

		class PointCloud
			: public Core::Renderers::Renderable
		{
		protected:
			int currentLevel;
			std::vector<PointCloudDetailLevel*> geometry;

			void Subdivide(const glm::vec3& u, const glm::vec3& v, const glm::vec3& w, const glm::vec2& uv_u, const glm::vec2& uv_v, const glm::vec2& uv_w, const glm::vec3& norm_u, const glm::vec3& norm_v, const glm::vec3& norm_w);
			void BuildDetailLevel(int lvl, float resolution);

		public:
			glm::vec3 ViewPosition;

			PointCloud();
			PointCloud(const std::string& filename);
			virtual ~PointCloud();
			virtual void Load();
			virtual void Update();

			virtual void Draw(Core::Renderers::Shader* shader);

			void BuildTestObject();
			void BuildFromMesh(const std::vector<unsigned short>& indices, const std::vector<glm::vec3>& indexed_vertices, const std::vector<glm::vec2>& indexed_uvs, const std::vector<glm::vec3>& indexed_normals);
			void SaveToFile(const std::string& filename);
		};

	}
}