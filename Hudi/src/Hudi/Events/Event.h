#pragma once

#include "hdpch.h"
#include "Hudi/Core/Core.h"

#include "Hudi/Source/KeyCodes.h"
#include "Hudi/Source/MouseCodes.h"

#include <SDL.h>

#include <glm/glm.hpp>

namespace Hudi {

	enum EventType 
	{
		QUIT				= SDL_QUIT,
		WINDOW_EVENT		= SDL_WINDOWEVENT,
		KEY_DOWN			= SDL_KEYDOWN,
		KEY_UP				= SDL_KEYUP,
		MOUSE_BUTTON_DOWN	= SDL_MOUSEBUTTONDOWN,
		MOUSE_BUTTON_UP		= SDL_MOUSEBUTTONUP,
		MOUSE_MOTION		= SDL_MOUSEMOTION,
		MOUSE_WHEEL			= SDL_MOUSEWHEEL
	};

	enum WindowEvent
	{
		WINDOWEVENT_RESIZED = SDL_WINDOWEVENT_RESIZED,
		WINDOWEVENT_SIZE_CHANGED = SDL_WINDOWEVENT_SIZE_CHANGED
	};

	class Event
	{
	public:
		Event() : m_Event(SDL_Event()) {}
		Event(SDL_Event e)
			: m_Event(e)
		{}

		EventType type() const { return (EventType)m_Event.type; }

		operator SDL_Event() const { return m_Event; }

	public:
		KeyCode GetKeyCode() const { return m_Event.key.keysym.sym; }
		int GetYOffset() const { return m_Event.wheel.y; }

	public:
		bool handled = false;
	private:
		SDL_Event m_Event;
		friend class EventManager;
	};

	class EventDispatcher
	{
	public:
		EventDispatcher(Event& event);

		bool Dispatch(EventType type, const std::function<void(Event&)>& fn);

	private:
		Event& m_Event;
	};


	class EventManager
	{
	public:
		static void Init();
		static void Clear();
		static void Reset();

		static void SetCallBackFn(std::function<void(Event&)> fn) { m_CallBackFn = fn; }
		static void SetWindowFn(std::function<void(Event&)> fn) { m_WindowEventFn = fn; }

		static void OnUpdate();
		
		static bool Quit();
		
		static bool KeyJustDown(KeyCode key);
		static bool KeyJustUp(KeyCode key);
		static bool KeyDown(KeyCode key);
		static bool KeyUp(KeyCode key);
		
		static bool MouseJustDown(MouseCode button);
		static bool MouseJustUp(MouseCode button);
		static bool MouseDown(MouseCode button);
		static bool MouseUp(MouseCode button);

		static glm::vec2 MousePosition();
	
	private:
		static std::function<void(Event&)> m_CallBackFn;
		static std::function<void(Event& e)> m_WindowEventFn;

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