#include "HierarchyPanels.h"

namespace Hudi {

	HierarchyPanels::HierarchyPanels()
	{
		FramebufferSpecification specs;
		specs.width = 800;
		specs.height = 600;
		specs.attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RED_INT, FramebufferTextureFormat::Depth };
		m_Framebuffer = Framebuffer::Create(specs);
	}

	void HierarchyPanels::OnEvent(Event& event)
	{
		OnViewportEvent(event);
	}

	void HierarchyPanels::OnUpdate(float dt)
	{
		OnViewportUpdate(dt);
	}

	void HierarchyPanels::OnImGuiRender(bool& hierarchy, bool& inspector, bool& viewport)
	{
		OnImGuiRenderHierarchyPanel(hierarchy);
		OnImGuiRenderInspectorPanel(inspector);
		OnImGuiRenderViewportPanel(viewport);
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

}