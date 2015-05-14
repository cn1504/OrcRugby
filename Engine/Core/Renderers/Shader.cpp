#include "Shader.h"
#include <Assets/Texture.h>
using namespace Core::Renderers;


Shader::Shader(std::string vertex, std::string fragment)
{
	std::string shadersFolder = "Resources/Shaders/";

	std::vector<GLuint> shaderList;

	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, shadersFolder + vertex));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, shadersFolder + fragment));
	
	ProgramId = CreateProgram(shaderList);

	for (auto shader : shaderList)
		glDeleteShader(shader);
}

Shader::Shader(std::string vertex, std::string geometry, std::string fragment)
{
	std::string shadersFolder = "Resources/Shaders/";

	std::vector<GLuint> shaderList;

	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, shadersFolder + vertex));
	shaderList.push_back(CreateShader(GL_GEOMETRY_SHADER, shadersFolder + geometry));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, shadersFolder + fragment));

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

Shader::Shader(std::string vertex, std::string tess_control, std::string tess_eval, std::string geometry, std::string fragment)
{
	std::string shadersFolder = "Resources/Shaders/";

	std::vector<GLuint> shaderList;

	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, shadersFolder + vertex));
	shaderList.push_back(CreateShader(GL_TESS_CONTROL_SHADER, shadersFolder + tess_control));
	shaderList.push_back(CreateShader(GL_TESS_EVALUATION_SHADER, shadersFolder + tess_eval));
	shaderList.push_back(CreateShader(GL_GEOMETRY_SHADER, shadersFolder + geometry));
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
		Debug->Error(("Could not open: " + shaderFilepath).c_str());
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
		Debug->Log(msg);
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
		Debug->Error(msg);
	}

	for (size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glDetachShader(program, shaderList[iLoop]);

	return program;
}


void Shader::Activate()
{
	glUseProgram(ProgramId);
}


void Shader::SetUniform(std::string name, int value)
{
	glUniform1i(glGetUniformLocation(ProgramId, name.c_str()), value);
}
void Shader::SetUniform(std::string name, float value)
{
	glUniform1f(glGetUniformLocation(ProgramId, name.c_str()), value);
}
void Shader::SetUniform(std::string name, const glm::vec2& value)
{
	glUniform2fv(glGetUniformLocation(ProgramId, name.c_str()), 1, glm::value_ptr(value));
}
void Shader::SetUniform(std::string name, const glm::vec3& value)
{
	glUniform3fv(glGetUniformLocation(ProgramId, name.c_str()), 1, glm::value_ptr(value));
}
void Shader::SetUniform(std::string name, const glm::vec4& value)
{
	glUniform4fv(glGetUniformLocation(ProgramId, name.c_str()), 1, glm::value_ptr(value));
}
void Shader::SetUniform(std::string name, const glm::dvec2& value)
{
	glUniform2dv(glGetUniformLocation(ProgramId, name.c_str()), 1, glm::value_ptr(value));
}
void Shader::SetUniform(std::string name, const glm::dvec3& value)
{
	glUniform3dv(glGetUniformLocation(ProgramId, name.c_str()), 1, glm::value_ptr(value));
}
void Shader::SetUniform(std::string name, const glm::dvec4& value)
{
	glUniform4dv(glGetUniformLocation(ProgramId, name.c_str()), 1, glm::value_ptr(value));
}
void Shader::SetUniform(std::string name, const glm::ivec2& value)
{
	glUniform2iv(glGetUniformLocation(ProgramId, name.c_str()), 1, glm::value_ptr(value));
}
void Shader::SetUniform(std::string name, const glm::ivec3& value)
{
	glUniform3iv(glGetUniformLocation(ProgramId, name.c_str()), 1, glm::value_ptr(value));
}
void Shader::SetUniform(std::string name, const glm::ivec4& value)
{
	glUniform4iv(glGetUniformLocation(ProgramId, name.c_str()), 1, glm::value_ptr(value));
}
void Shader::SetUniform(std::string name, const glm::mat3& value)
{
	glUniformMatrix3fv(glGetUniformLocation(ProgramId, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
void Shader::SetUniform(std::string name, const glm::mat4& value)
{
	glUniformMatrix4fv(glGetUniformLocation(ProgramId, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}


void Shader::SetVecAttribute(std::string name, GLuint buffer, int size, int stride)
{
	auto a = glGetAttribLocation(ProgramId, name.c_str());
	if (a >= 0)
	{
		glEnableVertexAttribArray(a);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glVertexAttribPointer(
			a,                  // attribute
			size,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			stride,                  // stride
			(void*)0            // array buffer offset
			);
	}
}
void Shader::SetVec2Attribute(std::string name, GLuint buffer)
{
	SetVecAttribute(name, buffer, 2, 0);
}
void Shader::SetVec3Attribute(std::string name, GLuint buffer)
{
	SetVecAttribute(name, buffer, 3, 0);
}
void Shader::SetVec4Attribute(std::string name, GLuint buffer)
{
	SetVecAttribute(name, buffer, 4, 0);
}


void Shader::SetIVecAttribute(std::string name, GLuint buffer, int size, int stride)
{
	auto a = glGetAttribLocation(ProgramId, name.c_str());
	if (a >= 0)
	{
		glEnableVertexAttribArray(a);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glVertexAttribIPointer(
			a,                                // attribute
			size,                             // size
			GL_INT,							  // type
			stride,                           // stride
			(void*)0                          // array buffer offset
			);
	}
}
void Shader::SetIVec2Attribute(std::string name, GLuint buffer)
{
	SetIVecAttribute(name, buffer, 2, 0);
}
void Shader::SetIVec3Attribute(std::string name, GLuint buffer)
{
	SetIVecAttribute(name, buffer, 3, 0);
}
void Shader::SetIVec4Attribute(std::string name, GLuint buffer)
{
	SetIVecAttribute(name, buffer, 4, 0);
}


void Shader::SetDVecAttribute(std::string name, GLuint buffer, int size, int stride)
{
	auto a = glGetAttribLocation(ProgramId, name.c_str());
	if (a >= 0)
	{
		glEnableVertexAttribArray(a);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glVertexAttribLPointer(
			a,                                // attribute
			size,                             // size
			GL_INT,							  // type
			stride,                           // stride
			(void*)0                          // array buffer offset
			);
	}
}
void Shader::SetDVec2Attribute(std::string name, GLuint buffer)
{
	SetDVecAttribute(name, buffer, 2, 0);
}
void Shader::SetDVec3Attribute(std::string name, GLuint buffer)
{
	SetDVecAttribute(name, buffer, 3, 0);
}
void Shader::SetDVec4Attribute(std::string name, GLuint buffer)
{
	SetDVecAttribute(name, buffer, 4, 0);
}


void Shader::SetElementBuffer(GLuint buffer)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
}


void Shader::DisableAttribute(std::string name)
{
	auto a = glGetAttribLocation(ProgramId, name.c_str());
	if (a >= 0)
		glDisableVertexAttribArray(a);
}

void Shader::SetTexture(std::string name, const Core::Assets::Texture& texture, int value)
{
	glActiveTexture(GL_TEXTURE0 + value);
	glBindTexture(GL_TEXTURE_2D, texture.GetID());
	SetUniform(name, value);
}