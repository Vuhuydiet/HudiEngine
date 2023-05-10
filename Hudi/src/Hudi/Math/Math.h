#pragma once

#include <glm/glm.hpp>

namespace Hudi::Math {

	bool DecomposeTransform(const glm::mat4& transform, glm::vec3& position, glm::vec3& rotation, glm::vec3& scale);

}