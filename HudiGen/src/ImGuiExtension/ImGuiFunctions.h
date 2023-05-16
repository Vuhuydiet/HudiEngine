#pragma once

#include <imgui.h>
#include <glm/glm.hpp>

namespace Hudi {

    void DrawInputScalar(const char* label, ImGuiDataType data_type, void* p_data, const void* p_step = nullptr, const void* p_step_fast = nullptr, const char* format = NULL, ImGuiInputTextFlags flags = 0, float columnWidth = 140.0f);

    void DrawFloatControl(const char* label, float& value, float dragSpeed = 0.1f, float min = 0.0f, float max = 1.0f, const char* fmt = "%.3f", float columnWidth = 140.0f);

    void DrawFloat2Control(const char* label, glm::vec2& values, float resetValue = 0.0f, float dragSpeed = 0.1f, float columnWidth = 90.0f);

    void DrawFloat3Control(const char* label, glm::vec3& values, float resetValue = 0.0f, float dragSpeed = 0.1f, float columnWidth = 70.0f);

    void DrawColorEdit4(const char* label, glm::vec4& color, float columnWidth = 50.0f);
}