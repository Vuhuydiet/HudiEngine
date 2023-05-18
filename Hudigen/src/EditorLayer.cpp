#include "EditorLayer.h"

#include <imgui.h>

namespace Hudi {

	EditorLayer::EditorLayer()
	{
#if 1
		Ref<Scene> scene = NewRef<Scene>(0);
		m_HierarchyPanels.SetContext(scene);

		GameObject mainCamera = scene->CreateEmptyObject("Main Camera");
		mainCamera.AddComponent<Transform>();
		Camera& cameraComponent = mainCamera.AddComponent<Camera>();
		scene->SetPrimaryCamera(mainCamera);

		GameObject vudeptrai = scene->CreateGameObject("vu dep trai");
		//auto trai = scene->CreateGameObject("trai");

		//vudeptrai->AddChild(trai);
		//trai->SetParent(vudeptrai);
#endif
	}

	void EditorLayer::OnAttach()
	{
	}

	void EditorLayer::OnDetach()
	{
	}

	void EditorLayer::OnUpdate(float dt)
	{
		// Poll Commands
		PanelCommand command;
		while (m_HierarchyPanels.PollCommand(command))
		{
			switch (command.type)
			{
				case PanelCommand::OpenScene:
				{
					std::filesystem::path path = (const char*)command.data;
					OpenScene(path);
					break;
				}
				case PanelCommand::None:
				{
					HD_WARN("Undefined command being push to the PanelCommand");
					break;
				}
			}
		}

		// Update Panels
		m_HierarchyPanels.OnUpdate(dt);
	}

	void EditorLayer::OnImGuiRender()
	{
		BeginDockspace();

		OnImGuiRenderMenuBar();
		m_HierarchyPanels.OnImGuiRender(m_OpenedHierarchy, m_OpenedInspector, m_OpenedViewport);
		m_ContentBrowserPanel.OnImGuiRender();

		EndDockspace();
	}

	void EditorLayer::OnEvent(Event& event)
	{
		m_HierarchyPanels.OnEvent(event);

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch(KEY_DOWN, HD_BIND_EVENT_FN(EditorLayer::OnKeyPressedEvent));
		
		if (event.type() == QUIT)
		{
			CloseApp();
		}
	}

	void EditorLayer::OnKeyPressedEvent(Event& event)
	{
		bool ctrl = Input::IsKeyDown(Key::L_CTRL) || Input::IsKeyDown(Key::R_CTRL);
		bool shift = Input::IsKeyDown(Key::L_SHIFT) || Input::IsKeyDown(Key::R_SHIFT);
		switch (event.GetKeyCode())
		{
		case Key::N:
			if (ctrl)
			{
				NewScene();
				event.handled = true;
			}
			break;
		case Key::O:
			if (ctrl)
			{
				OpenScene();
				event.handled = true;
			}
			break;
		case Key::S:
			if (ctrl && shift)
			{
				SaveSceneAs();
				event.handled = true;
			}
			else if (ctrl)
			{
				SaveScene();
				event.handled = true;
			}
			break;
		case Key::P:
			if (ctrl)
			{
				CloseScene();
				event.handled = true;
			}
			break;
		}
	}

	void EditorLayer::BeginDockspace()
	{
#if 0
		const ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->WorkPos);
		ImGui::SetNextWindowSize(viewport->WorkSize);
		ImGui::SetNextWindowViewport(viewport->ID);

		ImGuiDockNodeFlags_ dockspace_flags = ImGuiDockNodeFlags_None; // ImGuiDockNodeFlags_PassthruCentralNode;
		ImGuiWindowFlags host_window_flags = ImGuiWindowFlags_None;

		host_window_flags |= 
		ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove; 
		//ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking;
		host_window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			host_window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		// Begin docking window
		ImGui::Begin("Dockspace", nullptr, host_window_flags);
		ImGui::PopStyleVar(3);

		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();
		float minWindowX = style.WindowMinSize.x;
		style.WindowMinSize.x = 320.0f;

		ImGuiID dockspace_id = ImGui::GetID("DockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

		style.WindowMinSize.x = minWindowX;
#else
		// Note: Switch this to true to enable dockspace
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
			//ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDocking |
			ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// DockSpace
		ImGuiStyle& style = ImGui::GetStyle();
		float minWinSizeX = style.WindowMinSize.x;
		style.WindowMinSize.x = 300.0f;
		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;
#endif
	}

	void EditorLayer::EndDockspace()
	{
		ImGui::End();
	}

	void EditorLayer::OnImGuiRenderMenuBar()
	{
		bool isEditing = m_HierarchyPanels.GetViewportState() == ViewportState::Edit;
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				//ShowExampleMenuFile();
				if (ImGui::MenuItem("New", "Ctrl+N", nullptr, isEditing))
					NewScene();
				if (ImGui::MenuItem("Open...", "Ctrl+O", nullptr, isEditing))
					OpenScene();
				if (ImGui::MenuItem("Save", "Ctrl+S", nullptr, isEditing))
					SaveScene();
				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S", nullptr, isEditing))
					SaveSceneAs();
				if (ImGui::MenuItem("Close Scene", "Ctrl+P", nullptr, isEditing))
					CloseScene();
				ImGui::Separator();
				if (ImGui::MenuItem("Close"))
					CloseApp();

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Options"))
			{
				ImGui::MenuItem("Hierarchy", NULL, &m_OpenedHierarchy, true);
				ImGui::MenuItem("Inspector", NULL, &m_OpenedInspector, true);
				ImGui::MenuItem("Viewport", NULL, &m_OpenedViewport, true);

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

	}

	void EditorLayer::NewScene()
	{
		if (m_HierarchyPanels.GetViewportState() != ViewportState::Edit)
			return;

		m_HierarchyPanels.SetContext(NewRef<Scene>(0));
		m_CurrentScenePath.clear();
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		if (m_HierarchyPanels.GetViewportState() != ViewportState::Edit)
			return;
		if (path.extension() != ".hud")
		{
			HD_ERROR("Can only open .hud Hudi Scene files.");
			return;
		}

		if (!path.empty())
		{
			m_CurrentScenePath = path;
			Ref<Scene> newScene = NewRef<Scene>(0);
			SceneSerializer serializer(newScene);
			serializer.Open(path);
			m_HierarchyPanels.SetContext(newScene);
		}
	}

	void EditorLayer::OpenScene()
	{
		if (m_HierarchyPanels.GetViewportState() != ViewportState::Edit)
			return;

		std::string filepath = FileDialogs::OpenFile("Scene Files (*.hud)\0*.hud\0");
		OpenScene(filepath);
	}

	void EditorLayer::SaveScene()
	{
		if (m_HierarchyPanels.GetViewportState() != ViewportState::Edit)
			return;

		Ref<Scene> scene = m_HierarchyPanels.GetContext();
		if (!scene)
			return;

		if (m_CurrentScenePath.empty())
		{
			m_CurrentScenePath = FileDialogs::SaveFile("Scene Files (*.hud)\0*.hud\0");
		}
		if (!m_CurrentScenePath.empty())
		{
			SceneSerializer serializer(scene);
			serializer.Save(m_CurrentScenePath);
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		if (m_HierarchyPanels.GetViewportState() != ViewportState::Edit)
			return;

		Ref<Scene> scene = m_HierarchyPanels.GetContext();
		if (!scene)
			return;

		std::string filepath = FileDialogs::SaveFile("Scene Files (*.hud)\0*.hud\0");
		if (!filepath.empty())
		{
			m_CurrentScenePath = filepath;
			SceneSerializer serializer(scene);
			serializer.Save(filepath);
		}
	}

	void EditorLayer::CloseScene()
	{
		if (m_HierarchyPanels.GetViewportState() != ViewportState::Edit)
			return;

		m_HierarchyPanels.SetContext(nullptr);
	}

	void EditorLayer::CloseApp()
	{
		if (m_HierarchyPanels.GetViewportState() == ViewportState::Runtime)
		{
			m_HierarchyPanels.SetViewportEdit();
		}
		Application::Get().Close();
	}

}

static void ShowExampleMenuFile()
{
	//IMGUI_DEMO_MARKER("Examples/Menu");
	ImGui::MenuItem("(demo menu)", NULL, false, false);
	if (ImGui::MenuItem("New")) {}
	if (ImGui::MenuItem("Open", "Ctrl+O")) {}
	if (ImGui::BeginMenu("Open Recent"))
	{
		ImGui::MenuItem("fish_hat.c");
		ImGui::MenuItem("fish_hat.inl");
		ImGui::MenuItem("fish_hat.h");
		if (ImGui::BeginMenu("More.."))
		{
			ImGui::MenuItem("Hello");
			ImGui::MenuItem("Sailor");
			if (ImGui::BeginMenu("Recurse.."))
			{
				ShowExampleMenuFile();
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		ImGui::EndMenu();
	}
	if (ImGui::MenuItem("Save", "Ctrl+S")) {}
	if (ImGui::MenuItem("Save As..")) {}

	ImGui::Separator();
	//IMGUI_DEMO_MARKER("Examples/Menu/Options");
	if (ImGui::BeginMenu("Options"))
	{
		static bool enabled = true;
		ImGui::MenuItem("Enabled", "", &enabled);
		ImGui::BeginChild("child", ImVec2(0, 60), true);
		for (int i = 0; i < 10; i++)
			ImGui::Text("Scrolling Text %d", i);
		ImGui::EndChild();
		static float f = 0.5f;
		static int n = 0;
		ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
		ImGui::InputFloat("Input", &f, 0.1f);
		ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
		ImGui::EndMenu();
	}

	//IMGUI_DEMO_MARKER("Examples/Menu/Colors");
	if (ImGui::BeginMenu("Colors"))
	{
		float sz = ImGui::GetTextLineHeight();
		for (int i = 0; i < ImGuiCol_COUNT; i++)
		{
			const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
			ImVec2 p = ImGui::GetCursorScreenPos();
			ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol)i));
			ImGui::Dummy(ImVec2(sz, sz));
			ImGui::SameLine();
			ImGui::MenuItem(name);
		}
		ImGui::EndMenu();
	}

	// Here we demonstrate appending again to the "Options" menu (which we already created above)
	// Of course in this demo it is a little bit silly that this function calls BeginMenu("Options") twice.
	// In a real code-base using it would make senses to use this feature from very different code locations.
	if (ImGui::BeginMenu("Options")) // <-- Append!
	{
		//IMGUI_DEMO_MARKER("Examples/Menu/Append to an existing menu");
		static bool b = true;
		ImGui::Checkbox("SomeOption", &b);
		ImGui::EndMenu();
	}

	if (ImGui::BeginMenu("Disabled", false)) // Disabled
	{
		IM_ASSERT(0);
	}
	if (ImGui::MenuItem("Checked", NULL, true)) {}
}
