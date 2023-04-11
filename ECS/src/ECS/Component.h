#pragma once
#include "EntityManager.h"

#include <iostream>

namespace ECS {

	class World;

	class Component
	{
	public:
		Component() : m_Entt(0), enabled(true) {}
		virtual ~Component() = default;
		virtual void Init() {}

		virtual const char* ToString() const = 0;

	public:
		bool enabled = true;
	protected:
		Entity m_Entt;
		World* world = nullptr;
		friend class World;
	};

	inline std::ostream& operator<<(std::ostream& os, const Component& e)
	{
		return os << e.ToString();
	}
}