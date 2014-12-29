#include "PointCloud.h"

namespace Core
{

	PointCloud::PointCloud()
	{
	}

	PointCloud::PointCloud(const std::string& filename)
	{
		// Import mesh file
		std::fstream pcFile;
		pcFile.open("Resources/Meshes/" + filename, std::ios::in | std::ios::binary);

		auto size = vertices.size();

		pcFile.read((char*)&size, sizeof(size));
		vertices.resize(size);
		for (unsigned int i = 0; i < size; i++)
			pcFile.read((char*)&vertices[i], sizeof(vertices[i]));

		pcFile.read((char*)&size, sizeof(size));
		uvs.resize(size);
		for (unsigned int i = 0; i < size; i++)
			pcFile.read((char*)&uvs[i], sizeof(uvs[i]));

		pcFile.read((char*)&size, sizeof(size));
		normals.resize(size);
		for (unsigned int i = 0; i < size; i++)
			pcFile.read((char*)&normals[i], sizeof(normals[i]));
		
		pcFile.close();

		IndexSize = (GLsizei)vertices.size();

		// Load vectors into VBOs
		glGenBuffers(1, &VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
		Debug::GLError("ERROR: Could not generate the Vertex buffer object.");

		glGenBuffers(1, &UvBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, UvBuffer);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
		Debug::GLError("ERROR: Could not generate the Vertex buffer object.");

		glGenBuffers(1, &NormalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
		Debug::GLError("ERROR: Could not generate the Normals buffer object.");
	}

	PointCloud::~PointCloud()
	{
		glDeleteBuffers(1, &VertexBuffer);
		glDeleteBuffers(1, &UvBuffer);
		glDeleteBuffers(1, &NormalBuffer);
	}
	
	void PointCloud::Load()
	{}

	void PointCloud::Update()
	{}

	void PointCloud::EnableBuffers(Shader* shader)
	{
		// 1st attribute buffer : vertices
		auto a = glGetAttribLocation(shader->GetID(), "Vertex");
		if (a >= 0)
		{
			glEnableVertexAttribArray(a);
			glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
			glVertexAttribPointer(
				a,                  // attribute
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
				);
		}

		// 2nd attribute buffer : normals
		a = glGetAttribLocation(shader->GetID(), "Normal");
		if (a >= 0)
		{
			glEnableVertexAttribArray(a);
			glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
			glVertexAttribPointer(
				a,                  // attribute
				3,                  // size
				GL_FLOAT,           // type
				GL_TRUE,            // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
				);
		}

		// 3rd attribute buffer : uvs
		a = glGetAttribLocation(shader->GetID(), "Uv");
		if (a >= 0)
		{
			glEnableVertexAttribArray(a);
			glBindBuffer(GL_ARRAY_BUFFER, UvBuffer);
			glVertexAttribPointer(
				a,                  // attribute
				2,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
				);
		}
		Debug::GLError("ERROR: Could not set the point cloud vbas.");
	}

	void PointCloud::Render(Shader* shader)
	{
		// Draw the points !
		glDrawArrays(
			GL_POINTS,
			0,
			IndexSize
			);
		Debug::GLError("ERROR: Could not draw the point cloud.");

		Time::DrawCallsPerFrame++;
	}

	void PointCloud::DisableBuffers(Shader* shader)
	{
		auto a = glGetAttribLocation(shader->GetID(), "Vertex");
		if (a >= 0)
			glDisableVertexAttribArray(a);
		a = glGetAttribLocation(shader->GetID(), "Normal");
		if (a >= 0)
			glDisableVertexAttribArray(a);
		a = glGetAttribLocation(shader->GetID(), "Uv");
		if (a >= 0)
			glDisableVertexAttribArray(a);
	}

	void PointCloud::BuildFromMesh(const std::vector<unsigned short>& indices, const std::vector<glm::vec3>& indexed_vertices, const std::vector<glm::vec2>& indexed_uvs, const std::vector<glm::vec3>& indexed_normals)
	{
		for (int i = 0; i < indices.size(); i += 3)
		{
			glm::vec3 u = indexed_vertices[indices[i]];
			glm::vec3 v = indexed_vertices[indices[i+1]];
			glm::vec3 w = indexed_vertices[indices[i+2]];
			glm::vec2 uv_u = indexed_uvs[indices[i]];
			glm::vec2 uv_v = indexed_uvs[indices[i+1]];
			glm::vec2 uv_w = indexed_uvs[indices[i+2]];
			glm::vec3 norm_u = indexed_normals[indices[i]];
			glm::vec3 norm_v = indexed_normals[indices[i+1]];
			glm::vec3 norm_w = indexed_normals[indices[i+2]];
			
			Subdivide(u, v, w, uv_u, uv_v, uv_w, norm_u, norm_v, norm_w);
		}

		// Join points that are too close to each other

		IndexSize = (GLsizei)vertices.size();
		
		// Load vectors into VBOs
		glGenBuffers(1, &VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
		Debug::GLError("ERROR: Could not generate the Vertex buffer object.");

		glGenBuffers(1, &UvBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, UvBuffer);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
		Debug::GLError("ERROR: Could not generate the Vertex buffer object.");

		glGenBuffers(1, &NormalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
		Debug::GLError("ERROR: Could not generate the Normals buffer object.");
	}

	void PointCloud::Subdivide(const glm::vec3& u, const glm::vec3& v, const glm::vec3& w, const glm::vec2& uv_u, const glm::vec2& uv_v, const glm::vec2& uv_w, const glm::vec3& norm_u, const glm::vec3& norm_v, const glm::vec3& norm_w)
	{
		float pointSize = 0.005f;

		float area = 0.5f * glm::length(glm::cross(v - u, w - u));
		if (area < pointSize * pointSize)
		{
			vertices.push_back((u + v + w) / 3.0f);
			uvs.push_back((uv_u + uv_v + uv_w) / 3.0f);
			normals.push_back(glm::normalize((norm_u + norm_v + norm_w) / 3.0f));
			return;
		}

		glm::vec3 uv = glm::mix(u, v, 0.5f);
		glm::vec3 uw = glm::mix(u, w, 0.5f);
		glm::vec3 vw = glm::mix(v, w, 0.5f);
		glm::vec2 uv_uv = glm::mix(uv_u, uv_v, 0.5f);
		glm::vec2 uv_uw = glm::mix(uv_u, uv_w, 0.5f);
		glm::vec2 uv_vw = glm::mix(uv_v, uv_w, 0.5f);
		glm::vec3 norm_uv = glm::normalize(glm::mix(norm_u, norm_v, 0.5f));
		glm::vec3 norm_uw = glm::normalize(glm::mix(norm_u, norm_w, 0.5f));
		glm::vec3 norm_vw = glm::normalize(glm::mix(norm_v, norm_w, 0.5f));
				
		Subdivide(u, uv, uw, uv_u, uv_uv, uv_uw, norm_u, norm_uv, norm_uw);
		Subdivide(v, uv, vw, uv_v, uv_uv, uv_vw, norm_v, norm_uv, norm_vw);
		Subdivide(w, uw, vw, uv_w, uv_uw, uv_vw, norm_w, norm_uw, norm_vw);
		Subdivide(uv, uw, vw, uv_uv, uv_uw, uv_vw, norm_uv, norm_uw, norm_vw);
	}

	void PointCloud::SaveToFile(const std::string& filename)
	{
		std::fstream pcFile;
		pcFile.open("Resources/Meshes/" + filename, std::ios::out | std::ios::binary);

		auto size = vertices.size();

		pcFile.write((char*)&size, sizeof(size));
		pcFile.write((char*)&vertices[0], vertices.size() * sizeof(vertices[0]));

		size = uvs.size();
		pcFile.write((char*)&size, sizeof(size));
		pcFile.write((char*)&uvs[0], uvs.size() * sizeof(uvs[0]));

		size = normals.size();
		pcFile.write((char*)&size, sizeof(size));
		pcFile.write((char*)&normals[0], normals.size() * sizeof(normals[0]));

		pcFile.close();
	}


	void PointCloud::BuildTestObject()
	{
		// Point cloud cube surface
		// minimum pointSize = viewDistance * tan(FOVY / WindowHeight);
		float pointSize = 0.1f;
		float halfExtend = 0.5f;
		
		float onethird = 1.0f / 3.0f;

		// -x face
		float x = -halfExtend;
		float y = -halfExtend;
		float z = -halfExtend;
		glm::vec3 normal(-1.0f, 0.0f, 0.0f);
		while (y <= halfExtend)
		{
			while (z <= halfExtend)
			{
				vertices.push_back(glm::vec3(x, y, z));
				normals.push_back(normal);
				uvs.push_back(glm::vec2((y + halfExtend) / (2.0f * halfExtend) * onethird, (z + halfExtend) / (2.0f * halfExtend) * onethird));
				z += pointSize;
			}
			z = -halfExtend;
			y += pointSize;
		}

		// +x face
		x = halfExtend;
		y = -halfExtend;
		z = -halfExtend;
		normal = glm::vec3(1.0f, 0.0f, 0.0f);
		while (y <= halfExtend)
		{
			while (z <= halfExtend)
			{
				vertices.push_back(glm::vec3(x, y, z));
				normals.push_back(normal);
				uvs.push_back(glm::vec2(onethird + (y + halfExtend) / (2.0f * halfExtend) * onethird, (z + halfExtend) / (2.0f * halfExtend) * onethird));
				z += pointSize;
			}
			z = -halfExtend;
			y += pointSize;
		}

		// -y face
		x = -halfExtend;
		y = -halfExtend;
		z = -halfExtend;
		normal = glm::vec3(0.0f, -1.0f, 0.0f);
		while (x <= halfExtend)
		{
			while (z <= halfExtend)
			{
				vertices.push_back(glm::vec3(x, y, z));
				normals.push_back(normal);
				uvs.push_back(glm::vec2(2.0f * onethird + (x + halfExtend) / (2.0f * halfExtend) * onethird, (z + halfExtend) / (2.0f * halfExtend) * onethird));
				z += pointSize;
			}
			z = -halfExtend;
			x += pointSize;
		}

		// +y face
		x = -halfExtend;
		y = halfExtend;
		z = -halfExtend;
		normal = glm::vec3(0.0f, 1.0f, 0.0f);
		while (x <= halfExtend)
		{
			while (z <= halfExtend)
			{
				vertices.push_back(glm::vec3(x, y, z));
				normals.push_back(normal);
				uvs.push_back(glm::vec2((x + halfExtend) / (2.0f * halfExtend) * onethird, onethird + (z + halfExtend) / (2.0f * halfExtend) * onethird));
				z += pointSize;
			}
			z = -halfExtend;
			x += pointSize;
		}

		// -z face
		x = -halfExtend;
		y = -halfExtend;
		z = -halfExtend;
		normal = glm::vec3(0.0f, 0.0f, -1.0f);
		while (x <= halfExtend)
		{
			while (y <= halfExtend)
			{
				vertices.push_back(glm::vec3(x, y, z));
				normals.push_back(normal);
				uvs.push_back(glm::vec2(onethird + (x + halfExtend) / (2.0f * halfExtend) * onethird, onethird + (y + halfExtend) / (2.0f * halfExtend) * onethird));
				y += pointSize;
			}
			y = -halfExtend;
			x += pointSize;
		}

		// +z face
		x = -halfExtend;
		y = -halfExtend;
		z = halfExtend;
		normal = glm::vec3(0.0f, 0.0f, 1.0f);
		while (x <= halfExtend)
		{
			while (y <= halfExtend)
			{
				vertices.push_back(glm::vec3(x, y, z));
				normals.push_back(normal);
				uvs.push_back(glm::vec2(2.0f * onethird + (x + halfExtend) / (2.0f * halfExtend) * onethird, onethird + (y + halfExtend) / (2.0f * halfExtend) * onethird));
				y += pointSize;
			}
			y = -halfExtend;
			x += pointSize;
		}

		IndexSize = (GLsizei)vertices.size();

		SaveToFile("Cube.pc");

		// Load vectors into VBOs
		glGenBuffers(1, &VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
		Debug::GLError("ERROR: Could not generate the Vertex buffer object.");

		glGenBuffers(1, &UvBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, UvBuffer);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);
		Debug::GLError("ERROR: Could not generate the Vertex buffer object.");

		glGenBuffers(1, &NormalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);
		Debug::GLError("ERROR: Could not generate the Normals buffer object.");
	}

}