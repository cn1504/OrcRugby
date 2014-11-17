#include "Entity.h"
#include "Renderable.h"
#include <typeinfo>

#include "Mesh.h"
#include "Material.h"

namespace Core
{

	Entity::Entity()
	{
		renderable = nullptr;
	}


	Entity::~Entity()
	{
		for (auto c : Components)
		{
			Mesh* r = dynamic_cast<Mesh*>(c);
			if (r != nullptr)
				continue;

			Material* s = dynamic_cast<Material*>(c);
			if (s != nullptr)
				continue;

			delete c;
		}
	}


	void Entity::Load()
	{
		for (auto c : Components)
			c->Load();
	}


	void Entity::Update()
	{
		for (auto c : Components)
			c->Update();
	}


	void Entity::AddChild(Entity* c)
	{
		Children.push_back(c);
	}


	void Entity::AddComponent(Component* c)
	{
		c->Entity = this;
		Components.push_back(c);

		Renderable* r = dynamic_cast<Renderable*>(c);
		if (r != nullptr)
			renderable = r;
	}


	void Entity::AddOrReplaceComponent(Component* a, Component* b)
	{
		if (a != nullptr)
		{
			for (size_t i = 0; i < Components.size(); i++)
			{
				auto c = Components[i];

				if (c == a)
				{
					Renderable* r = dynamic_cast<Renderable*>(b);
					if (r != nullptr)
						renderable = r;

					delete c;
					Components[i] = b;

					return;
				}
			}
		}

		AddComponent(b);
	}


	bool Entity::IsRenderable()
	{
		return renderable != nullptr;
	}


	Renderable* Entity::GetRenderable()
	{
		return renderable;
	}

}