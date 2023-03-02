#pragma once

#include "hdpch.h"
#include "Hudi/Core/Core.h"

#include <SDL.h>

#include "Hudi/Source/KeyCodes.h"
#include "Hudi/Source/MouseCodes.h"
#include "Hudi/Source/Vec2.h"

namespace Hudi {

	enum EventType {
		QUIT				= SDL_QUIT,
		KEYDOWN				= SDL_KEYDOWN,
		KEYUP				= SDL_KEYUP,
		MOUSEBUTTONDOWN		= SDL_MOUSEBUTTONDOWN,
		MOUSEBUTTONUP		= SDL_MOUSEBUTTONUP,
		MOUSEMOTION			= SDL_MOUSEMOTION,
		MOUSEWHEEL			= SDL_MOUSEWHEEL
	};

	class Event
	{
	public:
		Event() {}
		Event(SDL_Event e)
			: m_Event(e) {}

		operator SDL_Event() const
		{
			return m_Event;
		}

		bool handled = false;
	private:
		SDL_Event m_Event;
		friend class EventManager;
	};

	class EventManager
	{
	public:
		static void Init();
		static void Clear();

		static void OnUpdate(std::queue<Event>& event_queue);
		
		static bool Quit();
		
		static bool KeyJustDown(KeyCode key);
		static bool KeyJustUp(KeyCode key);
		static bool KeyDown(KeyCode key);
		static bool KeyUp(KeyCode key);
		
		static bool MouseJustDown(MouseCode button);
		static bool MouseJustUp(MouseCode button);
		static bool MouseDown(MouseCode button);
		static bool MouseUp(MouseCode button);

		static Vec2<int> MousePosition();
	
	private:
		static bool m_QuitEvent;
		
		static std::unordered_set<KeyCode> m_KeyJustDown;
		static std::unordered_set<KeyCode> m_KeyJustUp;
		static std::unordered_set<KeyCode> m_KeyDownEvent;

		static std::unordered_set<MouseCode> m_ButtonJustDown;
		static std::unordered_set<MouseCode> m_ButtonJustUp;
		static std::unordered_set<MouseCode> m_MouseButtonDownEvent;
	};

	/*class HUDI_API Event
	{
		friend class EventPathcher;

	public:
		virtual EventType GetEventType() const { return EventType::None; }

		virtual const char* GetName() const { return "None"; }
		virtual std::string ToString() const { return GetName(); }

	protected:
		SDL_Event m_Event;

	private:
		friend int PollEvent(Event* e);
	};

	inline std::ostream& operator<< (std::ostream& os, const Event& e)
	{
		os << e.ToString();
	}*/

}