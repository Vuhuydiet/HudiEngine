#include "InspectorPanel.h"

#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_internal.h>

#include <ECS.h>
#include "MenuBar.h"

namespace Hudi {

	void InspectorPanel::OnImGuiRender(bool& open, GameObject object)
	{
		if (open)
		{
			ImGui::Begin("Inspector", &open);
            if (object.Valid())
            {
                OnDrawComponents(object);
                OnDrawAddComponent(object);
            }
			ImGui::End();
		}
	}

    static void DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float dragSpeed = 0.1f, float columnWidth = 100.0f)
    {
        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        if (ImGui::Button("X", buttonSize))
            values.x = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &values.x, dragSpeed, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        if (ImGui::Button("Y", buttonSize))
            values.y = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, dragSpeed, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        if (ImGui::Button("Z", buttonSize))
            values.z = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Z", &values.z, dragSpeed, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();
        ImGui::Columns(1);
        ImGui::PopID();
    }

    static void DrawFloat2Control(const std::string& label, glm::vec2& values, float resetValue = 0.0f, float dragSpeed = 0.1f, float columnWidth = 100.0f)
    {
        ImGui::PushID(label.c_str());

        ImGui::Columns(2);
        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text(label.c_str());
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        if (ImGui::Button("X", buttonSize))
            values.x = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &values.x, dragSpeed, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        if (ImGui::Button("Y", buttonSize))
            values.y = resetValue;
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, dragSpeed, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();

        ImGui::PopStyleVar();
        ImGui::Columns(1);
        ImGui::PopID();
    }

    template <typename T, typename Fn>
    static void OnDrawComponent(const char* name, GameObject object, bool removable, Fn&& func)
    {
        if (!object.HasComponent<T>())
            return;

        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;

        ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4,4 });
        float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImGui::Separator();

        bool open = ImGui::TreeNodeEx(name, flags);
        ImGui::PopStyleVar();

        ImGui::SameLine(contentRegionAvail.x - lineHeight * 0.5f);
        if (ImGui::Button("...", ImVec2{ lineHeight, lineHeight}))
        {
            ImGui::OpenPopup("ComponentSettings");
        }

        bool isRemove = false;
        if (ImGui::BeginPopup("ComponentSettings"))
        {
            isRemove = removable && ImGui::MenuItem("Remove Component");
            ImGui::EndPopup();
        }

        if (open)
        {
            T& component = object.GetComponent<T>();
            std::forward<Fn>(func)(component);
            ImGui::TreePop();
        }

        if (isRemove)
        {
            object.RemoveComponent<T>();
        }
    }

	void InspectorPanel::OnDrawComponents(GameObject object)
	{
        OnDrawComponent<Transform>("Transform", object, false, [](Transform& transform) {
            DrawVec3Control("Position", (glm::vec3&)transform.position, 0.0f);
            ImGui::Separator();
            DrawVec3Control("Rotation", (glm::vec3&)transform.rotation, 0.0f, 0.5f);
            ImGui::Separator();
            DrawVec3Control("Scale", (glm::vec3&)transform.scale, 1.0f);
        });

        OnDrawComponent<SpriteRenderer>("Sprite Renderer", object, true, [](SpriteRenderer& sprite) {
            //ImGui::DragFloat2("Size", glm::value_ptr(sprite->size), 0.1f);
            DrawFloat2Control("Size", sprite.size, 1.0f, 0.1f);
            ImGui::Separator();
            ImGui::ColorEdit4("Color", glm::value_ptr(sprite.color));
            ImGui::InputScalar("Order", ImGuiDataType_U16, &sprite.order, NULL, NULL, "%d", ImGuiInputTextFlags_CharsDecimal);

            char filepath[100];
            strcpy_s(filepath, sprite.filepath.c_str());
            if (ImGui::InputText("Image file path", filepath, 100, ImGuiInputTextFlags_EnterReturnsTrue))
                sprite.SetFilePath(filepath);
        });

        OnDrawComponent<Camera>("Camera", object, true, [](Camera& camera) {
            Camera::Type currentType = camera.GetType();
            if (ImGui::BeginCombo("Type", currentType == Camera::Perspective ? "Perspective" : "Orthographic"))
            {
                bool isPerspective = false, isOrthographic = false;
                ImGui::Selectable("Perspective", &isPerspective);
                ImGui::Selectable("Orthographic", &isOrthographic);
                currentType = isPerspective ? Camera::Perspective : (isOrthographic ? Camera::Orthographic : currentType);
                //RenderCommand::EnableDepthTest(currentType == Camera::Perspective);
                camera.Switch(currentType);
                ImGui::EndCombo();
            }

            if (currentType == Camera::Perspective)
            {
                Camera::PerspectiveData perData = camera.GetPerspectiveData();
                bool fov = ImGui::DragFloat("FOV", &perData.FOV, 0.5f, 0.1f, 189.9f);
                bool nc = ImGui::DragFloat("Near clip", &perData.zNear, 0.1f, 0.1f, 1000.0f);
                bool fc = ImGui::DragFloat("Far clip", &perData.zFar, 1.0f, 0.1f, 1000.0f);
                if (fov || nc || fc)
                    camera.SetProjectionData(perData);
            }
            else if (currentType == Camera::Orthographic)
            {
                Camera::OrthographicData orthoData = camera.GetOrthographicData();
                bool nc = ImGui::DragFloat("Near clip", &orthoData.zNear, 0.1f);
                bool fc = ImGui::DragFloat("Far clip", &orthoData.zFar, 0.1f);
                if (nc || fc)
                    camera.SetProjectionData(orthoData);
            }
        });
	}

    void InspectorPanel::OnDrawAddComponent(GameObject object)
    {
        if (ImGui::Button("Add Component"))
            ImGui::OpenPopup("Add Component");
        if (ImGui::BeginPopup("Add Component"))
        {
            if (!object.HasComponent<Transform>() && ImGui::MenuItem("Transform"))
            {
                object.AddComponent<Transform>();
                ImGui::CloseCurrentPopup();
            }
            if (!object.HasComponent<SpriteRenderer>() && ImGui::MenuItem("Sprite Renderer"))
            {
                object.AddComponent<SpriteRenderer>();
                ImGui::CloseCurrentPopup();
            }
            if (!object.HasComponent<Camera>() && ImGui::MenuItem("Camera"))
            {
                object.AddComponent<Camera>();
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }

}