#include "HierarchyPanels.h"

#include "ImGuiExtension/ImGuiFunctions.h"

#include <imgui.h>
#include <ECS.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_internal.h>

namespace Hudi {

    static void OnDrawComponents(Ref<Scene> context, GameObject object);
    static void OnDrawAddComponent(GameObject object);
	void HierarchyPanels::OnImGuiRenderInspectorPanel(bool& open)
	{
        if (!open)
            return;

		ImGui::Begin("Inspector", &open);
        if (m_SelectedObject.Valid())
        {
            OnDrawComponents(m_Context, m_SelectedObject);
            OnDrawAddComponent(m_SelectedObject);
        }
		ImGui::End();
	}
    
    template <typename T, typename Fn>
    static void OnDrawComponent(const char* name, GameObject object, bool removable, Fn&& func);
	static void OnDrawComponents(Ref<Scene> context, GameObject object)
	{
        OnDrawComponent<Transform>("Transform", object, false, [](Transform& transform) {
            DrawFloat3Control("Position", (glm::vec3&)transform.position, 0.0f);
            ImGui::Separator();
            DrawFloat3Control("Rotation", (glm::vec3&)transform.rotation, 0.0f, 0.5f);
            ImGui::Separator();
            DrawFloat3Control("Scale", (glm::vec3&)transform.scale, 1.0f);
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
            ImGui::SameLine();
            if (ImGui::Button("+", ImVec2{ 20, 20 }))
            {
                HD_INFO("button");
                std::string path = FileDialogs::OpenFile("Image (.png)\0*.png\0)");
                if (!path.empty())
                    sprite.SetFilePath(path);
            }
        });

        OnDrawComponent<Camera>("Camera", object, true, [&context, object](Camera& camera) {
            bool isChosen = context->GetPrimaryCamera() == object;
            if (ImGui::Checkbox("Primary", &isChosen))
            {
                if (isChosen)
                    context->SetPrimaryCamera(object);
                else
                    context->ResetPrimaryCamera();
            }

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

    static void OnDrawAddComponent(GameObject object)
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
        if (ImGui::Button("...", ImVec2{ lineHeight, lineHeight }))
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

}