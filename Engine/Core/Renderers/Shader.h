#pragma once
#include <Core.h>

namespace Core
{
	namespace Renderers
	{

		class Shader
		{
		protected:
			GLuint ProgramId;

			GLuint CreateShader(GLenum eShaderType, const std::string &strShaderFile);
			GLuint CreateProgram(const std::vector<GLuint> &shaderList);

			void SetVecAttribute(std::string name, GLuint buffer, int size, int stride);
			void SetIVecAttribute(std::string name, GLuint buffer, int size, int stride);
			void SetDVecAttribute(std::string name, GLuint buffer, int size, int stride);

		public:
			Shader() = delete;
			Shader(const Shader&) = delete;
			Shader& operator=(const Shader&) = delete;

			Shader(std::string vertex, std::string fragment);
			Shader(std::string vertex, std::string geometry, std::string fragment);
			Shader(std::string vertex, std::string tess_control, std::string tess_eval, std::string fragment);
			Shader(std::string vertex, std::string tess_control, std::string tess_eval, std::string geometry, std::string fragment);
			virtual ~Shader();

			void Activate();
			
			void SetTexture(std::string name, const Core::Assets::Texture& texture, int value);

			void SetUniform(std::string name, int value);
			void SetUniform(std::string name, float value);
			void SetUniform(std::string name, const glm::vec2& value);
			void SetUniform(std::string name, const glm::vec3& value);
			void SetUniform(std::string name, const glm::vec4& value);
			void SetUniform(std::string name, const glm::dvec2& value);
			void SetUniform(std::string name, const glm::dvec3& value);
			void SetUniform(std::string name, const glm::dvec4& value);
			void SetUniform(std::string name, const glm::ivec2& value);
			void SetUniform(std::string name, const glm::ivec3& value);
			void SetUniform(std::string name, const glm::ivec4& value);
			void SetUniform(std::string name, const glm::mat3& value);
			void SetUniform(std::string name, const glm::mat4& value);

			void SetVec2Attribute(std::string name, GLuint buffer);
			void SetVec3Attribute(std::string name, GLuint buffer);
			void SetVec4Attribute(std::string name, GLuint buffer);
			void SetIVec2Attribute(std::string name, GLuint buffer);
			void SetIVec3Attribute(std::string name, GLuint buffer);
			void SetIVec4Attribute(std::string name, GLuint buffer);
			void SetDVec2Attribute(std::string name, GLuint buffer);
			void SetDVec3Attribute(std::string name, GLuint buffer);
			void SetDVec4Attribute(std::string name, GLuint buffer);

			void SetElementBuffer(GLuint buffer);

			void DisableAttribute(std::string name);
		};

	}
}