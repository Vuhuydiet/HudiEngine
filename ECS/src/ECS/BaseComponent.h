#pragma once
#include "EntityManager.h"

#include <iostream>

namespace ECS {

	class World;

	class BaseComponent
	{
	public:
		BaseComponent() : m_Entt(0), enabled(true) {}
		virtual ~BaseComponent() = default;
		virtual void Init() {}

		virtual const char* ToString() const = 0;

	public:
		bool enabled = true;
	protected:
		Entity m_Entt;
		World* world = nullptr;
		friend class World;
	};

	inline std::ostream& operator<<(std::ostream& os, const BaseComponent& e)
	{
		return os << e.ToString();
	}

}