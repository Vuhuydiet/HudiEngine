#include "HierarchyPanels.h"

namespace Hudi {

	PanelCommand::PanelCommand(Type _type, const void* _data, size_t _dataSize)
		: type(_type), dataSize(_dataSize)
	{
		data = malloc(_dataSize);
		if (data)
			memcpy(data, _data, _dataSize);
	}

	PanelCommand::PanelCommand(const PanelCommand& other)
	{
		if (!other.data)
			return;

		data = malloc(other.dataSize);
		if (data)
		{
			type = other.type;
			dataSize = other.dataSize;
			memcpy(data, other.data, other.dataSize);
		}
	}

	PanelCommand::~PanelCommand()
	{
		free(data);
		data = nullptr;
		dataSize = 0;
	}

	PanelCommand& PanelCommand::operator= (const PanelCommand& other)
	{
		if (dataSize != 0)
		{
			free(data);
			data = nullptr;
			dataSize = 0;
		}

		if (!other.data)
			return *this;

		data = malloc(other.dataSize);
		if (data)
		{
			type = other.type;
			dataSize = other.dataSize;
			memcpy(data, other.data, other.dataSize);
		}
		return *this;
	}

	////////////////////////////////////////////////////////////////////////////
	/// HierarchyPanels ////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////

	static const std::string s_ToolbarIconPath = "resources\\images\\icons\\tool_bar\\";
	
	HierarchyPanels::HierarchyPanels()
	{
		FramebufferSpecification specs;
		specs.width = 800;
		specs.height = 600;
		specs.attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INT, FramebufferTextureFormat::Depth };
		m_Framebuffer = Framebuffer::Create(specs);

		// Icons
		m_Icons["play_button"] = Texture2D::Create(s_ToolbarIconPath + "play_button.png");
		m_Icons["stop_button"] = Texture2D::Create(s_ToolbarIconPath + "stop_button.png");
	}

	void HierarchyPanels::OnEvent(Event& event)
	{
		if (event.type() == KEY_DOWN)
		{
			bool ctrl = Input::IsKeyDown(Key::L_CTRL) || Input::IsKeyDown(Key::R_CTRL);
			bool shift = Input::IsKeyDown(Key::L_SHIFT) || Input::IsKeyDown(Key::R_SHIFT);
			switch (event.GetKeyCode())
			{
			case Key::D:
				if (ctrl && (IsHierarchyFocused() || IsViewportFocused()) && m_SelectedObject.IsValid())
				{
					m_Context->DuplicateObject(m_SelectedObject);
				}
				break;
			case Key::E:
				if (ctrl && (IsHierarchyFocused() || IsViewportFocused()) && m_SelectedObject.IsValid())
				{
					m_Context->DestroyGameObject(m_SelectedObject);
					m_SelectedObject.Reset();
				}
				break;
			}
		}

		OnViewportEvent(event);
	}

	void HierarchyPanels::OnUpdate(float dt)
	{
		OnViewportUpdate(dt);
	}

	void HierarchyPanels::OnImGuiRender(bool& hierarchy, bool& inspector, bool& viewport, bool& lib)
	{
		OnImGuiRenderHierarchyPanel(hierarchy);
		OnImGuiRenderInspectorPanel(inspector);
		OnImGuiRenderViewportPanel(viewport);
		OnImGuiRenderLibraryPanel(lib);
	}

	void HierarchyPanels::SetContext(Ref<Scene> scene)
	{
		if (m_Context == scene)
			return;
		m_Context = scene;
		m_SelectedObject.Reset();
	}

	Ref<Scene> HierarchyPanels::GetContext()
	{
		return m_Context;
	}

	GameObject HierarchyPanels::GetSelectedObject()
	{
		return m_SelectedObject;
	}

	bool HierarchyPanels::PollCommand(PanelCommand& command) {
		if (m_Commands.empty())
			return false;

		command = m_Commands.front();
		m_Commands.pop();
		return true;
	}

}