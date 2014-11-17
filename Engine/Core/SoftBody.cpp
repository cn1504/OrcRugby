#include "SoftBody.h"
#include "DynamicsWorld.h"
#include "Material.h"
#include "Mesh.h"

namespace Core
{

	SoftBody::SoftBody(DynamicsWorld* world, Core::Material* material, btSoftBody* body, Mesh* mesh)
		: FreeBody(world, material)
	{
		this->body = body;
		this->mesh = mesh;
	}


	SoftBody::~SoftBody()
	{
	}


	void SoftBody::Load()
	{
		body->setUserPointer(this);
		world->AddBody(this);
	}


	btSoftBody* SoftBody::GetBody()
	{
		return body;
	}


	void SoftBody::RebuildMesh()
	{
		// Get faces
		btSoftBody::tFaceArray&   faces(body->m_faces);

		std::vector<unsigned short> indices(faces.size() * 3);
		std::vector<glm::vec3> indexed_vertices(faces.size() * 3);
		std::vector<glm::vec3> indexed_normals(faces.size() * 3);

		for (int j = 0; j < faces.size(); ++j)
		{
			btSoftBody::Node*   node_0 = faces[j].m_n[0];
			btSoftBody::Node*   node_1 = faces[j].m_n[1];
			btSoftBody::Node*   node_2 = faces[j].m_n[2];

			indexed_vertices[j * 3] = glm::vec3(node_0->m_x.getX(), node_0->m_x.getY(), node_0->m_x.getZ());
			indexed_vertices[j * 3 + 1] = glm::vec3(node_1->m_x.getX(), node_1->m_x.getY(), node_1->m_x.getZ());
			indexed_vertices[j * 3 + 2] = glm::vec3(node_2->m_x.getX(), node_2->m_x.getY(), node_2->m_x.getZ());

			indexed_normals[j * 3] = glm::vec3(node_0->m_n.getX(), node_0->m_n.getY(), node_0->m_n.getZ());
			indexed_normals[j * 3 + 1] = glm::vec3(node_1->m_n.getX(), node_1->m_n.getY(), node_1->m_n.getZ());
			indexed_normals[j * 3 + 2] = glm::vec3(node_2->m_n.getX(), node_2->m_n.getY(), node_2->m_n.getZ());
			
			indices[j * 3] = j * 3;
			indices[j * 3 + 1] = j * 3 + 1;
			indices[j * 3 + 2] = j * 3 + 2;
		}
		
		mesh->UpdateGeometry(indices, indexed_vertices, indexed_normals);
	}

}