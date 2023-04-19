#include "InspectorPanel.h"

#include <imgui.h>

#include <ECS.h>
#include "MenuBar.h"

namespace Hudi {

	void InspectorPanel::OnImGuiRender()
	{
		bool& inspector = MenuBar::Get().opt_Inspector;

		if (inspector)
		{
			/*ImGui::Begin("Inspector", &inspector);
			Ref<GameObject> obj = MenuBar::Get().GetSelectedObject();
			if (obj)
			{
				std::vector<Ref<Component>> components = obj->GetComponents();
				for (auto& comp : components)
				{
					if (comp->Is<Transform>())
					{
						OnDrawTransform(std::static_pointer_cast<Transform>(comp));
					}
					else if (comp->Is<SpriteRenderer>())
					{
						OnDrawSpriteRenderer(std::static_pointer_cast<SpriteRenderer>(comp));
					}
				}
			}
			ImGui::End();*/
		}
		ImGui::ShowDemoWindow();
	}

	void InspectorPanel::OnDrawComponents(Ref<GameObject> obj)
	{
	}

    void InspectorPanel::OnDrawTransform(Ref<Transform> transform)
    {
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
        if (ImGui::CollapsingHeader("Transform", flags))
        {
            auto& [px, py, pz] = transform->position;
            float pos[3] = { px, py, pz };
            ImGuiInputTextFlags pos_flags = ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_EnterReturnsTrue;// | ImGuiInputTextFlags_AlwaysOverwrite;
            ImGui::InputScalarN("Position", ImGuiDataType_Float, pos, 3, NULL, NULL, "%.0f", pos_flags);
            { px = pos[0], py = pos[1], pz = pos[2]; }

            
            auto& [rx, ry, rz] = transform->rotation;
            float rot[3] = { rx, ry, rz };
            ImGuiInputTextFlags rot_flags = ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_EnterReturnsTrue;
            ImGui::InputScalarN("Rotation", ImGuiDataType_Float, rot, 3, NULL, NULL, "%.0f", rot_flags);
            { rx = rot[0], ry = rot[1], rz = rot[2]; }

            auto& [sx, sy, sz] = transform->scale;
            float scl[3] = { sx, sy, sz };
            ImGuiInputTextFlags scl_flags = ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_EnterReturnsTrue;
            ImGui::InputScalarN("Scale", ImGuiDataType_Float, scl, 3, NULL, NULL, "%.2f", scl_flags);
            { sx = scl[0], sy = scl[1], sz = scl[2]; }
        }
	}

	void InspectorPanel::OnDrawSpriteRenderer(Ref<SpriteRenderer> sprite)
	{
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
        if (ImGui::CollapsingHeader("Sprite Renderer", flags))
        {
            ImGuiInputTextFlags order_flags = ImGuiInputTextFlags_CharsDecimal | ImGuiInputTextFlags_EnterReturnsTrue;
            ImGui::InputScalar("Order", ImGuiDataType_U16, &sprite->order, NULL, NULL, "%d", order_flags);

        }

        //if (ImGui::CollapsingHeader("Configuration"))
        //{
        //    ImGuiIO& io = ImGui::GetIO();

        //    if (ImGui::TreeNode("Configuration##2"))
        //    {
        //        ImGui::SeparatorText("General");
        //        ImGui::CheckboxFlags("io.ConfigFlags: NavEnableKeyboard", &io.ConfigFlags, ImGuiConfigFlags_NavEnableKeyboard);
        //        ImGui::SameLine(); HelpMarker("Enable keyboard controls.");
        //        ImGui::CheckboxFlags("io.ConfigFlags: NavEnableGamepad", &io.ConfigFlags, ImGuiConfigFlags_NavEnableGamepad);
        //        ImGui::SameLine(); HelpMarker("Enable gamepad controls. Require backend to set io.BackendFlags |= ImGuiBackendFlags_HasGamepad.\n\nRead instructions in imgui.cpp for details.");
        //        ImGui::CheckboxFlags("io.ConfigFlags: NavEnableSetMousePos", &io.ConfigFlags, ImGuiConfigFlags_NavEnableSetMousePos);
        //        ImGui::SameLine(); HelpMarker("Instruct navigation to move the mouse cursor. See comment for ImGuiConfigFlags_NavEnableSetMousePos.");
        //        ImGui::CheckboxFlags("io.ConfigFlags: NoMouse", &io.ConfigFlags, ImGuiConfigFlags_NoMouse);
        //        if (io.ConfigFlags & ImGuiConfigFlags_NoMouse)
        //        {
        //            // The "NoMouse" option can get us stuck with a disabled mouse! Let's provide an alternative way to fix it:
        //            if (fmodf((float)ImGui::GetTime(), 0.40f) < 0.20f)
        //            {
        //                ImGui::SameLine();
        //                ImGui::Text("<<PRESS SPACE TO DISABLE>>");
        //            }
        //            if (ImGui::IsKeyPressed(ImGuiKey_Space))
        //                io.ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
        //        }
        //        ImGui::CheckboxFlags("io.ConfigFlags: NoMouseCursorChange", &io.ConfigFlags, ImGuiConfigFlags_NoMouseCursorChange);
        //        ImGui::SameLine(); HelpMarker("Instruct backend to not alter mouse cursor shape and visibility.");

        //        ImGui::CheckboxFlags("io.ConfigFlags: DockingEnable", &io.ConfigFlags, ImGuiConfigFlags_DockingEnable);
        //        ImGui::SameLine();
        //        if (io.ConfigDockingWithShift)
        //            HelpMarker("Drag from window title bar or their tab to dock/undock. Hold SHIFT to enable docking.\n\nDrag from window menu button (upper-left button) to undock an entire node (all windows).");
        //        else
        //            HelpMarker("Drag from window title bar or their tab to dock/undock. Hold SHIFT to disable docking.\n\nDrag from window menu button (upper-left button) to undock an entire node (all windows).");
        //        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        //        {
        //            ImGui::Indent();
        //            ImGui::Checkbox("io.ConfigDockingNoSplit", &io.ConfigDockingNoSplit);
        //            ImGui::SameLine(); HelpMarker("Simplified docking mode: disable window splitting, so docking is limited to merging multiple windows together into tab-bars.");
        //            ImGui::Checkbox("io.ConfigDockingWithShift", &io.ConfigDockingWithShift);
        //            ImGui::SameLine(); HelpMarker("Enable docking when holding Shift only (allow to drop in wider space, reduce visual noise)");
        //            ImGui::Checkbox("io.ConfigDockingAlwaysTabBar", &io.ConfigDockingAlwaysTabBar);
        //            ImGui::SameLine(); HelpMarker("Create a docking node and tab-bar on single floating windows.");
        //            ImGui::Checkbox("io.ConfigDockingTransparentPayload", &io.ConfigDockingTransparentPayload);
        //            ImGui::SameLine(); HelpMarker("Make window or viewport transparent when docking and only display docking boxes on the target viewport. Useful if rendering of multiple viewport cannot be synced. Best used with ConfigViewportsNoAutoMerge.");
        //            ImGui::Unindent();
        //        }

        //        ImGui::CheckboxFlags("io.ConfigFlags: ViewportsEnable", &io.ConfigFlags, ImGuiConfigFlags_ViewportsEnable);
        //        ImGui::SameLine(); HelpMarker("[beta] Enable beta multi-viewports support. See ImGuiPlatformIO for details.");
        //        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        //        {
        //            ImGui::Indent();
        //            ImGui::Checkbox("io.ConfigViewportsNoAutoMerge", &io.ConfigViewportsNoAutoMerge);
        //            ImGui::SameLine(); HelpMarker("Set to make all floating imgui windows always create their own viewport. Otherwise, they are merged into the main host viewports when overlapping it.");
        //            ImGui::Checkbox("io.ConfigViewportsNoTaskBarIcon", &io.ConfigViewportsNoTaskBarIcon);
        //            ImGui::SameLine(); HelpMarker("Toggling this at runtime is normally unsupported (most platform backends won't refresh the task bar icon state right away).");
        //            ImGui::Checkbox("io.ConfigViewportsNoDecoration", &io.ConfigViewportsNoDecoration);
        //            ImGui::SameLine(); HelpMarker("Toggling this at runtime is normally unsupported (most platform backends won't refresh the decoration right away).");
        //            ImGui::Checkbox("io.ConfigViewportsNoDefaultParent", &io.ConfigViewportsNoDefaultParent);
        //            ImGui::SameLine(); HelpMarker("Toggling this at runtime is normally unsupported (most platform backends won't refresh the parenting right away).");
        //            ImGui::Unindent();
        //        }

        //        ImGui::Checkbox("io.ConfigInputTrickleEventQueue", &io.ConfigInputTrickleEventQueue);
        //        ImGui::SameLine(); HelpMarker("Enable input queue trickling: some types of events submitted during the same frame (e.g. button down + up) will be spread over multiple frames, improving interactions with low framerates.");
        //        ImGui::Checkbox("io.MouseDrawCursor", &io.MouseDrawCursor);
        //        ImGui::SameLine(); HelpMarker("Instruct Dear ImGui to render a mouse cursor itself. Note that a mouse cursor rendered via your application GPU rendering path will feel more laggy than hardware cursor, but will be more in sync with your other visuals.\n\nSome desktop applications may use both kinds of cursors (e.g. enable software cursor only when resizing/dragging something).");

        //        ImGui::SeparatorText("Widgets");
        //        ImGui::Checkbox("io.ConfigInputTextCursorBlink", &io.ConfigInputTextCursorBlink);
        //        ImGui::SameLine(); HelpMarker("Enable blinking cursor (optional as some users consider it to be distracting).");
        //        ImGui::Checkbox("io.ConfigInputTextEnterKeepActive", &io.ConfigInputTextEnterKeepActive);
        //        ImGui::SameLine(); HelpMarker("Pressing Enter will keep item active and select contents (single-line only).");
        //        ImGui::Checkbox("io.ConfigDragClickToInputText", &io.ConfigDragClickToInputText);
        //        ImGui::SameLine(); HelpMarker("Enable turning DragXXX widgets into text input with a simple mouse click-release (without moving).");
        //        ImGui::Checkbox("io.ConfigWindowsResizeFromEdges", &io.ConfigWindowsResizeFromEdges);
        //        ImGui::SameLine(); HelpMarker("Enable resizing of windows from their edges and from the lower-left corner.\nThis requires (io.BackendFlags & ImGuiBackendFlags_HasMouseCursors) because it needs mouse cursor feedback.");
        //        ImGui::Checkbox("io.ConfigWindowsMoveFromTitleBarOnly", &io.ConfigWindowsMoveFromTitleBarOnly);
        //        ImGui::Checkbox("io.ConfigMacOSXBehaviors", &io.ConfigMacOSXBehaviors);
        //        ImGui::Text("Also see Style->Rendering for rendering options.");
        //        ImGui::TreePop();
        //        ImGui::Spacing();
        //    }
        //}
	}

}