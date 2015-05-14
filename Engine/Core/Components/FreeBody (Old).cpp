#include "FreeBody.h"
#include <Scene/DynamicsWorld.h>
#include <Assets/Material.h>

using namespace Core::Components;
	

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
