#include "Shader.h"

namespace Core
{

	Shader::Shader(std::string vertex, std::string fragment)
	{
		std::string shadersFolder = "Resources/Shaders/";

		std::vector<GLuint> shaderList;

		if (Settings::Misc::VerboseLogging)
			Debug::Log("Compiling \"" + vertex + "\"");
		shaderList.push_back(CreateShader(GL_VERTEX_SHADER, shadersFolder + vertex));
		if (Settings::Misc::VerboseLogging)
			Debug::Log("Compiling \"" + fragment + "\"");
		shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, shadersFolder + fragment));


		if (Settings::Misc::VerboseLogging)
			Debug::Log("Linking \"" + vertex + "\" + \"" + fragment + "\"");
		ProgramId = CreateProgram(shaderList);

		for (auto shader : shaderList)
			glDeleteShader(shader);
	}

	Shader::Shader(std::string vertex, std::string geometry, std::string fragment)
	{
		std::string shadersFolder = "Resources/Shaders/";

		std::vector<GLuint> shaderList;

		if (Settings::Misc::VerboseLogging)
			Debug::Log("Compiling \"" + vertex + "\"");
		shaderList.push_back(CreateShader(GL_VERTEX_SHADER, shadersFolder + vertex));
		if (Settings::Misc::VerboseLogging)
			Debug::Log("Compiling \"" + geometry + "\"");
		shaderList.push_back(CreateShader(GL_GEOMETRY_SHADER, shadersFolder + geometry));
		if (Settings::Misc::VerboseLogging)
			Debug::Log("Compiling \"" + fragment + "\"");
		shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, shadersFolder + fragment));

		if (Settings::Misc::VerboseLogging)
			Debug::Log("Linking \"" + vertex + "\" + \"" + geometry + "\" + \"" + fragment + "\"\n");
		ProgramId = CreateProgram(shaderList);

		for (auto shader : shaderList)
			glDeleteShader(shader);
	}

	Shader::Shader(std::string vertex, std::string tess_control, std::string tess_eval, std::string fragment)
	{
		std::string shadersFolder = "Resources/Shaders/";

		std::vector<GLuint> shaderList;

		shaderList.push_back(CreateShader(GL_VERTEX_SHADER, shadersFolder + vertex));
		shaderList.push_back(CreateShader(GL_TESS_CONTROL_SHADER, shadersFolder + tess_control));
		shaderList.push_back(CreateShader(GL_TESS_EVALUATION_SHADER, shadersFolder + tess_eval));
		shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, shadersFolder + fragment));

		ProgramId = CreateProgram(shaderList);

		for (auto shader : shaderList)
			glDeleteShader(shader);
	}


	Shader::~Shader()
	{
		glDeleteProgram(ProgramId);
	}


	GLuint Shader::CreateShader(GLenum eShaderType, const std::string &strShaderFile)
	{
		std::string shaderFilepath(strShaderFile);
		std::ifstream ifs(shaderFilepath);
		if (!ifs.is_open()) {
			Debug::Error(("Could not open: " + shaderFilepath).c_str());
		}
		std::string str((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
		ifs.close();

		GLuint shader = glCreateShader(eShaderType);
		const char *strFileData = str.c_str();
		glShaderSource(shader, 1, &strFileData, NULL);

		glCompileShader(shader);

		GLint status;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint infoLogLength;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

			GLchar *strInfoLog = new GLchar[infoLogLength + 1];
			glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

			std::string strShaderType = "";
			switch (eShaderType)
			{
			case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
			case GL_TESS_CONTROL_SHADER: strShaderType = "tessellation control"; break;
			case GL_TESS_EVALUATION_SHADER: strShaderType = "tessellation evaluation"; break;
			case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
			case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
			}

			std::string msg = "Compile failure in " + strShaderType + " shader " + strShaderFile + ":\n" + std::string(strInfoLog);
			delete[] strInfoLog;
			Debug::Log(msg);
		}

		return shader;
	}


	GLuint Shader::CreateProgram(const std::vector<GLuint> &shaderList)
	{
		GLuint program = glCreateProgram();
		
		for (auto shader : shaderList)
			glAttachShader(program, shader);
		
		glBindAttribLocation(program, 0, "Vertex");
		glBindAttribLocation(program, 1, "Normal");
		glBindAttribLocation(program, 2, "UV");

		glLinkProgram(program);

		GLint status;
		glGetProgramiv(program, GL_LINK_STATUS, &status);
		if (status == GL_FALSE)
		{
			GLint infoLogLength;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

			GLchar *strInfoLog = new GLchar[infoLogLength + 1];
			glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
			std::string msg = "Linker failure: " + std::string(strInfoLog);
			delete[] strInfoLog;
			Debug::Error(msg);
		}

		for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
			glDetachShader(program, shaderList[iLoop]);

		return program;
	}
	
	
	GLuint Shader::GetUL(std::string var)
	{
		return glGetUniformLocation(ProgramId, var.c_str());
	}


	void Shader::MakeCurrent()
	{
		glUseProgram(ProgramId);
	}

	GLuint Shader::GetID()
	{
		return ProgramId;
	}

}