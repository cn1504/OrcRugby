#include "FreeBody.h"
#include "DynamicsWorld.h"
#include "Material.h"

namespace Core
{

	FreeBody::FreeBody(DynamicsWorld* world, Material* material)
	{
		this->world = world;
		this->material = material;
	}


	FreeBody::~FreeBody()
	{
	}


	void FreeBody::Load()
	{
	}


	void FreeBody::Update()
	{
	}

}