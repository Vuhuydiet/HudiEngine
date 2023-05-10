#pragma once

#include <imgui.h>
#include <glm/glm.hpp>

namespace Hudi {

    void DrawFloat2Control(const char* label, glm::vec2& values, float resetValue = 0.0f, float dragSpeed = 0.1f, float columnWidth = 70.0f);

    void DrawFloat3Control(const char* label, glm::vec3& values, float resetValue = 0.0f, float dragSpeed = 0.1f, float columnWidth = 70.0f);

}