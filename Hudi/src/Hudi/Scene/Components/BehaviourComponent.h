#pragma once

#include "Component.h"

#include "Hudi/Scene/Physics/Collision2D.h"
#include "Collider2DComponent.h"

namespace Hudi {

	class Behaviour : public Component
	{
	public:
		virtual ~Behaviour() = default;
		virtual const char* ToString() const override { return "Behaviour"; }

	};


	class MonoBehaviour : public Behaviour
	{
	public:
		virtual ~MonoBehaviour() = default;
		virtual const char* ToString() const override { return "MonoBehaviour"; }

		//virtual void OnCollisionEnter2D(Collision2D collision) {}
	};

}