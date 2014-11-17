#include "Mesh.h"
#include "Assets.h"

namespace Core
{
	
	Mesh::Mesh(std::string filename)
	{
		// Import mesh file
		std::fstream meshFile;
		meshFile.open("Resources/Meshes/" + filename, std::ios::in | std::ios::binary);

		auto size = indices.size();
		meshFile.read((char*)&size, sizeof(size));
		indices.resize(size);
		for (unsigned int i = 0; i < size; i++)
			meshFile.read((char*)&indices[i], sizeof(indices[i]));
		
		meshFile.read((char*)&size, sizeof(size));
		indexed_vertices.resize(size);
		for (unsigned int i = 0; i < size; i++)
			meshFile.read((char*)&indexed_vertices[i], sizeof(indexed_vertices[i]));

		meshFile.read((char*)&size, sizeof(size));
		indexed_uvs.resize(size);
		for (unsigned int i = 0; i < size; i++)
			meshFile.read((char*)&indexed_uvs[i], sizeof(indexed_uvs[i]));

		meshFile.read((char*)&size, sizeof(size));
		indexed_normals.resize(size);
		for (unsigned int i = 0; i < size; i++)
			meshFile.read((char*)&indexed_normals[i], sizeof(indexed_normals[i]));
		
		meshFile.read((char*)&hasBones, sizeof(hasBones));
		if (hasBones)
		{
			meshFile.read((char*)&size, sizeof(size));
			indexed_boneIndices.resize(size);
			for (unsigned int i = 0; i < size; i++)
				meshFile.read((char*)&indexed_boneIndices[i], sizeof(indexed_boneIndices[i]));

			meshFile.read((char*)&size, sizeof(size));
			indexed_boneWeights.resize(size);
			for (unsigned int i = 0; i < size; i++)
				meshFile.read((char*)&indexed_boneWeights[i], sizeof(indexed_boneWeights[i]));
		}

		meshFile.close();


		IndexSize = (GLsizei)indices.size();

		// Load vectors into VBOs
		glGenBuffers(1, &VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);
		Debug::GLError("ERROR: Could not generate the Vertex buffer object.");

		glGenBuffers(1, &UvBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, UvBuffer);
		glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);
		Debug::GLError("ERROR: Could not generate the Vertex buffer object.");

		glGenBuffers(1, &NormalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
		glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);
		Debug::GLError("ERROR: Could not generate the Normals buffer object.");

		// Generate a buffer for the indices as well
		glGenBuffers(1, &ElementBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
		Debug::GLError("ERROR: Could not generate the Indices buffer object.");

		if (hasBones)
		{
			glGenBuffers(1, &BoneIndexBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, BoneIndexBuffer);
			glBufferData(GL_ARRAY_BUFFER, indexed_boneIndices.size() * sizeof(glm::ivec4), &indexed_boneIndices[0], GL_STATIC_DRAW);
			Debug::GLError("ERROR: Could not generate the Bone Indices buffer object");

			glGenBuffers(1, &BoneWeightBuffer);
			glBindBuffer(GL_ARRAY_BUFFER, BoneWeightBuffer);
			glBufferData(GL_ARRAY_BUFFER, indexed_boneWeights.size() * sizeof(glm::vec4), &indexed_boneWeights[0], GL_STATIC_DRAW);
			Debug::GLError("ERROR: Could not generate the Bone Indices buffer object");
		}
	}

	Mesh::Mesh(std::vector<unsigned short> indices, std::vector<glm::vec3> indexed_vertices, std::vector<glm::vec2> indexed_uvs, std::vector<glm::vec3> indexed_normals)
	{
		IndexSize = (GLsizei)indices.size();
		
		// Load vectors into VBOs
		glGenBuffers(1, &VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);
		Debug::GLError("ERROR: Could not generate the Vertex buffer object.");
		
		glGenBuffers(1, &UvBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, UvBuffer);
		glBufferData(GL_ARRAY_BUFFER, indexed_uvs.size() * sizeof(glm::vec2), &indexed_uvs[0], GL_STATIC_DRAW);
		Debug::GLError("ERROR: Could not generate the Vertex buffer object.");

		glGenBuffers(1, &NormalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
		glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);
		Debug::GLError("ERROR: Could not generate the Normals buffer object.");

		// Generate a buffer for the indices as well
		glGenBuffers(1, &ElementBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
		Debug::GLError("ERROR: Could not generate the Indices buffer object.");

		hasBones = false;
	}


	Mesh::Mesh(Mesh* other)
	{
		IndexSize = other->IndexSize;
		VertexBuffer = other->VertexBuffer;
		UvBuffer = other->UvBuffer;
		NormalBuffer = other->NormalBuffer;
		ElementBuffer = other->ElementBuffer;

		hasBones = other->hasBones;
		BoneIndexBuffer = other->BoneIndexBuffer;
		BoneWeightBuffer = other->BoneWeightBuffer;
	}


	Mesh::~Mesh()
	{
		glDeleteBuffers(1, &VertexBuffer);
		glDeleteBuffers(1, &UvBuffer);
		glDeleteBuffers(1, &NormalBuffer);
		glDeleteBuffers(1, &ElementBuffer);
		if (hasBones)
		{
			glDeleteBuffers(1, &BoneIndexBuffer);
			glDeleteBuffers(1, &BoneWeightBuffer);
		}
	}

	
	void Mesh::EnableBuffers(Shader* shader)
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

		if (hasBones)
		{
			// 4th attribute buffer : bone ids
			a = glGetAttribLocation(shader->GetID(), "BoneID");
			if (a >= 0)
			{
				glEnableVertexAttribArray(a);
				glBindBuffer(GL_ARRAY_BUFFER, BoneIndexBuffer);
				glVertexAttribIPointer(
					a,                                // attribute
					4,                                // size
					GL_INT,							  // type
					0,                                // stride
					(void*)0                          // array buffer offset
					);
			}

			// 5th attribute buffer : bone weights
			a = glGetAttribLocation(shader->GetID(), "BoneWeight");
			if (a >= 0)
			{
				glEnableVertexAttribArray(a);
				glBindBuffer(GL_ARRAY_BUFFER, BoneWeightBuffer);
				glVertexAttribPointer(
					a,                                // attribute
					4,                                // size
					GL_FLOAT,                         // type
					GL_FALSE,                         // normalized?
					0,                                // stride
					(void*)0                          // array buffer offset
					);
			}
		}

		// Index buffer
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
		Debug::GLError("ERROR: Could not bind the mesh buffers.");
	}


	void Mesh::Render(Shader* shader)
	{
		// Draw the triangles !
		glDrawElements(
			GL_TRIANGLES,               // mode
			IndexSize,                  // count
			GL_UNSIGNED_SHORT,          // type
			(void*)0                    // element array buffer offset
			);
		Debug::GLError("ERROR: Could not draw the elements.");

		Time::DrawCallsPerFrame++;
	}

	
	void Mesh::DisableBuffers(Shader* shader)
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
		if (hasBones)
		{
			a = glGetAttribLocation(shader->GetID(), "BoneID");
			if (a >= 0)
				glDisableVertexAttribArray(a);
			a = glGetAttribLocation(shader->GetID(), "BoneWeight");
			if (a >= 0)
				glDisableVertexAttribArray(a);
		}
		Debug::GLError("ERROR: Could not disable the mesh buffers.");
	}


	void Mesh::Load()
	{
	}


	void Mesh::Update()
	{
	}

	
	bool Mesh::HasBones()
	{
		return hasBones;
	}

	
	void Mesh::UpdateGeometry(std::vector<unsigned short> indices, std::vector<glm::vec3> indexed_vertices, std::vector<glm::vec3> indexed_normals)
	{
		if (hasBones)
		{
			return;
		}

		IndexSize = (GLsizei)indices.size();

		// Load vectors into VBOs
		glDeleteBuffers(1, &VertexBuffer);
		glGenBuffers(1, &VertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, indexed_vertices.size() * sizeof(glm::vec3), &indexed_vertices[0], GL_STATIC_DRAW);
		Debug::GLError("ERROR: Could not generate the Vertex buffer object.");
		
		glDeleteBuffers(1, &NormalBuffer);
		glGenBuffers(1, &NormalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, NormalBuffer);
		glBufferData(GL_ARRAY_BUFFER, indexed_normals.size() * sizeof(glm::vec3), &indexed_normals[0], GL_STATIC_DRAW);
		Debug::GLError("ERROR: Could not generate the Normals buffer object.");

		// Generate a buffer for the indices as well
		glDeleteBuffers(1, &ElementBuffer);
		glGenBuffers(1, &ElementBuffer);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ElementBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned short), &indices[0], GL_STATIC_DRAW);
		Debug::GLError("ERROR: Could not generate the Indices buffer object.");
	}


	btConvexTriangleMeshShape* Mesh::GenerateTriangleMeshShape()
	{
		auto triMesh = new btTriangleMesh();

		for (int i = 0; i < indices.size(); i++)
		{
			auto vertex1 = indexed_vertices[indices[i]];
			i++;
			auto vertex2 = indexed_vertices[indices[i]];
			i++;
			auto vertex3 = indexed_vertices[indices[i]];

			triMesh->addTriangle(
				btVector3(vertex1.x, vertex1.y, vertex1.z),
				btVector3(vertex2.x, vertex2.y, vertex2.z),
				btVector3(vertex3.x, vertex3.y, vertex3.z)
				);
		}

		return new btConvexTriangleMeshShape(triMesh);		
	}

}