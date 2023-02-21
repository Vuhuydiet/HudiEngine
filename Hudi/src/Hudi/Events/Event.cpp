#include "hdpch.h"
#include "Event.h"

namespace Hudi {

	int PollEvent(Event* e)
	{
		return SDL_PollEvent(e);
	}


	bool EventManager::m_QuitEvent = false;

	std::unordered_set<KeyCode> EventManager::m_KeyJustDown;
	std::unordered_set<KeyCode> EventManager::m_KeyJustUp;
	std::unordered_set<KeyCode> EventManager::m_KeyDownEvent;

	std::unordered_set<MouseCode> EventManager::m_ButtonJustDown;
	std::unordered_set<MouseCode> EventManager::m_ButtonJustUp;
	std::unordered_set<MouseCode> EventManager::m_MouseButtonDownEvent;

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

	void EventManager::OnUpdate()
	{
		Event e;
		while (PollEvent(&e))
		{
			switch (e.type)
			{
				case QUIT:
				{
					m_QuitEvent = true;
					break;
				}
				case KEYDOWN:
				{
					if (KeyUp((KeyCode)e.key.keysym.sym))
						m_KeyJustDown.insert((KeyCode)e.key.keysym.sym);
					m_KeyDownEvent.insert((KeyCode)e.key.keysym.sym);
					break;
				}
				case KEYUP:
				{
					if (KeyDown((KeyCode)e.key.keysym.sym))
						m_KeyJustUp.insert((KeyCode)e.key.keysym.sym);
					m_KeyDownEvent.erase((KeyCode)e.key.keysym.sym);
					break;
				}
				case MOUSEBUTTONDOWN:
				{
					if (MouseUp((MouseCode)e.button.button))
						m_ButtonJustDown.insert((MouseCode)e.button.button);
					m_MouseButtonDownEvent.insert((MouseCode)e.button.button);
					break;
				}
				case MOUSEBUTTONUP:
				{
					if (MouseDown((MouseCode)e.button.button))
						m_ButtonJustUp.insert((MouseCode)e.button.button);
					m_MouseButtonDownEvent.erase((MouseCode)e.button.button);
					break;
				}
				case MOUSEMOTION:
				{
					break;
				}
				case MOUSEWHEEL:
				{
					break;
				}
			}
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

	bool EventManager::KeyJustUp(KeyCode key)
	{
		return m_KeyJustUp.find(key) != m_KeyJustUp.end();
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
		return MouseDown(mousebuttoncode);
	}

	Vec2<int> EventManager::MousePosition()
	{
		Vec2<int> pos;
		SDL_GetMouseState(&pos.x, &pos.y);

		return pos;
	}


}