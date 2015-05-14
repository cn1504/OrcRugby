#include "PointCloud.h"
#include <Renderers/Shader.h>

using namespace Core::Assets;
typedef Core::Renderers::Shader Shader;

PointCloud::PointCloud()
{
}

PointCloud::PointCloud(const std::string& filename)
{
	// Import mesh file
	std::fstream pcFile;
	pcFile.open("Resources/Meshes/" + filename, std::ios::in | std::ios::binary);

	auto size = geometry.size();
	auto size1 = geometry.size();
	pcFile.read((char*)&size1, sizeof(size1));
		
	for (int i = 0; i < size1; i++)
	{
		auto lvl = new PointCloudDetailLevel;
		geometry.push_back(lvl);

		pcFile.read((char*)&lvl->Resolution, sizeof(lvl->Resolution));

		pcFile.read((char*)&size, sizeof(size));
		lvl->vertices.resize(size);
		for (unsigned int i = 0; i < size; i++)
			pcFile.read((char*)&lvl->vertices[i], sizeof(lvl->vertices[i]));

		pcFile.read((char*)&size, sizeof(size));
		lvl->uvs.resize(size);
		for (unsigned int i = 0; i < size; i++)
			pcFile.read((char*)&lvl->uvs[i], sizeof(lvl->uvs[i]));

		pcFile.read((char*)&size, sizeof(size));
		lvl->normals.resize(size);
		for (unsigned int i = 0; i < size; i++)
			pcFile.read((char*)&lvl->normals[i], sizeof(lvl->normals[i]));

		lvl->IndexSize = (GLsizei)lvl->vertices.size();
	}		
		
	pcFile.close();

	for (auto lvl : geometry)
	{
		// Load vectors into VBOs
		glGenBuffers(1, &lvl->VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, lvl->VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, lvl->vertices.size() * sizeof(glm::vec3), &lvl->vertices[0], GL_STATIC_DRAW);
		Debug::GLError("ERROR: Could not generate the Vertex buffer object.");

		glGenBuffers(1, &lvl->UvBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, lvl->UvBuffer);
		glBufferData(GL_ARRAY_BUFFER, lvl->uvs.size() * sizeof(glm::vec2), &lvl->uvs[0], GL_STATIC_DRAW);
		Debug::GLError("ERROR: Could not generate the Vertex buffer object.");

		glGenBuffers(1, &lvl->NormalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, lvl->NormalBuffer);
		glBufferData(GL_ARRAY_BUFFER, lvl->normals.size() * sizeof(glm::vec3), &lvl->normals[0], GL_STATIC_DRAW);
		Debug::GLError("ERROR: Could not generate the Normals buffer object.");
	}
		
}

PointCloud::~PointCloud()
{
	for (auto lvl : geometry)
	{
		glDeleteBuffers(1, &lvl->VertexBuffer);
		glDeleteBuffers(1, &lvl->UvBuffer);
		glDeleteBuffers(1, &lvl->NormalBuffer);

		delete lvl;
	}

	geometry.clear();
}
	
void PointCloud::Load()
{}

void PointCloud::Update()
{}

void PointCloud::Draw(Shader* shader)
{
	//auto viewDistance = glm::distance(Entity->Transform.Position, ViewPosition);
	auto viewDistance = 5.0f;
	currentLevel = (int)geometry.size() - 1;
	auto lvl = geometry[currentLevel];
	auto requiredResolution = glm::tan(glm::radians(Settings::Video::FOVY / Settings::Window::Height)) * viewDistance;
	while (lvl->Resolution > requiredResolution)
	{
		currentLevel--;
		if (currentLevel < 0)
		{
			currentLevel = 0;
			break;
		}
		lvl = geometry[currentLevel];
	}


	shader->SetVec3Attribute("Vertex", lvl->VertexBuffer);
	shader->SetVec3Attribute("Normal", lvl->NormalBuffer);
	shader->SetVec2Attribute("Uv", lvl->UvBuffer);
	Debug::GLError("ERROR: Could not set the point cloud vbas.");
	
	// Draw the points !
	glDrawArrays(
		GL_POINTS,
		0,
		lvl->IndexSize
		);
	Debug::GLError("ERROR: Could not draw the point cloud.");

	Time::DrawCallsPerFrame++;

	shader->DisableAttribute("Vertex");
	shader->DisableAttribute("Normal");
	shader->DisableAttribute("Uv");
}

void PointCloud::SaveToFile(const std::string& filename)
{
	std::fstream pcFile;
	pcFile.open("Resources/Meshes/" + filename, std::ios::out | std::ios::binary);

	auto size = geometry.size();
	pcFile.write((char*)&size, sizeof(size));

	for (auto lvl : geometry)
	{
		pcFile.write((char*)&lvl->Resolution, sizeof(lvl->Resolution));

		size = lvl->vertices.size();
		pcFile.write((char*)&size, sizeof(size));
		pcFile.write((char*)& lvl->vertices[0], lvl->vertices.size() * sizeof(lvl->vertices[0]));

		size = lvl->uvs.size();
		pcFile.write((char*)&size, sizeof(size));
		pcFile.write((char*)& lvl->uvs[0], lvl->uvs.size() * sizeof(lvl->uvs[0]));

		size = lvl->normals.size();
		pcFile.write((char*)&size, sizeof(size));
		pcFile.write((char*)& lvl->normals[0], lvl->normals.size() * sizeof(lvl->normals[0]));
	}

	pcFile.close();
}

void PointCloud::BuildFromMesh(const std::vector<unsigned short>& indices, const std::vector<glm::vec3>& indexed_vertices, const std::vector<glm::vec2>& indexed_uvs, const std::vector<glm::vec3>& indexed_normals)
{
	if (geometry.size() > 0)
		return;

	auto lvl = new PointCloudDetailLevel;
	geometry.push_back(lvl);

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
	lvl->IndexSize = (GLsizei)lvl->vertices.size();

	// Join points that are too close to each other
	auto pointSize = glm::tan(glm::radians(Settings::Video::FOVY / (float)Settings::Window::Height)) * Settings::Video::MinDrawDistance;
	BuildDetailLevel(0, pointSize);
	BuildDetailLevel(1, pointSize * 2.0f);
	BuildDetailLevel(2, pointSize * 4.0f);
	BuildDetailLevel(3, pointSize * 7.0f);
	BuildDetailLevel(4, pointSize * 12.0f);
	BuildDetailLevel(5, pointSize * 20.0f);
}

void PointCloud::Subdivide(const glm::vec3& u, const glm::vec3& v, const glm::vec3& w, const glm::vec2& uv_u, const glm::vec2& uv_v, const glm::vec2& uv_w, const glm::vec3& norm_u, const glm::vec3& norm_v, const glm::vec3& norm_w)
{
	auto pointSize = glm::tan(glm::radians(Settings::Video::FOVY / (float)Settings::Window::Height)) * Settings::Video::MinDrawDistance;
	auto lvl = geometry[0];

	float area = 0.5f * glm::length(glm::cross(v - u, w - u));
	if (area < pointSize * pointSize)
	{
		lvl->vertices.push_back((u + v + w) / 3.0f);
		lvl->uvs.push_back((uv_u + uv_v + uv_w) / 3.0f);
		lvl->normals.push_back(glm::normalize((norm_u + norm_v + norm_w) / 3.0f));
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

void PointCloud::BuildDetailLevel(int lvl, float resolution)
{
	Debug::Log("Level and Resolution: " + std::to_string(lvl) + ", " + std::to_string(resolution));

	auto src = geometry[(lvl == 0) ? 0 : lvl - 1];
	PointCloudDetailLevel* dst = src;

	if (lvl == 0)
	{
	}
	else
	{
		dst = new PointCloudDetailLevel;
		geometry.push_back(dst);		

		dst->Resolution = resolution;
		auto halfRes = resolution * 0.5f;

		Debug::Log("Source point count: " + std::to_string(src->IndexSize));

		// Blend points within resolution distance apart that face the same direction
		float cosThetaBlendThreshold = glm::cos(glm::radians(45.0f));

		for (int i = 0; i < src->IndexSize; i++)
		{
			auto srcPoint = src->vertices[i];
			bool found = false;
			for (int j = 0; j < dst->vertices.size(); j++)
			{
				// Bounding Box rejection
				auto dstPoint = dst->vertices[j];
				if ((srcPoint.x - halfRes) < dstPoint.x && dstPoint.x < (srcPoint.x + halfRes))
				{
					if ((srcPoint.y - halfRes) < dstPoint.y && dstPoint.y < (srcPoint.y + halfRes))
					{
						if ((srcPoint.z - halfRes) < dstPoint.z && dstPoint.z < (srcPoint.z + halfRes))
						{
							if (glm::distance(srcPoint, dstPoint) < (halfRes * 0.5f))
							{
								if (glm::dot(src->normals[i], dst->normals[j]) > cosThetaBlendThreshold)
								{
									found = true;
								}
							}
						}
					}
				}
			}

			if (found == false)
			{
				if (i % 1000 == 0)
				{
					Debug::Log("Point " + std::to_string(i) + ": " + std::to_string((float)i / (float)src->IndexSize));
				}
				dst->vertices.push_back(src->vertices[i]);
				dst->uvs.push_back(src->uvs[i]);
				dst->normals.push_back(src->normals[i]);
			}
		}

	}

	dst->IndexSize = (GLsizei)dst->vertices.size();
		
	// Load vectors into VBOs
	glGenBuffers(1, &dst->VertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, dst->VertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, dst->vertices.size() * sizeof(glm::vec3), &dst->vertices[0], GL_STATIC_DRAW);
	Debug::GLError("ERROR: Could not generate the Vertex buffer object.");

	glGenBuffers(1, &dst->UvBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, dst->UvBuffer);
	glBufferData(GL_ARRAY_BUFFER, dst->uvs.size() * sizeof(glm::vec2), &dst->uvs[0], GL_STATIC_DRAW);
	Debug::GLError("ERROR: Could not generate the Vertex buffer object.");

	glGenBuffers(1, &dst->NormalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, dst->NormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, dst->normals.size() * sizeof(glm::vec3), &dst->normals[0], GL_STATIC_DRAW);
	Debug::GLError("ERROR: Could not generate the Normals buffer object.");
		
	Debug::Log("Level point count: " + std::to_string(dst->IndexSize));
}


void PointCloud::BuildTestObject()
{
	if (geometry.size() > 0)
		return;

	// Point cloud cube surface
	// minimum pointSize = viewDistance * tan(FOVY / WindowHeight);
	auto pointSize = glm::tan(glm::radians(Settings::Video::FOVY / (float)Settings::Window::Height)) * Settings::Video::MinDrawDistance;

	auto dl = new PointCloudDetailLevel;
	geometry.push_back(dl);
		
	// Unit Cube	
	float halfExtend = 0.5f;		
	float onethird = 1.0f / 3.0f;

	// -x face
	float x = -halfExtend;
	float y = -halfExtend;
	float z = -halfExtend;
	glm::vec3 normal(-1.0f, 0.0f, 0.0f);/*
	while (y <= halfExtend)
	{
		while (z <= halfExtend)
		{
			dl->vertices.push_back(glm::vec3(x, y, z));
			dl->normals.push_back(normal);
			dl->uvs.push_back(glm::vec2((y + halfExtend) / (2.0f * halfExtend) * onethird, (z + halfExtend) / (2.0f * halfExtend) * onethird));
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
			dl->vertices.push_back(glm::vec3(x, y, z));
			dl->normals.push_back(normal);
			dl->uvs.push_back(glm::vec2(onethird + (y + halfExtend) / (2.0f * halfExtend) * onethird, (z + halfExtend) / (2.0f * halfExtend) * onethird));
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
			dl->vertices.push_back(glm::vec3(x, y, z));
			dl->normals.push_back(normal);
			dl->uvs.push_back(glm::vec2(2.0f * onethird + (x + halfExtend) / (2.0f * halfExtend) * onethird, (z + halfExtend) / (2.0f * halfExtend) * onethird));
			z += pointSize;
		}
		z = -halfExtend;
		x += pointSize;
	}
	*/
	// +y face
	x = -halfExtend;
	y = halfExtend;
	z = -halfExtend;
	normal = glm::vec3(0.0f, 1.0f, 0.0f);
	while (x <= halfExtend)
	{
		while (z <= halfExtend)
		{
			dl->vertices.push_back(glm::vec3(x, y, z));
			dl->normals.push_back(normal);
			dl->uvs.push_back(glm::vec2((x + halfExtend) / (2.0f * halfExtend) * onethird, onethird + (z + halfExtend) / (2.0f * halfExtend) * onethird));
			z += pointSize;
		}
		z = -halfExtend;
		x += pointSize;
	}
	/*
	// -z face
	x = -halfExtend;
	y = -halfExtend;
	z = -halfExtend;
	normal = glm::vec3(0.0f, 0.0f, -1.0f);
	while (x <= halfExtend)
	{
		while (y <= halfExtend)
		{
			dl->vertices.push_back(glm::vec3(x, y, z));
			dl->normals.push_back(normal);
			dl->uvs.push_back(glm::vec2(onethird + (x + halfExtend) / (2.0f * halfExtend) * onethird, onethird + (y + halfExtend) / (2.0f * halfExtend) * onethird));
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
			dl->vertices.push_back(glm::vec3(x, y, z));
			dl->normals.push_back(normal);
			dl->uvs.push_back(glm::vec2(2.0f * onethird + (x + halfExtend) / (2.0f * halfExtend) * onethird, onethird + (y + halfExtend) / (2.0f * halfExtend) * onethird));
			y += pointSize;
		}
		y = -halfExtend;
		x += pointSize;
	}
	*/
	dl->IndexSize = (GLsizei)dl->vertices.size();
		
	// Build detail levels
	pointSize = glm::tan(glm::radians(Settings::Video::FOVY / Settings::Window::Height)) * Settings::Video::MinDrawDistance;
	BuildDetailLevel(0, pointSize);
	BuildDetailLevel(1, pointSize * 2.0f);
	BuildDetailLevel(2, pointSize * 4.0f);
	BuildDetailLevel(3, pointSize * 7.0f);
	BuildDetailLevel(4, pointSize * 12.0f);
	BuildDetailLevel(5, pointSize * 20.0f);

	SaveToFile("Cube.pc");
}