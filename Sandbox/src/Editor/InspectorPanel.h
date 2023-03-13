#pragma once

#include <Hudi.h>

namespace Hudi {

	class InspectorPanel
	{
	public:
		void OnImGuiRender();

	private:
		void OnDrawComponents(Ref<GameObject> obj);

		void OnDrawTransform(Ref<Transform> transform);
		void OnDrawSpriteRenderer(Ref<SpriteRenderer> sprite);
	private:
	};

}