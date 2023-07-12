#include "hdpch.h"

#define HD_EXTERN_MEMBER_VARS_DEFINE
#include "Event.h"

#include <SDL.h>

namespace Hudi {

	EventType Event::type() const { return (EventType)((SDL_Event*)m_Event)->type; }
	WindowEvent Event::WindowType() const { return (WindowEvent)((SDL_Event*)m_Event)->window.event; }

	KeyCode Event::GetKeyCode() const { return ((SDL_Event*)m_Event)->key.keysym.sym; }
	int Event::GetWheelYOffset() const { return ((SDL_Event*)m_Event)->wheel.y; }

	EventDispatcher::EventDispatcher(Event& event)
		: m_Event(event)
	{
	}

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
	
	EventManager* EventManager::s_Instance = nullptr;

	void EventManager::Init()
	{
		if (!s_Instance)
		{
			s_Instance = new EventManager();
		}

		Clear();
	}

	void EventManager::ShutDown() 
	{
		delete s_Instance;
		s_Instance = nullptr;
	}

	void EventManager::Clear()
	{
		s_Instance->s_QuitEvent = false;

		s_Instance->s_KeyJustDown.clear();
		s_Instance->s_KeyJustUp.clear();

		s_Instance->s_ButtonJustDown.clear();
		s_Instance->s_ButtonJustUp.clear();
	}

	void EventManager::Reset()
	{
		s_Instance->s_QuitEvent = false;
		s_Instance->s_KeyJustDown.clear();
		s_Instance->s_KeyJustUp.clear();
		s_Instance->s_KeyDownEvent.clear();
		s_Instance->s_ButtonJustDown.clear();
		s_Instance->s_ButtonJustUp.clear();
		s_Instance->s_MouseButtonDownEvent.clear();
	}

	void EventManager::ClearEventQueue()
	{
		SDL_Event e;
		while (SDL_PollEvent(&e));
	}

	void EventManager::SetCallBackFn(std::function<void(Event&)> fn)
	{
		s_Instance->s_CallBackFn = fn;
	}

	void EventManager::OnUpdate()
	{
		//SDL_PumpEvents();

		SDL_Event e;
		while (SDL_PollEvent(&e))
		{
			Event event;
			event.m_Event = &e;
			switch (e.type)
			{
			case QUIT:
			{
				s_Instance->s_QuitEvent = true;
				break;
			}
			case WINDOW_EVENT:
			{

			}
			case KEY_DOWN:
			{
				KeyCode code = e.key.keysym.sym;
				if (KeyUp(code))
					s_Instance->s_KeyJustDown.insert(code);
				s_Instance->s_KeyDownEvent.insert(code);
				break;
			}
			case KEY_UP:
			{
				KeyCode code = e.key.keysym.sym;
				if (KeyDown(code))
					s_Instance->s_KeyJustUp.insert(code);
				s_Instance->s_KeyDownEvent.erase(code);
				break;
			}
			case MOUSE_BUTTON_DOWN:
			{
				MouseCode code = e.button.button;
				if (MouseUp(code))
					s_Instance->s_ButtonJustDown.insert(code);
				s_Instance->s_MouseButtonDownEvent.insert(code);
				break;
			}
			case MOUSE_BUTTON_UP:
			{
				MouseCode code = e.button.button;
				if (MouseDown(code))
					s_Instance->s_ButtonJustUp.insert(code);
				s_Instance->s_MouseButtonDownEvent.erase(code);
				break;
			}
			case MOUSE_MOTION:
			{
				s_Instance->s_PreviousMousePos = s_Instance->s_CurrentMousePos;
				s_Instance->s_CurrentMousePos = { e.motion.x, e.motion.y };
				break;
			}
			case MOUSE_WHEEL:
			{
				break;
			}
			}
			HD_CORE_ASSERT(!s_Instance->s_CallBackFn, "No call back function has been set!");
			s_Instance->s_CallBackFn(event);
		}
	}

	bool EventManager::Quit()
	{
		return s_Instance->s_QuitEvent;
	}

	bool EventManager::KeyJustDown(KeyCode key)
	{
		return s_Instance->s_KeyJustDown.find(key) != s_Instance->s_KeyJustDown.end();
	}

	bool EventManager::KeyJustUp(KeyCode keycode)
	{
		return s_Instance->s_KeyJustUp.find(keycode) != s_Instance->s_KeyJustUp.end();
	}

	bool EventManager::KeyDown(KeyCode keycode)
	{
		return s_Instance->s_KeyDownEvent.find(keycode) != s_Instance->s_KeyDownEvent.end();
	}

	bool EventManager::KeyUp(KeyCode keycode)
	{
		return !KeyDown(keycode);
	}

	bool EventManager::MouseJustDown(MouseCode button)
	{
		return s_Instance->s_ButtonJustDown.find(button) != s_Instance->s_ButtonJustDown.end();
	}

	bool EventManager::MouseJustUp(MouseCode button)
	{
		return s_Instance->s_ButtonJustUp.find(button) != s_Instance->s_ButtonJustUp.end();
	}

	bool EventManager::MouseDown(MouseCode mousebuttoncode)
	{
		return s_Instance->s_MouseButtonDownEvent.find(mousebuttoncode) != s_Instance->s_MouseButtonDownEvent.end();
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

	glm::vec2 EventManager::MouseDeltaPos() { return s_Instance->s_CurrentMousePos - s_Instance->s_PreviousMousePos; }



}