#pragma once
#include <Core.h>
#include <Components/Component.h>
#include <Space/Transform.h>
#include <Renderers/Renderable.h>

namespace Core
{
	class Entity
	{
	private:
		std::vector<Entity*> Children;
		std::vector<Core::Components::Component*> Components;
		Core::Renderers::Renderable* renderable;

	public:
		Core::Space::Transform Transform;

		Entity();
		~Entity();

		void Load();
		void Update();
		void AddChild(Entity* c);
		void AddComponent(Core::Components::Component* c);
		void AddOrReplaceComponent(Core::Components::Component* a, Core::Components::Component* b);

		bool IsRenderable();
		Core::Renderers::Renderable* GetRenderable();

		template<class T>
		T* GetComponent();
	};


	template<class T>
	T* Entity::GetComponent()
	{
		for (auto c : Components)
		{
			T* t = dynamic_cast<T*>(c);
			if (t != nullptr)
				return t;
		}
		return nullptr;
	}

}