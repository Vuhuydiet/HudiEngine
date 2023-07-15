#include "HierarchyPanels.h"

#include "ImGuiExtension/ImGuiFunctions.h"

#include <ECS.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_internal.h>

namespace Hudi {

	static bool s_IsFocused = false;
	static bool s_IsHovered = false;

	bool HierarchyPanels::IsInspectorFocused() const { return s_IsFocused; }
	bool HierarchyPanels::IsInspectorHovered() const { return s_IsHovered; }

	static void OnDrawComponents(Ref<Scene> context, GameObject object);
	static void OnDrawAddComponent(Ref<Scene> context, GameObject object);
	void HierarchyPanels::OnImGuiRenderInspectorPanel(bool& open)
	{
		if (!open)
			return;

		ImGui::Begin("Inspector", &open);

		s_IsFocused = ImGui::IsWindowFocused();
		s_IsHovered = ImGui::IsWindowHovered();

		if (m_Context && m_SelectedObject.IsValid())
		{
			OnDrawComponents(m_Context, m_SelectedObject);
			OnDrawAddComponent(m_Context, m_SelectedObject);
		}
		ImGui::End();
	}

	template <typename T, typename Fn>
	static void OnDrawComponent(const char* name, GameObject object, bool removable, Fn&& func);
	static void OnDrawScriptComponent(Ref<ScriptEngine> engine, GameObject object, const std::string& script);
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

			DrawColorEdit4("Color", sprite.color);

			DrawInputScalar("Order", ImGuiDataType_U32, &sprite.order, nullptr, nullptr, "%d", ImGuiInputTextFlags_CharsDecimal);

			ImGui::Text("Image path"); ImGui::SameLine();
			std::string filename = sprite.filepath.string();
			if (filename.empty())
				filename = "null";
			float windowWidth = ImGui::GetContentRegionAvail().x;
			ImGui::Button(filename.c_str(), { windowWidth, 0 });
			if (ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left) && ImGui::IsItemHovered())
			{
				std::string path = FileDialogs::OpenFile("Image (.png)\0*.png\0");
				if (!path.empty())
					sprite.SetFilePath(path);
			}
			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("CONTENT_BROWSER_PNG"))
				{
					const char* path = (const char*)payload->Data;
					sprite.SetFilePath(path);
				}
				ImGui::EndDragDropTarget();
			}
			});

		OnDrawComponent<Camera>("Camera", object, true, [&context, object](Camera& camera) {
			bool isChosen = context->GetPrimaryCamera() == object;
			if (ImGui::Checkbox("Primary", &isChosen))
			{
				/*if (isChosen)
					context->SetPrimaryCamera(object);
				else
					context->ResetPrimaryCamera();*/
				isChosen ? context->SetPrimaryCamera(object) : context->ResetPrimaryCamera();
			}

			Camera::Type currentType = camera.GetType();
			ImGui::Text("Projection:"); ImGui::SameLine();
			if (ImGui::BeginCombo("##Projection", currentType == Camera::Perspective ? "Perspective" : "Orthographic"))
			{
				bool isPerspective = false, isOrthographic = false;
				ImGui::Selectable("Perspective", &isPerspective);
				ImGui::Selectable("Orthographic", &isOrthographic);
				currentType = isPerspective ? Camera::Perspective : (isOrthographic ? Camera::Orthographic : currentType);
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

		OnDrawComponent<Rigidbody2D>("Rigidbody 2D", object, true, [](Rigidbody2D& rb2) {
			std::map<Rigidbody2D::BodyType, const char*> types = {
				{ Rigidbody2D::BodyType::Static, "Static" },
				{ Rigidbody2D::BodyType::Dynamic, "Dynamic" },
				{ Rigidbody2D::BodyType::Kinematic, "Kinematic" }
			};
			Rigidbody2D::BodyType currentType = rb2.type;
			ImGui::Text("Body Type:"); ImGui::SameLine();
			if (ImGui::BeginCombo("##Body type", types.at(currentType)))
			{
				bool isStatic = false, isDynamic = false, isKinematic = false;
				ImGui::Selectable("Static", &isStatic);
				ImGui::Selectable("Dynamic", &isDynamic);
				ImGui::Selectable("Kinematic", &isKinematic);
				rb2.type = isStatic ? Rigidbody2D::BodyType::Static : (isDynamic ? Rigidbody2D::BodyType::Dynamic : (isKinematic ? Rigidbody2D::BodyType::Kinematic : currentType));
				
				ImGui::EndCombo();
			}
			if (currentType != Rigidbody2D::BodyType::Static)
				ImGui::Checkbox("Fixed Rotation", &rb2.fixedRotation);
		});

		OnDrawComponent<BoxCollider2D>("Box Collider 2D", object, true, [](BoxCollider2D& box2) {
			DrawFloat2Control("Offset", box2.offset);
			ImGui::Separator();
			DrawFloat2Control("Size", box2.size, 0.5f, 0.01f);
			ImGui::Separator();
			DrawFloatControl("Density", box2.density, 0.01f);
			ImGui::Separator();
			DrawFloatControl("Friction", box2.friction, 0.01f);
			ImGui::Separator();
			DrawFloatControl("Restitution", box2.restitution, 0.01f);
			ImGui::Separator();
			DrawFloatControl("Restitution Threshold", box2.restitutionThreshold, 0.01f);
		});

		// Behaviours components
		auto engine = context->GetScriptEngine();
		auto behaviours = engine->GetBehaviours(object.GetEntityID());
		for (auto& [scriptName, script] : behaviours)
		{
			OnDrawScriptComponent(engine, object, scriptName);
		}

		ImGui::Separator();
	}

	template <typename T, typename Fn>
	static void OnDrawComponent(const char* name, GameObject object, bool removable, Fn&& func)
	{
		if (!object.HasComponent<T>())
			return;

		ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4,4 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::Separator();

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		bool open = ImGui::TreeNodeEx(name, flags);
		ImGui::PopStyleVar();

		ImGui::SameLine(contentRegionAvail.x - lineHeight * 0.5f);
		if (ImGui::Button("...", ImVec2{ lineHeight, lineHeight }))
		{
			ImGui::OpenPopup("ComponentSettings");
		}

		bool isRemoved = false;
		if (ImGui::BeginPopup("ComponentSettings"))
		{
			isRemoved = removable && ImGui::MenuItem("Remove Component");
			ImGui::EndPopup();
		}

		if (open)
		{
			T& component = object.GetComponent<T>();
			std::forward<Fn>(func)(component);
			ImGui::TreePop();
		}

		if (isRemoved)
			object.RemoveComponent<T>();
	}
	
	static void OnDrawScriptComponent(Ref<ScriptEngine> engine, GameObject object, const std::string& script)
	{
		ECS::Entity entity = object.GetEntityID();

		ImVec2 contentRegionAvail = ImGui::GetContentRegionAvail();
		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4,4 });
		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImGui::Separator();

		ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
		bool open = ImGui::TreeNodeEx(script.c_str(), flags, "Script Component (%s)", script.c_str());
		ImGui::PopStyleVar();

		ImGui::SameLine(contentRegionAvail.x - lineHeight * 0.5f);
		if (ImGui::Button("...", ImVec2{ lineHeight, lineHeight }))
		{
			ImGui::OpenPopup("ComponentSettings");
		}

		bool isRemoved = false;
		if (ImGui::BeginPopup("ComponentSettings"))
		{
			if (ImGui::MenuItem("Remove Component"))
				isRemoved = true;
			ImGui::EndPopup();
		}

		if (open)
			ImGui::TreePop();

		if (isRemoved)
			engine->RemoveBehaviourComponent(object, script);

	}

	static void OnDrawAddComponent(Ref<Scene> context, GameObject object)
	{
		float buttonWidth = 100.0f;
		float windowWidth = ImGui::GetWindowContentRegionMax().x;
		ImGui::SetCursorPosX(windowWidth * 0.5f - buttonWidth * 0.5f);
		if (ImGui::Button("Add Component", { buttonWidth, 0.0f }))
			ImGui::OpenPopup("AddComponent");

		if (ImGui::BeginPopup("AddComponent"))
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
			if (!object.HasComponent<Rigidbody2D>() && ImGui::MenuItem("Rigidbody 2D"))
			{
				object.AddComponent<Rigidbody2D>();
				ImGui::CloseCurrentPopup();
			}
			if (!object.HasComponent<BoxCollider2D>() && ImGui::MenuItem("Box Collider 2D"))
			{
				object.AddComponent<BoxCollider2D>();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		// Add ScriptComponent
		auto engine = context->GetScriptEngine();
		if (ImGui::BeginDragDropTarget())
		{
			if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("BEHAVIOUR_LIBRARY_PANEL"))
			{
				std::string scriptName = (const char*)payload->Data;
				engine->AddBehaviourComponent(object, scriptName);
			}
			ImGui::EndDragDropTarget();
		}
	}

}