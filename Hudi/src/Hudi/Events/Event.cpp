#include "hdpch.h"
#include "Event.h"

namespace Hudi {

	EventDispatcher::EventDispatcher(Event& event)
		: m_Event(event) 
	{}

	bool EventDispatcher::Dispatch(EventType type, const std::function<void(Event&)>& fn)
	{
		if (m_Event.handled || m_Event.type() != type)
		{
			return false;
		}

		fn(m_Event);
		return true;
	}

	/////////////////////////////////////////////////////////////////////////////////
	///// Event Manager /////////////////////////////////////////////////////////////
	/////////////////////////////////////////////////////////////////////////////////

	std::function<void(Event&)> EventManager::m_CallBackFn = nullptr;
	std::function<void(Event&)> EventManager::m_WindowEventFn = nullptr;

	bool EventManager::m_QuitEvent = false;

	std::unordered_set<KeyCode> EventManager::m_KeyJustDown;
	std::unordered_set<KeyCode> EventManager::m_KeyJustUp;
	std::unordered_set<KeyCode> EventManager::m_KeyDownEvent;

	std::unordered_set<MouseCode> EventManager::m_ButtonJustDown;
	std::unordered_set<MouseCode> EventManager::m_ButtonJustUp;
	std::unordered_set<MouseCode> EventManager::m_MouseButtonDownEvent;

	glm::vec2 EventManager::m_CurrentMousePos = { 0, 0 };
	glm::vec2 EventManager::m_PreviousMousePos = { 0, 0 };

	void EventManager::Init()
	{
		Clear();
	}

	void EventManager::Clear()
	{
		m_QuitEvent = false;

		m_KeyJustDown.clear();
		m_KeyJustUp.clear();

		m_ButtonJustDown.clear();
		m_ButtonJustUp.clear();
	}

	void EventManager::Reset()
	{
		m_QuitEvent = false;
		m_KeyJustDown.clear();
		m_KeyJustUp.clear();
		m_KeyDownEvent.clear();
		m_ButtonJustDown.clear();
		m_ButtonJustUp.clear();
		m_MouseButtonDownEvent.clear();
	}

	void EventManager::OnUpdate()
	{
		//SDL_PumpEvents();

		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			Event event(e);
			switch (e.type)
			{
				case QUIT:
				{
					m_QuitEvent = true;
					break;
				}
				case WINDOW_EVENT:
				{
					
				}
				case KEY_DOWN:
				{
					KeyCode code = e.key.keysym.sym;
					if (KeyUp(code))
						m_KeyJustDown.insert(code);
					m_KeyDownEvent.insert(code);
					break;
				}
				case KEY_UP:
				{
					KeyCode code = e.key.keysym.sym;
					if (KeyDown(code))
						m_KeyJustUp.insert(code);
					m_KeyDownEvent.erase(code);
					break;
				}
				case MOUSE_BUTTON_DOWN:
				{
					MouseCode code = e.button.button;
					if (MouseUp(code))
						m_ButtonJustDown.insert(code);
					m_MouseButtonDownEvent.insert(code);
					break;
				}
				case MOUSE_BUTTON_UP:
				{
					MouseCode code = e.button.button;
					if (MouseDown(code))
						m_ButtonJustUp.insert(code);
					m_MouseButtonDownEvent.erase(code);
					break;
				}
				case MOUSE_MOTION:
				{
					m_PreviousMousePos = m_CurrentMousePos;
					m_CurrentMousePos = { e.motion.x, e.motion.y };
					break;
				}
				case MOUSE_WHEEL:
				{
					break;
				}
			}
			HD_CORE_ASSERT(!m_WindowEventFn, "No window event function has been set!");
			m_WindowEventFn(event);
			
			HD_CORE_ASSERT(!m_CallBackFn, "No call back function has been set!");
			m_CallBackFn(event);
		}
	}

	bool EventManager::Quit()
	{
		return m_QuitEvent;
	}

	bool EventManager::KeyJustDown(KeyCode key)
	{
		return m_KeyJustDown.find(key) != m_KeyJustDown.end();
	}

	bool EventManager::KeyJustUp(KeyCode keycode)
	{
		return m_KeyJustUp.find(keycode) != m_KeyJustUp.end();
	}

	bool EventManager::KeyDown(KeyCode keycode)
	{
		return m_KeyDownEvent.find(keycode) != m_KeyDownEvent.end();
	}

	bool EventManager::KeyUp(KeyCode keycode)
	{
		return !KeyDown(keycode);
	}

	bool EventManager::MouseJustDown(MouseCode button)
	{
		return m_ButtonJustDown.find(button) != m_ButtonJustDown.end();
	}

	bool EventManager::MouseJustUp(MouseCode button)
	{
		return m_ButtonJustUp.find(button) != m_ButtonJustUp.end();
	}

	bool EventManager::MouseDown(MouseCode mousebuttoncode)
	{
		return m_MouseButtonDownEvent.find(mousebuttoncode) != m_MouseButtonDownEvent.end();
	}

	bool EventManager::MouseUp(MouseCode mousebuttoncode)
	{
		return !MouseDown(mousebuttoncode);
	}

	glm::vec2 EventManager::MousePosition()
	{
		int xpos, ypos;
		SDL_GetMouseState(&xpos, &ypos);

		return { xpos, ypos };
	}


}