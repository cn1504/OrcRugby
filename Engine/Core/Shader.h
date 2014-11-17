#pragma once
#include "Core.h"

namespace Core
{

	class Shader
	{
	protected:
		GLuint ProgramId;

		GLuint CreateShader(GLenum eShaderType, const std::string &strShaderFile);
		GLuint CreateProgram(const std::vector<GLuint> &shaderList);

	public:
		Shader(std::string vertex, std::string fragment);
		Shader(std::string vertex, std::string geometry, std::string fragment);
		Shader(std::string vertex, std::string tess_control, std::string tess_eval, std::string fragment);
		virtual ~Shader();

		void MakeCurrent();
		GLuint GetUL(std::string var);
		GLuint GetID();
	};

}