#pragma once
 
#include "hdpch.h"
#include "Hudi/Components/Collider2DComponent.h"

namespace Hudi {

#ifdef PHYSICS_ENGINE
	namespace algo {

		bool CheckBoxBox(Hudi::BoxCollider2D* ca, BoxCollider2D* cb)
		{
			BoxCollider2D newCa = *ca;

			BoxCollider2D newCb = *cb;

			if (newCa.maxPoint.x < newCb.minPoint.x || newCa.minPoint.x > newCb.maxPoint.x)
				return false;
			if (newCa.maxPoint.y < newCb.minPoint.y || newCa.minPoint.y > newCb.maxPoint.y)
				return false;

			return true;
		}

		bool CheckCirCir(CircleCollider2D* ca, CircleCollider2D* cb)
		{
			CircleCollider2D newCa = *ca;
			CircleCollider2D newCb = *cb;

			int AB2 = newCa.radius + newCb.radius;
			AB2 *= AB2;

			float distance2 = (newCa.center.x - newCb.center.x) * (ca->center.x - newCb.center.x)
				+ (ca->center.y - newCb.center.y) * (ca->center.y - newCb.center.y);

			return distance2 <= AB2;
		}

		bool CheckBoxCir(BoxCollider2D* ca, CircleCollider2D* cb)
		{
			return false;
		}

	}

	bool BoxCollider2D::TestCollision(Collider* cb)
	{
		BoxCollider2D* boxColliderB = dynamic_cast<BoxCollider2D*>(cb);
		if (boxColliderB) 
		{
			return algo::CheckBoxBox(this, boxColliderB);
		}

		CircleCollider2D* circleColliderB = dynamic_cast<CircleCollider2D*>(cb);
		if (circleColliderB)
		{
			return algo::CheckBoxCir(this, circleColliderB);
		}

		return false;
	}

	bool CircleCollider2D::TestCollision(Collider* cb) 
	{
		BoxCollider2D* boxColliderB = dynamic_cast<BoxCollider2D*>(cb);
		if (boxColliderB) 
		{
			return algo::CheckBoxCir(boxColliderB, this);
		}

		CircleCollider2D* cirColliderB = dynamic_cast<CircleCollider2D*>(cb);
		if (cirColliderB) 
		{
			return algo::CheckCirCir(this, cirColliderB);
		}

		return false;
	}
#endif

}